/* 
** ZABBIX
** Copyright (C) 2000-2005 SIA Zabbix
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "zbxserver.h"
#include "expression.h"
#include "evalfunc.h"
#include "common.h"
#include "db.h"
#include "log.h"
#include "zlog.h"

/******************************************************************************
 *                                                                            *
 * Function: trigger_get_N_functionid                                         *
 *                                                                            *
 * Purpose: explode short trigger expression to normal mode                   *
 *          {11}=1 explode to {hostX:keyY.functionZ(parameterN)}=1            *
 *                                                                            *
 * Parameters: short_expression - null terminated trigger expression          *
 *                                {11}=1 & {2346734}>5                        *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *
 * Author: Alexander Vladishev                                                *
 *                                                                            *
 * Comments: !!! Don't forget sync code with PHP !!!                          *
 *                                                                            *
 ******************************************************************************/
static int	trigger_get_N_functionid(char *short_expression, int n, zbx_uint64_t *functionid)
{
	const char	*__function_name = "trigger_get_N_functionid";

	typedef enum {
		EXP_NONE,
		EXP_FUNCTIONID
	} parsing_state_t;

	parsing_state_t	state = EXP_NONE;
	int		num = 0, ret = FAIL;
	char		*p_functionid = NULL;
	register char	*c;

	assert(short_expression);

	zabbix_log(LOG_LEVEL_DEBUG, "In %s() short_expression:'%s' n:%d)", __function_name, short_expression, n);

	for (c = short_expression; '\0' != *c && ret != SUCCEED; c++)
	{
		if ('{' == *c)
		{
			state = EXP_FUNCTIONID;
			p_functionid = c + 1;
		}
		else if ('}' == *c && EXP_FUNCTIONID == state && p_functionid)
		{
			*c = '\0';

			if (SUCCEED == is_uint64(p_functionid, functionid))
			{
				if (++num == n)
				{
					zabbix_log(LOG_LEVEL_DEBUG, "%s() functionid:" ZBX_FS_UI64, __function_name, *functionid);
					ret = SUCCEED;
				}
			}

			*c = '}';
			state = EXP_NONE;
		}
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End of %s():%s", __function_name, zbx_result_string(ret));

	return ret;
}

/******************************************************************************
 *                                                                            *
 * Function: DBget_trigger_expression_by_triggerid                            *
 *                                                                            *
 * Purpose: retrive trigger expression by triggerid                           *
 *                                                                            *
 * Parameters: triggerid - trigger identificator from database                *
 *             expression - result buffer                                     *
 *             max_expression_len - size of result buffer                     *
 *                                                                            *
 * Return value: upon successful completion return SUCCEED                    *
 *               otherwise FAIL                                               *
 *                                                                            *
 * Author: Alexander Vladishev                                                *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static int	DBget_trigger_expression_by_triggerid(zbx_uint64_t triggerid, char *expression, size_t max_expression_len)
{
	const char	*__function_name = "DBget_trigger_expression_by_triggerid";
	DB_RESULT	result;
	DB_ROW		row;
	int		ret = FAIL;

	assert(expression);

	zabbix_log(LOG_LEVEL_DEBUG, "In %s() triggerid:" ZBX_FS_UI64, __function_name, triggerid);

	result = DBselect("select expression from triggers where triggerid=" ZBX_FS_UI64,
			triggerid);

	if (NULL != (row = DBfetch(result)) && SUCCEED != DBis_null(row[0]))
	{
		zbx_strlcpy(expression, row[0], max_expression_len);
		zabbix_log(LOG_LEVEL_DEBUG, "%s() expression:'%s'", __function_name, expression);
		ret = SUCCEED;
	}

	DBfree_result(result);

	zabbix_log(LOG_LEVEL_DEBUG, "End of %s():%s", __function_name, zbx_result_string(ret));

	return ret;
}

/******************************************************************************
 *                                                                            *
 * Function: DBget_item_lastvalue_by_functionid                               *
 *                                                                            *
 * Purpose: retrive item lastvalue by functionid                              *
 *                                                                            *
 * Parameters: functionid - function identificator from database              *
 *             lastvalue - pointer to result buffer. Must be NULL             *
 *                                                                            *
 * Return value: upon successful completion return SUCCEED                    *
 *               otherwise FAIL                                               *
 *                                                                            *
 * Author: Alexander Vladishev                                                *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static int	DBget_item_lastvalue_by_functionid(zbx_uint64_t functionid, char **lastvalue)
{
	const char	*__function_name = "DBget_item_lastvalue_by_functionid";
	DB_RESULT	result;
	DB_ROW		row;
	DB_RESULT	h_result;
	DB_ROW		h_row;
	zbx_uint64_t	itemid, valuemapid;
	int		value_type, ret = FAIL;
	char		tmp[MAX_STRING_LEN];

	zabbix_log(LOG_LEVEL_DEBUG, "In %s() functionid:" ZBX_FS_UI64, __function_name, functionid);

	result = DBselect("select i.itemid,i.value_type,i.valuemapid,i.units,i.lastvalue from items i,functions f"
			" where i.itemid=f.itemid and f.functionid=" ZBX_FS_UI64,
			functionid);

	if (NULL != (row = DBfetch(result)) && SUCCEED != DBis_null(row[4]))
	{
		ZBX_STR2UINT64(itemid, row[0]);
		value_type = atoi(row[1]);
		ZBX_STR2UINT64(valuemapid, row[2]);

		switch (value_type) {
			case ITEM_VALUE_TYPE_LOG:
			case ITEM_VALUE_TYPE_TEXT:
				zbx_snprintf(tmp, sizeof(tmp), "select value from %s where itemid=" ZBX_FS_UI64 " order by id desc",
						value_type == ITEM_VALUE_TYPE_LOG ? "history_log" : "history_text",
						itemid);

				h_result = DBselectN(tmp, 1);

				if (NULL != (h_row = DBfetch(h_result)))
					*lastvalue = zbx_dsprintf(*lastvalue, "%s", h_row[0]);
				else
					*lastvalue = zbx_dsprintf(*lastvalue, "%s", row[4]);

				DBfree_result(h_result);
				break;
			case ITEM_VALUE_TYPE_STR:
				zbx_strlcpy(tmp, row[4], sizeof(tmp));

				replace_value_by_map(tmp, valuemapid);

				*lastvalue = zbx_dsprintf(*lastvalue, "%s", tmp);
				break;
			default:
				zbx_strlcpy(tmp, row[4], sizeof(tmp));

				if (SUCCEED != replace_value_by_map(tmp, valuemapid))
					add_value_suffix(tmp, sizeof(tmp), row[3], value_type);

				*lastvalue = zbx_dsprintf(*lastvalue, "%s", tmp);
				break;
		}
		ret = SUCCEED;
	}

	DBfree_result(result);

	if (NULL != *lastvalue)
		zabbix_log(LOG_LEVEL_DEBUG, "%s() lastvalue:'%s'", __function_name, *lastvalue);
	zabbix_log(LOG_LEVEL_DEBUG, "End of %s():%s", __function_name, zbx_result_string(ret));

	return ret;
}

/******************************************************************************
 *                                                                            *
 * Function: item_description                                                 *
 *                                                                            *
 * Purpose: substitute key parameters in the item description string          *
 *          with real values                                                  *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static void	item_description(char **data, const char *key)
{
	char	*p, *m, *str_out = NULL, params[MAX_STRING_LEN], param[MAX_STRING_LEN];

	if (2 /* key with parameters */ != parse_command(key, NULL, 0, params, sizeof(params)))
		return;

	p = *data;
	while (NULL != (m = strchr(p, '$')))
	{
		*m = '\0';
		str_out = zbx_strdcat(str_out, p);
		*m++ = '$';

		if (*m >= '1' && *m <= '9')
		{
			if (0 != get_param(params, *m - '0', param, sizeof(param)))
				*param = '\0';

			str_out = zbx_strdcat(str_out, param);
			p = m + 1;
		}
		else
		{
			str_out = zbx_strdcat(str_out, "$");
			p = m;
		}
	}

	if (NULL != str_out)
	{
		str_out = zbx_strdcat(str_out, p);
		zbx_free(*data);
		*data = str_out;
	}
}

/******************************************************************************
 *                                                                            *
 * Function: DBget_trigger_value_by_functionid                                *
 *                                                                            *
 * Purpose: retrive trigger value by functionid and field name                *
 *                                                                            *
 * Parameters: functionid - function identificator from database              *
 *             value - pointer to result buffer. Must be NULL                 *
 *             fieldname - field name in tables 'hosts' or 'items'            *
 *                                                                            *
 * Return value: upon successful completion return SUCCEED                    *
 *               otherwise FAIL                                               *
 *                                                                            *
 * Author: Alexander Vladishev                                                *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
#define ZBX_REQUEST_HOST_NAME		0
#define ZBX_REQUEST_HOST_IPADDRESS	1
#define ZBX_REQUEST_ITEM_NAME		4
#define ZBX_REQUEST_ITEM_KEY		5
static int	DBget_trigger_value_by_functionid(zbx_uint64_t functionid, char **replace_to, int request)
{
	const char	*__function_name = "DBget_trigger_value_by_functionid";
	DB_RESULT	result;
	DB_ROW		row;
	DB_ITEM		item;
	int		ret = FAIL;

	zabbix_log(LOG_LEVEL_DEBUG, "In %s() functionid:" ZBX_FS_UI64, __function_name, functionid);

	result = DBselect(
			"select %s,functions f"
			" where h.hostid=i.hostid"
				" and i.itemid=f.itemid"
				" and f.functionid=" ZBX_FS_UI64,
			ZBX_SQL_ITEM_SELECT,
			functionid);

	if (NULL != (row = DBfetch(result)))
	{
		DBget_item_from_db(&item, row);

		switch (request) {
		case ZBX_REQUEST_HOST_NAME:
			*replace_to = zbx_dsprintf(*replace_to, "%s", item.host_name);
			ret = SUCCEED;
			break;
		case ZBX_REQUEST_HOST_IPADDRESS:
			*replace_to = zbx_dsprintf(*replace_to, "%s", item.host_ip);
			ret = SUCCEED;
			break;
		case ZBX_REQUEST_ITEM_NAME:
			*replace_to = zbx_dsprintf(*replace_to, "%s", item.description);
			item_description(replace_to, item.key);
			ret = SUCCEED;
			break;
		case ZBX_REQUEST_ITEM_KEY:
			*replace_to = zbx_dsprintf(*replace_to, "%s", item.key);
			ret = SUCCEED;
			break;
		}
	}
	DBfree_result(result);

	return ret;
}

/******************************************************************************
 *                                                                            *
 * Function: str2double                                                       *
 *                                                                            *
 * Purpose: convert string to double                                          *
 *                                                                            *
 * Parameters: str - string to convert                                        *
 *                                                                            *
 * Return value: converted double value                                       *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments: the function automatically processes prefixes 'K','M','G'        *
 *                                                                            *
 ******************************************************************************/
double	str2double(char *str)
{
	if(str[strlen(str)-1] == 'K')
	{
		str[strlen(str)-1] = 0;
		return (double)1024*atof(str);
	}
	else if(str[strlen(str)-1] == 'M')
	{
		str[strlen(str)-1] = 0;
		return (double)1024*1024*atof(str);
	}
	else if(str[strlen(str)-1] == 'G')
	{
		str[strlen(str)-1] = 0;
		return (double)1024*1024*1024*atof(str);
	}
	return atof(str);
}


/******************************************************************************
 *                                                                            *
 * Function: delete_spaces                                                    *
 *                                                                            *
 * Purpose: delete all spaces                                                 *
 *                                                                            *
 * Parameters: c - string to delete spaces                                    *
 *                                                                            *
 * Return value:  the string wtihout spaces                                   *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	delete_spaces(char *c)
{
	int i,j;

	j=0;
	for(i=0;c[i]!=0;i++)
	{
		if( c[i] != ' ')
		{
			c[j]=c[i];
			j++;
		}
	}
	c[j]=0;
}

/******************************************************************************
 *                                                                            *
 * Function: evaluate_simple                                                  *
 *                                                                            *
 * Purpose: evaluate simple expression                                        *
 *                                                                            *
 * Parameters: exp - expression string                                        *
 *                                                                            *
 * Return value:  SUCCEED - evaluated succesfully, result - value of the exp  *
 *                FAIL - otherwise                                            *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments: format: <double> or <double> <operator> <double>                 *
 *                                                                            *
 *           It is recursive function!                                        *
 *                                                                            *
 ******************************************************************************/
int	evaluate_simple(double *result,char *exp,char *error,int maxerrlen)
{
	double	value1,value2;
	char	first[MAX_STRING_LEN],second[MAX_STRING_LEN];
	char 	*p;

	zabbix_log( LOG_LEVEL_DEBUG, "In evaluate_simple(%s)",
		exp);

/* Remove left and right spaces */
	lrtrim_spaces(exp);

/* Compress repeating - and +. Add prefix N to negative numebrs. */
	compress_signs(exp);

	/* We should process negative prefix, i.e. N123 == -123 */
	if( exp[0]=='N' && is_double_prefix(exp+1) == SUCCEED )
	{
/* str2double support prefixes */
		*result=-str2double(exp+1);
		return SUCCEED;
	}
	else if( exp[0]!='N' && is_double_prefix(exp) == SUCCEED )
	{
/* str2double support prefixes */
		*result=str2double(exp);
		return SUCCEED;
	}

	/* Operators with lowest priority come first */
	/* HIGHEST / * - + < > # = & | LOWEST */
	if( (p = strchr(exp,'|')) != NULL )
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);

		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s", error);
			zabbix_syslog("%s", error);
			return FAIL;
		}
		if( value1 == 1)
		{
			*result=value1;
			return SUCCEED;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s", error);
			zabbix_syslog("%s", error);
			return FAIL;
		}
		if( value2 == 1)
		{
			*result=value2;
			return SUCCEED;
		}
		*result=0;
		return SUCCEED;
	}
	if( (p = strchr(exp,'&')) != NULL )
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);

		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s", error);
			zabbix_syslog("%s", error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s", error);
			zabbix_syslog("%s", error);
			return FAIL;
		}
		if( (value1 == 1) && (value2 == 1) )
		{
			*result=1;
		}
		else
		{
			*result=0;
		}
		return SUCCEED;
	}
	if((p = strchr(exp,'=')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( cmp_double(value1,value2) ==0 )
		{
			*result=1;
		}
		else
		{
			*result=0;
		}
		return SUCCEED;
	}
	if((p = strchr(exp,'#')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( cmp_double(value1,value2) != 0 )
		{
			*result=1;
		}
		else
		{
			*result=0;
		}
		return SUCCEED;
	}
	if((p = strchr(exp,'>')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s", error);
			zabbix_syslog("%s", error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s", error);
			zabbix_syslog("%s", error);
			return FAIL;
		}
		if( value1 > value2 )
		{
			*result=1;
		}
		else
		{
			*result=0;
		}
		return SUCCEED;
	}
	if((p = strchr(exp,'<')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( value1 < value2 )
		{
			*result=1;
		}
		else
		{
			*result=0;
		}
		zabbix_log(LOG_LEVEL_DEBUG, "Result [" ZBX_FS_DBL "]",*result );
		return SUCCEED;
	}
	if((p = strchr(exp,'+')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		*result=value1+value2;
		return SUCCEED;
	}
	if((p = strchr(exp,'-')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		*result=value1-value2;
		return SUCCEED;
	}
	if((p = strchr(exp,'*')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		*result=value1*value2;
		return SUCCEED;
	}
	if((p = strchr(exp,'/')) != NULL)
	{
		*p=0;
		strscpy( first, exp);
		*p='|';
		p++;
		strscpy( second, p);
		if( evaluate_simple(&value1,first,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if( evaluate_simple(&value2,second,error,maxerrlen) == FAIL )
		{
			zabbix_log(LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		if(cmp_double(value2,0) == 0)
		{
			zbx_snprintf(error,maxerrlen,"Division by zero. Cannot evaluate expression [%s/%s]",
				first,
				second);
			zabbix_log(LOG_LEVEL_WARNING, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return FAIL;
		}
		else
		{
			*result=value1/value2;
		}
		return SUCCEED;
	}
	else
	{
		zbx_snprintf(error,maxerrlen,"Format error or unsupported operator.  Exp: [%s]",
			exp);
		zabbix_log(LOG_LEVEL_WARNING, "%s",
			error);
		zabbix_syslog("%s",
			error);
		return FAIL;
	}

	return SUCCEED;
}

/******************************************************************************
 *                                                                            *
 * Function: evaluate                                                         *
 *                                                                            *
 * Purpose: evaluate simplified expression                                    *
 *                                                                            *
 * Parameters: exp - expression string                                        *
 *                                                                            *
 * Return value:  SUCCEED - evaluated succesfully, result - value of the exp  *
 *                FAIL - otherwise                                            *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments: example: ({15}>10)|({123}=1)                                     *
 *                                                                            *
 ******************************************************************************/
int	evaluate(int *result, char *exp, char *error, int maxerrlen)
{
	double	value;
	char	*res;
	char	simple[MAX_STRING_LEN];
	char	tmp[MAX_STRING_LEN];
	char	value_str[MAX_STRING_LEN];
	int	i,l,r;
	char	c;
	int	t;

	zabbix_log(LOG_LEVEL_DEBUG, "In evaluate(%s)",
		exp);

	res = NULL;

	strscpy(tmp, exp);
	t=0;
	while( find_char( tmp, ')' ) != FAIL )
	{
		l=-1;
		r=find_char(tmp,')');
		for(i=r;i>=0;i--)
		{
			if( tmp[i] == '(' )
			{
				l=i;
				break;
			}
		}
		if( l == -1 )
		{
			zbx_snprintf(error, maxerrlen, "Cannot find left bracket [(]. Expression:[%s]",
				tmp);
			zabbix_log(LOG_LEVEL_WARNING, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return	FAIL;
		}
		for(i=l+1;i<r;i++)
		{
			simple[i-l-1]=tmp[i];
		} 
		simple[r-l-1]=0;

		if( evaluate_simple( &value, simple, error, maxerrlen ) != SUCCEED )
		{
			/* Changed to LOG_LEVEL_DEBUG */
			zabbix_log( LOG_LEVEL_DEBUG, "%s",
				error);
			zabbix_syslog("%s",
				error);
			return	FAIL;
		}

		/* res = first+simple+second */
		c=tmp[l]; tmp[l]='\0';
		res = zbx_strdcat(res, tmp);
		tmp[l]=c;

		zbx_snprintf(value_str,MAX_STRING_LEN-1,"%lf",
			value);
		res = zbx_strdcat(res, value_str);
		res = zbx_strdcat(res, tmp+r+1);

		delete_spaces(res);
		strscpy(tmp,res);

		zbx_free(res); res = NULL;
	}
	if( evaluate_simple( &value, tmp, error, maxerrlen ) != SUCCEED )
	{
		zabbix_log(LOG_LEVEL_WARNING, "%s",
			error);
		zabbix_syslog("%s",
			error);
		return	FAIL;
	}
	if(cmp_double(value,0) == 0)
	{
		*result = TRIGGER_VALUE_FALSE;
	}
	else
	{
		*result = TRIGGER_VALUE_TRUE;
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End evaluate(result:%lf)",
		value);

	return SUCCEED;
}

/******************************************************************************
 *                                                                            *
 * Function: extract_numbers                                                  *
 *                                                                            *
 * Purpose: Extract from string numbers with prefixes (A-Z)                   *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments: !!! Don't forget sync code with PHP !!!                          *
 *           Use zbx_free_numbers to free allocated memory                    *
 *                                                                            *
 ******************************************************************************/
static char**	extract_numbers(char *str, int *count)
{
	char *s = NULL;
	char *e = NULL;

	char **result = NULL;

	int	dot_founded = 0;
	int	len = 0;

	assert(count);

	*count = 0;

	/* find start of number */
	for ( s = str; *s; s++)
	{
		if ( !isdigit(*s) ) {
			continue; /* for s */
		}

		if ( s != str && '{' == *(s-1) ) {
			/* skip functions '{65432}' */
			s = strchr(s, '}');
			continue; /* for s */
		}

		dot_founded = 0;
		/* find end of number */
		for ( e = s; *e; e++ )
		{
			if ( isdigit(*e) ) {
				continue; /* for e */
			}
			else if ( '.' == *e && !dot_founded ) {
				dot_founded = 1;
				continue; /* for e */
			}
			else if ( *e >= 'A' && *e <= 'Z' )
			{
				e++;
			}
			break; /* for e */
		}

		/* number founded */
		len = e - s;
		(*count)++;
		result = zbx_realloc(result, sizeof(char*) * (*count));
		result[(*count)-1] = zbx_malloc(NULL, len + 1);
		memcpy(result[(*count)-1], s, len);
		result[(*count)-1][len] = '\0';

		s = e;
		if (*s == '\0')
			break;
	}

	return result;
}

static void	zbx_free_numbers(char ***numbers, int count)
{
	register int i = 0;

	if ( !numbers ) return;
	if ( !*numbers ) return;

	for ( i = 0; i < count; i++ )
	{
		zbx_free((*numbers)[i]);
	}

	zbx_free(*numbers);
}

/******************************************************************************
 *                                                                            *
 * Function: expand_trigger_description_constants                             *
 *                                                                            *
 * Purpose: substitute simple macros in data string with real values          *
 *                                                                            *
 * Parameters: data - trigger description                                     *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments: !!! Don't forget sync code with PHP !!!                          *
 *           replcae ONLY $1-9 macros NOT {HOSTNAME}                          *
 *                                                                            *
 ******************************************************************************/
static void	expand_trigger_description_constants(
		char **data,
		zbx_uint64_t triggerid
	)
{
	DB_RESULT db_trigger;
	DB_ROW	db_trigger_data;

	char	**numbers = NULL;
	int	numbers_cnt = 0;

	int	i = 0;

	char	*new_str = NULL;

	char	replace[3] = "$0";

	db_trigger = DBselect("select expression from triggers where triggerid=" ZBX_FS_UI64, triggerid);

	if ( (db_trigger_data = DBfetch(db_trigger)) ) {

		numbers = extract_numbers(db_trigger_data[0], &numbers_cnt);

		for ( i = 0; i < 9; i++ )
		{
			replace[1] = '0' + i + 1;
			new_str = string_replace(
					*data,
					replace, 
					i < numbers_cnt ? 
						numbers[i] :
						""
					);
			zbx_free(*data);
			*data = new_str;
		}

		zbx_free_numbers(&numbers, numbers_cnt);
	}

	DBfree_result(db_trigger);
}

/******************************************************************************
 *                                                                            *
 * Function: get_host_profile_value_by_triggerid                              *
 *                                                                            *
 * Purpose: request host profile value by triggerid and field name            *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value: returns requested host profile value                         *
 *                      or *UNKNOWN* if profile is not defined                *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
#define STR_UNKNOWN_VARIABLE		"*UNKNOWN*"

static char	*get_host_profile_value_by_triggerid(char *dst, zbx_uint64_t triggerid, const char *fieldname)
{
	DB_RESULT	result;
	DB_ROW		row;

	result = DBselect("select distinct p.%s from triggers t,functions f,items i,hosts h,hosts_profiles p"
			" where t.triggerid=" ZBX_FS_UI64 " and t.triggerid=f.triggerid and i.itemid=f.itemid"
			" and h.hostid=i.hostid and p.hostid=h.hostid", 
			fieldname,
			triggerid);

	if (NULL == (row = DBfetch(result)) || SUCCEED == DBis_null(row[0]))
	{
		zabbix_log(LOG_LEVEL_DEBUG, "No PROFILE.%s in substitute_simple_macros. Triggerid [" ZBX_FS_UI64 "]",
				fieldname,
				triggerid);

		dst = zbx_dsprintf(dst, "%s", STR_UNKNOWN_VARIABLE);
	}
	else
		dst = zbx_dsprintf(dst, "%s", row[0]);

	DBfree_result(result);

	return dst;
}

/******************************************************************************
 *                                                                            *
 * Function: substitute_simple_macros                                         *
 *                                                                            *
 * Purpose: substitute simple macros in data string with real values          *
 *                                                                            *
 * Parameters: trigger - trigger structure                                    *
 *             action - action structure (NULL if unknown)                    *
 *             escalation - escalation structure. used for recovery           *
 *                          messages in {ESC.HISTORY} macro.                  *
 *                          (NULL for other cases)                            * 
 *             data - data string                                             *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments: {DATE},{TIME},{HOSTNAME},{IPADDRESS},{STATUS},                   *
 *           {TRIGGER.NAME}, {TRIGGER.KEY}, {TRIGGER.SEVERITY}                *
 *                                                                            *
 ******************************************************************************/
/* definition of macros variables */
#define MVAR_DATE			"{DATE}"
#define MVAR_EVENT_ID			"{EVENT.ID}"
#define MVAR_EVENT_DATE			"{EVENT.DATE}"
#define MVAR_EVENT_TIME			"{EVENT.TIME}"
#define MVAR_EVENT_AGE			"{EVENT.AGE}"
#define MVAR_ESC_HISTORY		"{ESC.HISTORY}"
#define MVAR_HOSTNAME			"{HOSTNAME}"
#define MVAR_IPADDRESS			"{IPADDRESS}"
#define MVAR_HOST_DNS			"{HOST.DNS}"
#define MVAR_HOST_CONN			"{HOST.CONN}"
#define MVAR_TIME			"{TIME}"
#define MVAR_ITEM_LASTVALUE		"{ITEM.LASTVALUE}"
#define MVAR_ITEM_NAME			"{ITEM.NAME}"
#define MVAR_TRIGGER_COMMENT		"{TRIGGER.COMMENT}"
#define MVAR_TRIGGER_ID			"{TRIGGER.ID}"
#define MVAR_TRIGGER_KEY		"{TRIGGER.KEY}"
#define MVAR_TRIGGER_NAME		"{TRIGGER.NAME}"
#define MVAR_TRIGGER_SEVERITY		"{TRIGGER.SEVERITY}"
#define MVAR_TRIGGER_NSEVERITY		"{TRIGGER.NSEVERITY}"
#define MVAR_TRIGGER_STATUS		"{TRIGGER.STATUS}"
#define MVAR_TRIGGER_STATUS_OLD		"{STATUS}"
#define MVAR_TRIGGER_VALUE		"{TRIGGER.VALUE}"
#define MVAR_TRIGGER_URL		"{TRIGGER.URL}"
#define MVAR_PROFILE_DEVICETYPE		"{PROFILE.DEVICETYPE}"
#define MVAR_PROFILE_NAME		"{PROFILE.NAME}"
#define MVAR_PROFILE_OS			"{PROFILE.OS}"
#define MVAR_PROFILE_SERIALNO		"{PROFILE.SERIALNO}"
#define MVAR_PROFILE_TAG		"{PROFILE.TAG}"
#define MVAR_PROFILE_MACADDRESS		"{PROFILE.MACADDRESS}"
#define MVAR_PROFILE_HARDWARE		"{PROFILE.HARDWARE}"
#define MVAR_PROFILE_SOFTWARE		"{PROFILE.SOFTWARE}"
#define MVAR_PROFILE_CONTACT		"{PROFILE.CONTACT}"
#define MVAR_PROFILE_LOCATION		"{PROFILE.LOCATION}"
#define MVAR_PROFILE_NOTES		"{PROFILE.NOTES}"

void	substitute_simple_macros(DB_EVENT *event, DB_ACTION *action, DB_ITEM *item, DB_ESCALATION *escalation, char **data, int macro_type)
{

	char
		*pl,
		*pr = NULL,
		*str_out = NULL,
		*replace_to = NULL;

	char	*buf = NULL;
	int	buf_offset, buf_allocated = MAX_STRING_LEN;
	int	status, esc_step;
	int	var_len;

	time_t  now;
	struct  tm      *tm;

	DB_RESULT	result;
	DB_ROW		row;

	char		expression[TRIGGER_EXPRESSION_LEN_MAX];
	zbx_uint64_t	functionid;
	int		ret;

	if (NULL == data || NULL == *data || '\0' == **data)
	{
		zabbix_log(LOG_LEVEL_DEBUG, "In substitute_simple_macros(data:NULL)");
		return;
	}
	
	zabbix_log(LOG_LEVEL_DEBUG, "In substitute_simple_macros (data:\"%s\")",
			*data);

	if ( macro_type & MACRO_TYPE_TRIGGER_DESCRIPTION ) {
		expand_trigger_description_constants(data, event->objectid);
	}

	pl = *data;
	if (NULL == (pr = strchr(pl, '{')))
		return;

	for ( ; NULL != pr; pr = strchr(pl, '{'))
	{
		pr[0] = '\0';
/*zabbix_log(LOG_LEVEL_DEBUG, "str_out1 [%s] pl [%s]", str_out, pl);*/
		str_out = zbx_strdcat(str_out, pl);
/*zabbix_log(LOG_LEVEL_DEBUG, "str_out1 [%s] pl [%s]", str_out, pl);*/
		pr[0] = '{';

		replace_to = zbx_dsprintf(replace_to, "{");
		var_len = 1;

		if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_TRIGGER_NAME, strlen(MVAR_TRIGGER_NAME)) == 0)
		{
			var_len = strlen(MVAR_TRIGGER_NAME);
			replace_to = zbx_dsprintf(replace_to, "%s", event->trigger_description);
			/* Why it was here? *//* For substituting macros in trigger description :) */
			substitute_simple_macros(event, action, item, escalation, &replace_to, MACRO_TYPE_TRIGGER_DESCRIPTION);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_TRIGGER_COMMENT, strlen(MVAR_TRIGGER_COMMENT)) == 0)
		{
			var_len = strlen(MVAR_TRIGGER_COMMENT);
			replace_to = zbx_dsprintf(replace_to, "%s", event->trigger_comments);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_DEVICETYPE, strlen(MVAR_PROFILE_DEVICETYPE)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_DEVICETYPE);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "devicetype");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_NAME, strlen(MVAR_PROFILE_NAME)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_NAME);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "name");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_OS, strlen(MVAR_PROFILE_OS)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_OS);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "os");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_SERIALNO, strlen(MVAR_PROFILE_SERIALNO)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_SERIALNO);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "serialno");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_TAG, strlen(MVAR_PROFILE_TAG)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_TAG);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "tag");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_MACADDRESS, strlen(MVAR_PROFILE_MACADDRESS)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_MACADDRESS);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "macaddress");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_HARDWARE, strlen(MVAR_PROFILE_HARDWARE)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_HARDWARE);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "hardware");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_SOFTWARE, strlen(MVAR_PROFILE_SOFTWARE)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_SOFTWARE);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "software");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_CONTACT, strlen(MVAR_PROFILE_CONTACT)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_CONTACT);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "contact");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_LOCATION, strlen(MVAR_PROFILE_LOCATION)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_LOCATION);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "location");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_PROFILE_NOTES, strlen(MVAR_PROFILE_NOTES)) == 0)
		{
			var_len = strlen(MVAR_PROFILE_NOTES);
			replace_to = get_host_profile_value_by_triggerid(replace_to, event->objectid, "notes");
		}
		else if ((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY | MACRO_TYPE_TRIGGER_DESCRIPTION)) &&
				0 == strncmp(pr, MVAR_HOSTNAME, strlen(MVAR_HOSTNAME)))
		{
			var_len = strlen(MVAR_HOSTNAME);

			ret = DBget_trigger_expression_by_triggerid(event->objectid, expression, sizeof(expression));
			if (SUCCEED == ret)
				ret = trigger_get_N_functionid(expression, 1, &functionid);
			if (SUCCEED == ret)
				ret = DBget_trigger_value_by_functionid(functionid, &replace_to,
						ZBX_REQUEST_HOST_NAME);
			if (FAIL == ret)
			{
				zabbix_log(LOG_LEVEL_DEBUG, "No %s in substitute_simple_macros. Triggerid [" ZBX_FS_UI64 "]",
						pr, event->objectid);
				replace_to = zbx_dsprintf(replace_to, "%s", STR_UNKNOWN_VARIABLE);
			}
		}
		else if ((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
				0 == strncmp(pr, MVAR_ITEM_NAME, strlen(MVAR_ITEM_NAME)))
		{
			var_len = strlen(MVAR_ITEM_NAME);

			ret = DBget_trigger_expression_by_triggerid(event->objectid, expression, sizeof(expression));
			if (SUCCEED == ret)
				ret = trigger_get_N_functionid(expression, 1, &functionid);
			if (SUCCEED == ret)
				ret = DBget_trigger_value_by_functionid(functionid, &replace_to,
						ZBX_REQUEST_ITEM_NAME);
			if (FAIL == ret)
			{
				zabbix_log(LOG_LEVEL_DEBUG, "No %s in substitute_simple_macros. Triggerid [" ZBX_FS_UI64 "]",
						pr, event->objectid);
				replace_to = zbx_dsprintf(replace_to, "%s", STR_UNKNOWN_VARIABLE);
			}
		}
		else if ((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY | MACRO_TYPE_TRIGGER_DESCRIPTION)) &&
				0 == strncmp(pr, MVAR_ITEM_LASTVALUE, strlen(MVAR_ITEM_LASTVALUE)))
		{
			var_len = strlen(MVAR_ITEM_LASTVALUE);

			ret = DBget_trigger_expression_by_triggerid(event->objectid, expression, sizeof(expression));
			if (SUCCEED == ret)
				ret = trigger_get_N_functionid(expression, 1, &functionid);
			if (SUCCEED == ret)
				ret = DBget_item_lastvalue_by_functionid(functionid, &replace_to);
			if (FAIL == ret)
			{
				zabbix_log(LOG_LEVEL_DEBUG, "No %s in substitute_simple_macros. Triggerid [" ZBX_FS_UI64 "]",
						pr, event->objectid);
				replace_to = zbx_dsprintf(replace_to, "%s", STR_UNKNOWN_VARIABLE);
			}
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_TRIGGER_KEY, strlen(MVAR_TRIGGER_KEY)) == 0)
		{
			var_len = strlen(MVAR_TRIGGER_KEY);

			ret = DBget_trigger_expression_by_triggerid(event->objectid, expression, sizeof(expression));
			if (SUCCEED == ret)
				ret = trigger_get_N_functionid(expression, 1, &functionid);
			if (SUCCEED == ret)
				ret = DBget_trigger_value_by_functionid(functionid, &replace_to,
						ZBX_REQUEST_ITEM_KEY);
			if (FAIL == ret)
			{
				zabbix_log(LOG_LEVEL_DEBUG, "No %s in substitute_simple_macros. Triggerid [" ZBX_FS_UI64 "]",
						pr, event->objectid);
				replace_to = zbx_dsprintf(replace_to, "%s", STR_UNKNOWN_VARIABLE);
			}
		}
		else if ((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
				0 == strncmp(pr, MVAR_IPADDRESS, strlen(MVAR_IPADDRESS)))
		{
			var_len = strlen(MVAR_IPADDRESS);

			ret = DBget_trigger_expression_by_triggerid(event->objectid, expression, sizeof(expression));
			if (SUCCEED == ret)
				ret = trigger_get_N_functionid(expression, 1, &functionid);
			if (SUCCEED == ret)
				ret = DBget_trigger_value_by_functionid(functionid, &replace_to,
						ZBX_REQUEST_HOST_IPADDRESS);
			if (FAIL == ret)
			{
				zabbix_log(LOG_LEVEL_DEBUG, "No %s in substitute_simple_macros. Triggerid [" ZBX_FS_UI64 "]",
						pr, event->objectid);
				replace_to = zbx_dsprintf(replace_to, "%s", STR_UNKNOWN_VARIABLE);
			}
		}
		else if ((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
				0 == strncmp(pr, MVAR_DATE, strlen(MVAR_DATE)))
		{
			var_len	= strlen(MVAR_DATE);
			now	= time(NULL);
			tm	= localtime(&now);
			replace_to = zbx_dsprintf(replace_to, "%.4d.%.2d.%.2d",
					tm->tm_year + 1900,
					tm->tm_mon + 1,
					tm->tm_mday);
		}
		else if ((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
				0 == strncmp(pr, MVAR_TIME, strlen(MVAR_TIME)))
		{
			var_len	= strlen(MVAR_TIME);
			now	= time(NULL);
			tm	= localtime(&now);
			replace_to = zbx_dsprintf(replace_to, "%.2d:%.2d:%.2d",
					tm->tm_hour,
					tm->tm_min,
					tm->tm_sec);

		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_TRIGGER_STATUS, strlen(MVAR_TRIGGER_STATUS)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS_OLD block */
			var_len = strlen(MVAR_TRIGGER_STATUS);
			replace_to = zbx_dsprintf(replace_to, "%s",
					event->value == TRIGGER_VALUE_TRUE ? "PROBLEM" : "OK");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) && 
			strncmp(pr, MVAR_TRIGGER_STATUS_OLD, strlen(MVAR_TRIGGER_STATUS_OLD)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_TRIGGER_STATUS_OLD);
			replace_to = zbx_dsprintf(replace_to, "%s",
					event->value == TRIGGER_VALUE_TRUE ? "PROBLEM" : "OK");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_TRIGGER_ID, strlen(MVAR_TRIGGER_ID)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_TRIGGER_ID);
			replace_to = zbx_dsprintf(replace_to, ZBX_FS_UI64,
				event->objectid);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY | MACRO_TYPE_TRIGGER_EXPRESSION)) &&
			strncmp(pr, MVAR_TRIGGER_VALUE, strlen(MVAR_TRIGGER_VALUE)) == 0)
		{
			var_len = strlen(MVAR_TRIGGER_VALUE);
			replace_to = zbx_dsprintf(replace_to, "%d",
				event->value);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_TRIGGER_URL, strlen(MVAR_TRIGGER_URL)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_TRIGGER_URL);
			replace_to = zbx_dsprintf(replace_to, "%s",
				event->trigger_url);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_EVENT_ID, strlen(MVAR_EVENT_ID)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_EVENT_ID);
			replace_to = zbx_dsprintf(replace_to, ZBX_FS_UI64,
				event->eventid);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_EVENT_DATE, strlen(MVAR_EVENT_DATE)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_EVENT_DATE);
			now	= event->clock;
			tm	= localtime(&now);
			replace_to = zbx_dsprintf(replace_to, "%.4d.%.2d.%.2d",
				tm->tm_year + 1900,
				tm->tm_mon + 1,
				tm->tm_mday);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_EVENT_TIME, strlen(MVAR_EVENT_TIME)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_EVENT_TIME);
			now	= event->clock;
			tm	= localtime(&now);
			replace_to = zbx_dsprintf(replace_to, "%.2d:%.2d:%.2d",
				tm->tm_hour,
				tm->tm_min,
				tm->tm_sec);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_EVENT_AGE, strlen(MVAR_EVENT_AGE)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_EVENT_AGE);
			now		= time(NULL) - event->clock;
			replace_to	= zbx_dsprintf(replace_to, "%s", zbx_age2str(now));
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) &&
			strncmp(pr, MVAR_ESC_HISTORY, strlen(MVAR_ESC_HISTORY)) == 0)
		{
			/* NOTE: if you make changes for this bloc, don't forgot MVAR_TRIGGER_STATUS block */
			var_len = strlen(MVAR_ESC_HISTORY);

			if (NULL == buf)
				buf = zbx_malloc(buf, buf_allocated);
			buf_offset	= 0;
			*buf		= '\0';

			result = DBselect("select clock from events where eventid=" ZBX_FS_UI64,
					escalation != NULL ? escalation->eventid : event->eventid);

			if (NULL != (row = DBfetch(result)))
			{
				now	= (time_t)atoi(row[0]);
				tm	= localtime(&now);
				zbx_snprintf_alloc(&buf, &buf_allocated, &buf_offset, 64,
						"Problem started: %.4d.%.2d.%.2d %.2d:%.2d:%.2d Age: %s\n",
						tm->tm_year + 1900,
						tm->tm_mon + 1,
						tm->tm_mday,
						tm->tm_hour,
						tm->tm_min,
						tm->tm_sec,
						zbx_age2str(time(NULL) - now));
			}

			DBfree_result(result);

			result = DBselect("select a.clock,a.status,m.description,a.sendto,a.error,a.esc_step"
					" from alerts a left join media_type m on m.mediatypeid = a.mediatypeid"
					" where a.eventid=" ZBX_FS_UI64 " and a.alerttype=%d order by a.clock",
					escalation != NULL ? escalation->eventid : event->eventid,
					ALERT_TYPE_MESSAGE);

			while (NULL != (row = DBfetch(result))) {
				now	 = atoi(row[0]);
				tm	 = localtime(&now);
				status	 = atoi(row[1]);
				esc_step = atoi(row[5]);

				if (esc_step != 0)
					zbx_snprintf_alloc(&buf, &buf_allocated, &buf_offset, 16, "%d. ", esc_step);

				zbx_snprintf_alloc(&buf, &buf_allocated, &buf_offset, 256,
						"%.4d.%.2d.%.2d %.2d:%.2d:%.2d %-11s %s %s %s\n",
						tm->tm_year + 1900,
						tm->tm_mon + 1,
						tm->tm_mday,
						tm->tm_hour,
						tm->tm_min,
						tm->tm_sec,
						(status == ALERT_STATUS_NOT_SENT ? "in progress" :
							(status == ALERT_STATUS_SENT ? "sent" : "failed")),
						SUCCEED == DBis_null(row[2]) ? "" : row[2],
						row[3],
						row[4]);
			}

			DBfree_result(result);

			if (escalation != NULL && escalation->r_eventid == event->eventid)
			{
				now	= (time_t)event->clock;
				tm	= localtime(&now);
				zbx_snprintf_alloc(&buf, &buf_allocated, &buf_offset, 64,
						"Problem ended: %.4d.%.2d.%.2d %.2d:%.2d:%.2d\n",
						tm->tm_year + 1900,
						tm->tm_mon + 1,
						tm->tm_mday,
						tm->tm_hour,
						tm->tm_min,
						tm->tm_sec);
			}

			if (0 != buf_offset)
				buf[--buf_offset] = '\0';

			replace_to = zbx_dsprintf(replace_to, "%s", buf);
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) && 
			strncmp(pr, MVAR_TRIGGER_SEVERITY, strlen(MVAR_TRIGGER_SEVERITY)) == 0)
		{
			var_len = strlen(MVAR_TRIGGER_SEVERITY);

			if(event->trigger_priority == 0)	replace_to = zbx_dsprintf(replace_to, "Not classified");
			else if(event->trigger_priority == 1)	replace_to = zbx_dsprintf(replace_to, "Information");
			else if(event->trigger_priority == 2)	replace_to = zbx_dsprintf(replace_to, "Warning");
			else if(event->trigger_priority == 3)	replace_to = zbx_dsprintf(replace_to, "Average");
			else if(event->trigger_priority == 4)	replace_to = zbx_dsprintf(replace_to, "High");
			else if(event->trigger_priority == 5)	replace_to = zbx_dsprintf(replace_to, "Disaster");
			else					replace_to = zbx_dsprintf(replace_to, "Unknown");
		}
		else if((macro_type & (MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY)) && 
			strncmp(pr, MVAR_TRIGGER_NSEVERITY, strlen(MVAR_TRIGGER_NSEVERITY)) == 0)
		{
			var_len = strlen(MVAR_TRIGGER_NSEVERITY);
			replace_to = zbx_dsprintf(replace_to, "%d", event->trigger_priority);
		}
		else if (macro_type & (MACRO_TYPE_ITEM_KEY | MACRO_TYPE_HOST_IPMI_IP))
		{
			if (0 == strncmp(pr, MVAR_HOSTNAME, strlen(MVAR_HOSTNAME)))
			{
				var_len = strlen(MVAR_HOSTNAME);
				replace_to = zbx_dsprintf(replace_to, "%s", item->host_name);
			}
			else if (0 == strncmp(pr, MVAR_IPADDRESS, strlen(MVAR_IPADDRESS)))
			{
				var_len = strlen(MVAR_IPADDRESS);
				replace_to = zbx_dsprintf(replace_to, "%s", item->host_ip);
			}
			else if (0 == strncmp(pr, MVAR_HOST_DNS, strlen(MVAR_HOST_DNS)))
			{
				var_len = strlen(MVAR_HOST_DNS);
				replace_to = zbx_dsprintf(replace_to, "%s", item->host_dns);
			}
			else if (0 == strncmp(pr, MVAR_HOST_CONN, strlen(MVAR_HOST_CONN)))
			{
				var_len = strlen(MVAR_HOST_CONN);
				replace_to = zbx_dsprintf(replace_to, "%s", item->useip ? item->host_ip : item->host_dns);
			}
		}

/*zabbix_log(LOG_LEVEL_DEBUG, "str_out2 [%s] replace_to [%s]", str_out, replace_to);*/
		str_out = zbx_strdcat(str_out, replace_to);
/*zabbix_log(LOG_LEVEL_DEBUG, "str_out2 [%s] replace_to [%s]", str_out, replace_to);*/
		pl = pr + var_len;

		zbx_free(replace_to);
	}
/*zabbix_log(LOG_LEVEL_DEBUG, "str_out3 [%s] pl [%s]", str_out, pl);*/
	str_out = zbx_strdcat(str_out, pl);
/*zabbix_log(LOG_LEVEL_DEBUG, "str_out3 [%s] pl [%s]", str_out, pl);*/

	zbx_free(*data);
	zbx_free(buf);

	*data = str_out;

	zabbix_log(LOG_LEVEL_DEBUG, "End substitute_simple_macros (result:%s)",
		*data);
}

/******************************************************************************
 *                                                                            *
 * Function: substitute_macros                                                *
 *                                                                            *
 * Purpose: substitute macros in data string with real values                 *
 *                                                                            *
 * Parameters: trigger - trigger structure                                    *
 *             action - action structure                                      *
 *             escalation - escalation structure. used for recovery           *
 *                          messages in {ESC.HISTORY} macro.                  *
 *                          (NULL for other cases)                            * 
 *             data - data string                                             *
 *                                                                            *
 * Return value:                                                              *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments: example: "{127.0.0.1:system[procload].last(0)}" to "1.34"        *
 *                                                                            *
 ******************************************************************************/
void	substitute_macros(DB_EVENT *event, DB_ACTION *action, DB_ESCALATION *escalation, char **data)
{
	char	
		*str_out = NULL,
		*replace_to = NULL,
		*pl = NULL,
		*pr = NULL,
		*pms = NULL,
		*pme = NULL,
		*p = NULL;
	char
		host[MAX_STRING_LEN],
		key[MAX_STRING_LEN],
		function[MAX_STRING_LEN],
		parameter[MAX_STRING_LEN];

	if (NULL == data || NULL == *data || '\0' == **data)
	{
		zabbix_log(LOG_LEVEL_DEBUG, "In substitute_macros(data:NULL)");
		return;
	}

	zabbix_log(LOG_LEVEL_DEBUG, "In substitute_macros(data:\"%s\")",
			*data);

	substitute_simple_macros(event, action, NULL, escalation, data, MACRO_TYPE_MESSAGE_SUBJECT | MACRO_TYPE_MESSAGE_BODY);

	pl = *data;
	while((pr = strchr(pl, '{')))
	{
		if((pme = strchr(pr, '}')) == NULL)
			break;

		pme[0] = '\0';
	
		pr = strrchr(pr, '{'); /* find '{' near '}' */	

		/* copy left side */
		pr[0] = '\0';
		str_out = zbx_strdcat(str_out, pl);
		pr[0] = '{';


		/* copy original name of variable */
		replace_to = zbx_dsprintf(replace_to, "%s}", pr);	/* in format used '}' */
									/* cose in 'pr' string symbol '}' is changed to '\0' by 'pme'*/
		pl = pr + strlen(replace_to);

		pms = pr + 1;
	
		if(NULL != (p = strchr(pms, ':')))
		{
			*p = '\0';
			zbx_snprintf(host, sizeof(host), "%s", pms);
			*p = ':';
			pms = p + 1;
			if(NULL != (p = strrchr(pms, '.')))
			{
				*p = '\0';
				zbx_snprintf(key, sizeof(key), "%s", pms);
				*p = '.';
				pms = p + 1;
				if(NULL != (p = strchr(pms, '(')))
				{
					*p = '\0';
					zbx_snprintf(function, sizeof(function), "%s", pms);
					*p = '(';
					pms = p + 1;
					if(NULL != (p = strchr(pms, ')')))
					{
						*p = '\0';
						zbx_snprintf(parameter, sizeof(parameter), "%s", pms);
						*p = ')';
						pms = p + 1;
						
						/* function 'evaluate_function2' require 'replace_to' with size 'MAX_STRING_LEN' */
						zbx_free(replace_to);
						replace_to = zbx_malloc(replace_to, MAX_STRING_LEN);

						if(evaluate_function2(replace_to,host,key,function,parameter) != SUCCEED)
							zbx_snprintf(replace_to, MAX_STRING_LEN, "%s", STR_UNKNOWN_VARIABLE);
					}
				}
			}
			
		}
		pme[0] = '}';

		str_out = zbx_strdcat(str_out, replace_to);
		zbx_free(replace_to);
	}
	str_out = zbx_strdcat(str_out, pl);

	zbx_free(*data);

	*data = str_out;

	zabbix_log( LOG_LEVEL_DEBUG, "End substitute_macros(result:%s)",
		*data );
}

/******************************************************************************
 *                                                                            *
 * Function: substitute_functions                                             *
 *                                                                            *
 * Purpose: substitute expression functions with theirs values                *
 *                                                                            *
 * Parameters: exp - expression string                                        *
 *             error - place error message here if any                        *
 *             maxerrlen - max length of error msg                            *
 *                                                                            *
 * Return value:  SUCCEED - evaluated succesfully, exp - updated expression   *
 *                FAIL - otherwise                                            *
 *                                                                            *
 * Author: Alexei Vladishev, Aleksander Vladishev                             *
 *                                                                            *
 * Comments: example: "({15}>10)|({123}=0)" => "(6.456>10)|(0=0)              *
 *                                                                            *
 ******************************************************************************/
static int	substitute_functions(char **exp, char *error, int maxerrlen)
{
#define ID_LEN 21
	char	functionid[ID_LEN], *f;
	char	*out = NULL, *e, *value = NULL;
	int	out_alloc = 64, out_offset = 0;
	int	level;
	char	err[MAX_STRING_LEN];

	zabbix_log(LOG_LEVEL_DEBUG, "In substitute_functions(%s)",
			*exp);

	if (**exp == '\0')
		goto empty;

	out = zbx_malloc(out, out_alloc);

	for (e = *exp; *e != '\0';)
	{
		if (*e == '{')
		{
			e++;	/* '{' */
			f = functionid;
			while (*e != '}' && *e != '\0')
			{
				if (functionid - f == ID_LEN)
					break;
				if (*e < '0' || *e > '9')
					break;

				*f++ = *e++;
			}

			if (*e != '}')
			{
				zbx_snprintf(error, maxerrlen, "Invalid expression [%s]",
						*exp);
				level = LOG_LEVEL_WARNING;
				goto error;
			}

			*f = '\0';
			e++;	/* '}' */

			if (DBget_function_result(&value, functionid, err, sizeof(err)) != SUCCEED)
			{
				zbx_snprintf(error, maxerrlen, "unable to get function value: %s",
						err);
				/* It may happen because of functions.lastvalue is NULL, so this is not warning  */
				level = LOG_LEVEL_DEBUG;
				goto error;
			}

			zbx_strcpy_alloc(&out, &out_alloc, &out_offset, value);

			zbx_free(value);
		}
		else
			zbx_chrcpy_alloc(&out, &out_alloc, &out_offset, *e++);
	}
	zbx_free(*exp);

	*exp = out;
empty:
	zabbix_log( LOG_LEVEL_DEBUG, "End substitute_functions() [%s]",
			*exp);

	return SUCCEED;
error:
	if (NULL != out)
		zbx_free(out);

	zabbix_log(level, "%s", error);
	zabbix_syslog("%s", error);

	return FAIL;
}

/******************************************************************************
 *                                                                            *
 * Function: evaluate_expression                                              *
 *                                                                            *
 * Purpose: evaluate expression                                               *
 *                                                                            *
 * Parameters: exp - expression string                                        *
 *             error - place rrror message if any                             *
 *             maxerrlen - max length of error message                        *
 *                                                                            *
 * Return value:  SUCCEED - evaluated succesfully, result - value of the exp  *
 *                FAIL - otherwise                                            *
 *                error - error message                                       *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments: example: ({a0:system[procload].last(0)}>1)|                      *
 *                    ({a0:system[procload].max(300)}>3)                      *
 *                                                                            *
 ******************************************************************************/
int	evaluate_expression(int *result,char **expression, DB_TRIGGER *trigger, char *error, int maxerrlen)
{
	/* Required for substitution of macros */
	DB_EVENT	event;
	DB_ACTION	action;

	zabbix_log(LOG_LEVEL_DEBUG, "In evaluate_expression(%s)",
		*expression);

	/* Substitute macros first */
	memset(&event,0,sizeof(DB_EVENT));	
	memset(&action,0,sizeof(DB_ACTION));	
	event.value = trigger->value;

	substitute_simple_macros(&event, &action, NULL, NULL, expression, MACRO_TYPE_TRIGGER_EXPRESSION);

	/* Evaluate expression */
	delete_spaces(*expression);
	if( substitute_functions(expression, error, maxerrlen) == SUCCEED)
	{
		if( evaluate(result, *expression, error, maxerrlen) == SUCCEED)
		{
			zabbix_log(LOG_LEVEL_DEBUG, "End evaluate_expression(result:%d)",
				*result);
			return SUCCEED;
		}
	}
	zabbix_log(LOG_LEVEL_DEBUG, "Evaluation of expression [%s] failed [%s]",
		*expression,
		error);
	zabbix_syslog("Evaluation of expression [%s] failed [%s]",
		*expression,
		error);

	zabbix_log(LOG_LEVEL_DEBUG, "End evaluate_expression(result:FAIL)");
	return FAIL;
}
