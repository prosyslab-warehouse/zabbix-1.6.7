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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>

#include <string.h>

#include <time.h>

#include <sys/socket.h>
#include <errno.h>

#include "nodecommand.h"
#include "comms.h"
#include "common.h"
#include "db.h"
#include "log.h"
#include "zlog.h"
#include "../poller/checks_ipmi.h"

/******************************************************************************
 *                                                                            *
 * Function: execute_script                                                   *
 *                                                                            *
 * Purpose: executing command                                                 *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static void	execute_script(zbx_uint64_t hostid, char *command, char **result, int *result_allocated)
{
	int		result_offset = 0;
	char		*p, buffer[MAX_STRING_LEN];
	FILE		*f;
#ifdef HAVE_OPENIPMI
	DB_RESULT	db_result;
	DB_ROW		db_row;
	DB_ITEM		item;
	int		ret, val;
#endif

	zabbix_log(LOG_LEVEL_DEBUG, "In execute_script(command:%s)", command);

	p = command;
	while (*p == ' ' && *p != '\0')
		p++;

#ifdef HAVE_OPENIPMI
	if (0 == strncmp(p, "IPMI", 4))
	{
		db_result = DBselect("select distinct host,ip,useip,port,dns,useipmi,ipmi_port,ipmi_authtype,"
				"ipmi_privilege,ipmi_username,ipmi_password from hosts where hostid=" ZBX_FS_UI64 DB_NODE,
				hostid,
				DBnode_local("hostid"));

		if (NULL != (db_row = DBfetch(db_result)))
		{
			item.host_name		= db_row[0];
			item.host_ip		= db_row[1];
			item.useip		= atoi(db_row[2]);
			item.port		= atoi(db_row[3]);
			item.host_dns		= db_row[4];

			item.useipmi		= atoi(db_row[5]);
			item.ipmi_port		= atoi(db_row[6]);
			item.ipmi_authtype	= atoi(db_row[7]);
			item.ipmi_privilege	= atoi(db_row[8]);
			item.ipmi_username	= db_row[9];
			item.ipmi_password	= db_row[10];

			if (SUCCEED == (ret = parse_ipmi_command(p, &item.ipmi_sensor, &val)))
			{
				if (SUCCEED == (ret = set_ipmi_control_value(&item, val, buffer, sizeof(buffer))))
				{
					zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
							"%d%cNODE %d: IPMI command successfully executed",
							ret,
							ZBX_DM_DELIMITER,
							CONFIG_NODEID);
				}
				else
				{
					zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
							"%d%cNODE %d: Cannot execute IPMI command [%s] error: %s",
							FAIL,
							ZBX_DM_DELIMITER,
							CONFIG_NODEID,
							command,
							buffer);
				}
			}
			else
				zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
						"%d%cNODE %d: Cannot parse IPMI command [%s]",
						FAIL,
						ZBX_DM_DELIMITER,
						CONFIG_NODEID,
						command);
		}
		else
		{
			zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
					"%d%cNODE %d: Cannot execute [%s] error: Invalid hostid",
					FAIL,
					ZBX_DM_DELIMITER,
					CONFIG_NODEID,
					command);
		}
		DBfree_result(db_result);
	}
	else
	{
#endif
		if(0 != (f = popen(p, "r"))) {
			zbx_snprintf_alloc(result, result_allocated, &result_offset, 8, "%d%c",
				SUCCEED,
				ZBX_DM_DELIMITER);

			while (NULL != fgets(buffer, sizeof(buffer)-1, f)) {
				zbx_snprintf_alloc(result, result_allocated, &result_offset, sizeof(buffer),
					"%s",
					buffer);
			}
			(*result)[result_offset] = '\0';

			pclose(f);
		} else {
			zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
				"%d%cNODE %d: Cannot execute [%s] error:%s",
				FAIL,
				ZBX_DM_DELIMITER,
				CONFIG_NODEID,
				command,
				strerror(errno));
		}
#ifdef HAVE_OPENIPMI
	}
#endif
}

/******************************************************************************
 *                                                                            *
 * Function: send_script                                                      *
 *                                                                            *
 * Purpose: sending command to slave node                                     *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	send_script(int nodeid, const char *data, char **result, int *result_allocated)
{
	DB_RESULT	dbresult;
	DB_ROW		dbrow;
	int		result_offset = 0;
	zbx_sock_t	sock;
	char		*answer;

	zabbix_log(LOG_LEVEL_DEBUG, "In send_script(nodeid:%d)", nodeid);

	dbresult = DBselect("select ip,port from nodes where nodeid=%d",
		nodeid);

	if (NULL != (dbrow = DBfetch(dbresult))) {
		if (SUCCEED == zbx_tcp_connect(&sock, CONFIG_SOURCE_IP, dbrow[0], atoi(dbrow[1]), ZABBIX_TRAPPER_TIMEOUT)) {
			if (FAIL == zbx_tcp_send(&sock, data)) {
				zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
					"%d%cNODE %d: Error while sending data to Node [%d] error: %s",
					FAIL,
					ZBX_DM_DELIMITER,
					CONFIG_NODEID,
					nodeid,
					zbx_tcp_strerror());
				goto exit_sock;
			}

			if (SUCCEED == zbx_tcp_recv(&sock, &answer/*, ZBX_TCP_READ_UNTIL_CLOSE*/)) {
				zbx_snprintf_alloc(result, result_allocated, &result_offset, strlen(answer)+1,
				"%s",
				answer);
			} else {
				zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
					"%d%cNODE %d: Error while receiving answer from Node [%d] error: %s",
					FAIL,
					ZBX_DM_DELIMITER,
					CONFIG_NODEID,
					nodeid,
					zbx_tcp_strerror());
				goto exit_sock;
			}
exit_sock:
			zbx_tcp_close(&sock);
		} else {
			zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
				"%d%cNODE %d: Unable to connect to Node [%d] error: %s",
				FAIL,
				ZBX_DM_DELIMITER,
				CONFIG_NODEID,
				nodeid,
				zbx_tcp_strerror());
		}
	} else {
		zbx_snprintf_alloc(result, result_allocated, &result_offset, 128,
			"%d%cNODE %d: Node [%d] is unknown",
			FAIL,
			ZBX_DM_DELIMITER,
			CONFIG_NODEID,
			nodeid);
	}
	DBfree_result(dbresult);
}

/******************************************************************************
 *                                                                            *
 * Function: get_next_point_to_node                                           *
 *                                                                            *
 * Purpose: find next point to slave node                                     *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
int	get_next_point_to_node(int current_nodeid, int slave_nodeid, int *nodeid)
{
	DB_RESULT	dbresult;
	DB_ROW		dbrow;
	int		id, res = FAIL;

	dbresult = DBselect("select nodeid from nodes where masterid=%d",
		current_nodeid);

	while (NULL != (dbrow = DBfetch(dbresult))) {
		id = atoi(dbrow[0]);
		if (id == slave_nodeid || SUCCEED == get_next_point_to_node(id, slave_nodeid, NULL)) {
			if (NULL != nodeid)
				*nodeid = id;
			res = SUCCEED;
			break;
		}
	}
	DBfree_result(dbresult);

	return res;
}

/******************************************************************************
 *                                                                            *
 * Function: node_process_command                                             *
 *                                                                            *
 * Purpose: process command received from a master node or php                *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
int	node_process_command(zbx_sock_t *sock, const char *data)
{
	const char	*r;
	char		*tmp = NULL, *result = NULL;
	int		tmp_allocated = 64, result_allocated = 1024;
	int		datalen;
	int		nodeid, next_nodeid;
	int		result_offset = 0;
	zbx_uint64_t	hostid;

	result = zbx_malloc(result, result_allocated);
	tmp = zbx_malloc(tmp, tmp_allocated);
	datalen = strlen(data);

	zabbix_log(LOG_LEVEL_DEBUG, "In node_process_command(datalen:%d)",
		datalen);

	r = data;
	zbx_get_next_field(&r, &tmp, &tmp_allocated, ZBX_DM_DELIMITER); /* Constant 'Command' */
	zbx_get_next_field(&r, &tmp, &tmp_allocated, ZBX_DM_DELIMITER); /* NodeID */
	nodeid = atoi(tmp);
	zbx_get_next_field(&r, &tmp, &tmp_allocated, ZBX_DM_DELIMITER); /* hostid */
	ZBX_STR2UINT64(hostid, tmp);
	zbx_get_next_field(&r, &tmp, &tmp_allocated, ZBX_DM_DELIMITER);

	if (nodeid == CONFIG_NODEID) {
		zabbix_log(LOG_LEVEL_WARNING, "NODE %d: Received command \"%s\"",
			CONFIG_NODEID,
			tmp);

		execute_script(hostid, tmp, &result, &result_allocated);
	} else if (SUCCEED == get_next_point_to_node(CONFIG_NODEID, nodeid, &next_nodeid)) {
		zabbix_log(LOG_LEVEL_WARNING, "NODE %d: Sending command \"%s\" for nodeid %d"
			"to node %d",
			CONFIG_NODEID,
			tmp,
			nodeid,
			next_nodeid);

		send_script(next_nodeid, data, &result, &result_allocated);
	} else {
		zbx_snprintf_alloc(&result, &result_allocated, &result_offset, 128,
			"%d%cNODE %d: Node [%d] is unknown",
			FAIL,
			ZBX_DM_DELIMITER,
			CONFIG_NODEID,
			nodeid);
	}

	alarm(CONFIG_TIMEOUT);
	if (zbx_tcp_send_raw(sock, result) != SUCCEED) {
		zabbix_log(LOG_LEVEL_WARNING, "NODE %d: Error sending result of command to node %d",
			CONFIG_NODEID,
			nodeid);
	}
	alarm(0);

	zbx_free(tmp);
	zbx_free(result);

	return SUCCEED;
}
