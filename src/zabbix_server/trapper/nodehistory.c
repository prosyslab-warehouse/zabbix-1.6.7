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

#include "nodehistory.h"
#include "common.h"
#include "db.h"
#include "log.h"

#include "../events.h"
#include "../nodewatcher/nodecomms.h"

static char	*buffer = NULL, *tmp = NULL;
static int	buffer_allocated, tmp_allocated;

/******************************************************************************
 *                                                                            *
 * Function: send_history_last_id                                             *
 *                                                                            *
 * Purpose: send list of last historical tables ids                           *
 *                                                                            *
 * Parameters: sock - opened socket of node-node connection                   *
 *             record                                                         *
 *                                                                            *
 * Return value:  SUCCEED - sent succesfully                                  *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Aleksander Vladishev                                               *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
int	send_history_last_id(zbx_sock_t *sock, const char *data)
{
	DB_RESULT	result;
	DB_ROW		row;
	const char	*r;
	const ZBX_TABLE	*table;
	const ZBX_FIELD *field;
	int		buffer_offset;
	int		sender_nodeid, nodeid, res;

	zabbix_log(LOG_LEVEL_DEBUG, "In send_list_of_history_ids()");

	buffer_allocated = 320;
	buffer = zbx_malloc(buffer, buffer_allocated);

	r = data;
	if (NULL == r)
		goto error;

	zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* constant 'ZBX_GET_HISTORY_LAST_ID' */
	if (NULL == r)
		goto error;

	zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* sender_nodeid */
	sender_nodeid = atoi(buffer);
	if (NULL == r)
		goto error;

	zbx_get_next_field(&r, &buffer, &buffer_allocated, '\n'); /* nodeid */
	nodeid = atoi(buffer);
	if (NULL == r)
		goto error;

	zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* table name */
	if (NULL == (table = DBget_table(buffer)))
		goto error;

	if (NULL == r)
		goto error;

	zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* field name */
	if (NULL == (field = DBget_field(table, buffer)))
		goto error;

	buffer_offset= 0;
	zbx_snprintf_alloc(&buffer, &buffer_allocated, &buffer_offset, 320,
			"select max(%s)"
			" from %s"
			" where 1=1" DB_NODE,
			field->name,
			table->table,
			DBnode(field->name, nodeid));

	buffer_offset= 0;
	result = DBselect("%s", buffer);
	if (NULL != (row = DBfetch(result)))
		zbx_snprintf_alloc(&buffer, &buffer_allocated, &buffer_offset, 32, "%s",
				SUCCEED == DBis_null(row[0]) ? "0" : row[0]);
	DBfree_result(result);

	if (buffer_offset == 0)
		goto error;

	alarm(CONFIG_TIMEOUT);
	res = send_data_to_node(sender_nodeid, sock, buffer);
	alarm(0);

	zbx_free(buffer);

	return res;
error:
	buffer_offset= 0;
	zbx_snprintf_alloc(&buffer, &buffer_allocated, &buffer_offset, 8, "FAIL");

	alarm(CONFIG_TIMEOUT);
	res = send_data_to_node(sender_nodeid, sock, buffer);
	alarm(0);

	zbx_free(buffer);

	zabbix_log( LOG_LEVEL_ERR, "NODE %d: Received invalid record from node %d for node %d [%s]",
		CONFIG_NODEID,
		sender_nodeid,
		nodeid,
		data);

	return FAIL;
}

/******************************************************************************
 *                                                                            *
 * Function: process_record_event                                             *
 *                                                                            *
 * Purpose: process record update                                             *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static int	process_record_event(int sender_nodeid, int nodeid, const ZBX_TABLE *table, const char *record)
{
	const char	*r;
	int		f, len;
	DB_EVENT	event;

	zabbix_log(LOG_LEVEL_DEBUG, "In process_record_event()");

	memset(&event, 0, sizeof(event));

	r = record;
	for (f = 0; table->fields[f].name != 0; f++) {
		if (NULL == r)
			goto error;

		len = zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER);

		if (0 == strcmp(table->fields[f].name, "eventid")) {
			ZBX_STR2UINT64(event.eventid, buffer);
		} else if (0 == strcmp(table->fields[f].name, "source")) {
			event.source = atoi(buffer);
		} else if (0 == strcmp(table->fields[f].name, "object")) {
			event.object = atoi(buffer);
		} else if (0 == strcmp(table->fields[f].name, "objectid")) {
			ZBX_STR2UINT64(event.objectid, buffer);
		} else if (0 == strcmp(table->fields[f].name, "clock")) {
			event.clock=atoi(buffer);
		} else if (0 == strcmp(table->fields[f].name, "value")) {
			event.value=atoi(buffer);
		} else if (0 == strcmp(table->fields[f].name, "acknowledged")) {
			event.acknowledged=atoi(buffer);
		}
	}

	return process_event(&event);
error:
	zabbix_log( LOG_LEVEL_ERR, "NODE %d: Received invalid record from node %d for node %d [%s]",
		CONFIG_NODEID,
		sender_nodeid,
		nodeid,
		record);

	return FAIL;
}

static void	begin_history_sql(char **sql, int *sql_allocated, int *sql_offset, const ZBX_TABLE *table)
{
	int	f;

	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, ZBX_TABLENAME_LEN + 16, "insert into %s (",
			table->table);

	if (0 != (table->flags & ZBX_HISTORY_SYNC))
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 8, "nodeid,");

	for (f = 0; table->fields[f].name != 0; f++) {
		if (0 != (table->flags & ZBX_HISTORY_SYNC) && 0 == (table->fields[f].flags & ZBX_HISTORY_SYNC))
			continue;

		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, ZBX_FIELDNAME_LEN + 2, "%s,",
				table->fields[f].name);
	}

	(*sql_offset)--;
	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 10, ") values ");
}

/******************************************************************************
 *                                                                            *
 * Function: process_record                                                   *
 *                                                                            *
 * Purpose: process record update                                             *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static int	process_record(char **sql, int *sql_allocated, int *sql_offset, int sender_nodeid, int nodeid, const ZBX_TABLE *table,
		const char *record, int lastrecord)
{
	const char	*r;
	int		f, len;
	int		res = FAIL;
	char		*value_esc;

	zabbix_log(LOG_LEVEL_DEBUG, "In process_record()");

	if (*sql_offset == 0)
	{
#ifdef HAVE_ORACLE
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 8, "begin\n");
#endif

#ifdef HAVE_MYSQL
		begin_history_sql(sql, sql_allocated, sql_offset, table);
#endif
	}

#ifndef HAVE_MYSQL
	begin_history_sql(sql, sql_allocated, sql_offset, table);
#endif

	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 2, "(");
	if (0 != (table->flags & ZBX_HISTORY_SYNC))
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 16, "%d,",
				nodeid);

	for (r = record, f = 0; table->fields[f].name != 0; f++) {
		if (0 != (table->flags & ZBX_HISTORY_SYNC) && 0 == (table->fields[f].flags & ZBX_HISTORY_SYNC))
			continue;

		if (NULL == r)
			goto error;

		len = zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER);

		if (table->fields[f].type == ZBX_TYPE_INT ||
				table->fields[f].type == ZBX_TYPE_UINT ||
				table->fields[f].type == ZBX_TYPE_ID ||
				table->fields[f].type == ZBX_TYPE_FLOAT)
		{
			zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 2, "%s,",
					buffer);
		}
		else if (table->fields[f].type == ZBX_TYPE_BLOB)
		{
#ifdef HAVE_POSTGRESQL
			len = zbx_hex2binary(buffer);
			len = zbx_pg_escape_bytea((u_char *)buffer, len, &tmp, &tmp_allocated);

			zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 4, "'%s',");
#else
			if ('\0' == *buffer)
				zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 4, "'',");
			else
				zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 4, "0x%s,",
						buffer);
#endif
		}
		else	/* ZBX_TYPE_TEXT, ZBX_TYPE_CHAR */
		{
			zbx_hex2binary(buffer);
			value_esc = DBdyn_escape_string(buffer);
			len = strlen(value_esc);

			zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 4, "'%s',",
					value_esc);

			zbx_free(value_esc);
		}
	}

	(*sql_offset)--;

#ifdef HAVE_MYSQL
	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 3, "),");
#else
	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 4, ");\n");
#endif

	if (lastrecord || *sql_offset > ZBX_MAX_SQL_SIZE)
	{
#ifdef HAVE_ORACLE
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 8, "end;\n");
#endif

#ifdef HAVE_MYSQL
		(*sql_offset)--;
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 4, ";\n");
#endif
		if (DBexecute("%s", *sql) >= ZBX_DB_OK)
			res = SUCCEED;
		(*sql_offset) = 0;
	}
	else
		res = SUCCEED;

	return res;
error:
	zabbix_log( LOG_LEVEL_ERR, "NODE %d: Received invalid record from node %d for node %d [%s]",
		CONFIG_NODEID,
		sender_nodeid,
		nodeid,
		record);

	return FAIL;
}

/******************************************************************************
 *                                                                            *
 * Function: update_items                                                     *
 *                                                                            *
 * Purpose: process record update                                             *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author:                                                                    *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static int	process_items(char **sql, int *sql_allocated, int *sql_offset, int sender_nodeid, int nodeid, const ZBX_TABLE *table,
		const char *record, int lastrecord)
{
	const char	*r;
	int		f, len, res = FAIL;
	zbx_uint64_t	itemid = 0;
	char		*value_esc;
	int		clock, value_type = -1;
	double		value_double;
	zbx_uint64_t	value_uint64;

	zabbix_log(LOG_LEVEL_DEBUG, "In process_items()");

	if (*sql_offset == 0)
	{
#ifdef HAVE_ORACLE
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 8, "begin\n");
#endif
	}

	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 40, "update items set prevvalue=lastvalue");

	for (r = record, f = 0; table->fields[f].name != 0; f++) {
		if (0 != (table->flags & ZBX_HISTORY_SYNC) && 0 == (table->fields[f].flags & ZBX_HISTORY_SYNC))
			continue;

		if (NULL == r)
			goto error;

		len = zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER);

		if (0 == strcmp(table->fields[f].name, "itemid"))
			ZBX_STR2UINT64(itemid, buffer)

		if (table->fields[f].type == ZBX_TYPE_INT ||
				table->fields[f].type == ZBX_TYPE_UINT ||
				table->fields[f].type == ZBX_TYPE_ID ||
				table->fields[f].type == ZBX_TYPE_FLOAT)
		{
			if (0 == strcmp(table->fields[f].name, "clock"))
			{
				zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 16, ",lastclock=%s",
						buffer);
				clock = atoi(buffer);
			}
			else if (0 == strcmp(table->fields[f].name, "value"))
			{
				zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 16, ",lastvalue=%s",
						buffer);

				value_type = table->fields[f].type;
				if (value_type == ZBX_TYPE_FLOAT)
					value_double = atof(buffer);
				else if (value_type == ZBX_TYPE_UINT)
					ZBX_STR2UINT64(value_uint64, buffer)
			}
		}
		else	/* ZBX_TYPE_TEXT, ZBX_TYPE_CHAR */
		{
			if (0 == strcmp(table->fields[f].name, "value"))
			{
				zbx_hex2binary(buffer);
				value_esc = DBdyn_escape_string_len(buffer, ITEM_LASTVALUE_LEN);
				len = strlen(value_esc);

				zbx_snprintf_alloc(sql, sql_allocated, sql_offset, len + 16, ",lastvalue='%s'",
						value_esc);

				zbx_free(value_esc);
			}
		}
	}

	if (value_type == ZBX_TYPE_FLOAT)
		DBadd_trend(itemid, value_double, clock);
	else if (value_type == ZBX_TYPE_UINT)
		DBadd_trend_uint(itemid, value_double, clock);

	zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 40, " where itemid=" ZBX_FS_UI64 ";\n",
			itemid);

	if (lastrecord || *sql_offset > ZBX_MAX_SQL_SIZE)
	{
#ifdef HAVE_ORACLE
		zbx_snprintf_alloc(sql, sql_allocated, sql_offset, 8, "end;\n");
#endif
		if (DBexecute("%s", *sql) >= ZBX_DB_OK)
			res = SUCCEED;
		(*sql_offset) = 0;
	}
	else
		res = SUCCEED;

	return res;
error:
	zabbix_log( LOG_LEVEL_ERR, "NODE %d: Received invalid record from node %d for node %d [%s]",
		CONFIG_NODEID,
		sender_nodeid,
		nodeid,
		record);

	return FAIL;
}

/******************************************************************************
 *                                                                            *
 * Function: node_history                                                     *
 *                                                                            *
 * Purpose: process new history received from a salve node                    *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:  SUCCEED - processed successfully                            *
 *                FAIL - an error occured                                     *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
int	node_history(char *data, size_t datalen)
{
	const char	*r;
	char		*newline = NULL;
	char		*pos;
	int		sender_nodeid = 0, nodeid = 0, firstline = 1, events = 0, history = 0;
	const ZBX_TABLE	*table_sync = NULL, *table = NULL;
	int		res = SUCCEED;

	char		*sql1 = NULL, *sql2 = NULL, *sql3 = NULL;
	int		sql1_allocated, sql2_allocated, sql3_allocated;
	int		sql1_offset, sql2_offset, sql3_offset;

	assert(data);

/*	zabbix_set_log_level(LOG_LEVEL_DEBUG);*/
	zabbix_log(LOG_LEVEL_DEBUG, "In node_history()");

	buffer_allocated = 4096;
	sql1_allocated = 32768;
	sql2_allocated = 32768;
	sql3_allocated = 32768;
	tmp_allocated = 4096;

	buffer = zbx_malloc(buffer, buffer_allocated);
	sql1 = zbx_malloc(sql1, sql1_allocated);
	sql2 = zbx_malloc(sql2, sql2_allocated);
	sql3 = zbx_malloc(sql3, sql3_allocated);
	tmp = zbx_malloc(tmp, tmp_allocated);

	DBbegin();

	for (r = data; *r != '\0' && res == SUCCEED;) {
		if (NULL != (newline = strchr(r, '\n')))
			*newline = '\0';

		if (1 == firstline) {
			zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* constant 'History' */
			zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* sender_nodeid */
			sender_nodeid=atoi(buffer);
			zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* nodeid */
			nodeid=atoi(buffer);
			zbx_get_next_field(&r, &buffer, &buffer_allocated, ZBX_DM_DELIMITER); /* tablename */

			table = DBget_table(buffer);
			if (0 == (table->flags & (ZBX_HISTORY | ZBX_HISTORY_SYNC)))
				table = NULL;

			if (NULL != table && 0 != (table->flags & ZBX_HISTORY_SYNC)) {
				table_sync = table;
				if (NULL != (pos = strstr(buffer, "_sync"))) {
					*pos = '\0';
					table = DBget_table(buffer);
				}
			}

			if (NULL != table && 0 == strcmp(table->table, "events"))
				events = 1;

			if (NULL != table && 0 == strncmp(table->table, "history", 7))
				history = 1;

			if (NULL == table) {
				zabbix_log(LOG_LEVEL_ERR, "NODE %d: Invalid received data: unknown tablename \"%s\"",
					CONFIG_NODEID,
					buffer);
			}
			if (NULL != newline) {
				zabbix_log(LOG_LEVEL_WARNING, "NODE %d: Received %s from node %d for node %d datalen %d",
					CONFIG_NODEID,
					buffer,
					sender_nodeid,
					nodeid,
					(int)datalen);
			}
			firstline = 0;
			sql1_offset = 0;
			sql2_offset = 0;
			sql3_offset = 0;
		} else if (NULL != table) {
			if (events) {
				res = process_record_event(sender_nodeid, nodeid, table, r);
			} else {
				res = process_record(&sql1, &sql1_allocated, &sql1_offset, sender_nodeid, nodeid, table, r, newline ? 0 : 1);
				if (SUCCEED == res && 0 != history)
					res = process_items(&sql2, &sql2_allocated, &sql2_offset, sender_nodeid, nodeid, table, r, newline ? 0 : 1);
				if (SUCCEED == res && NULL != table_sync && 0 != CONFIG_MASTER_NODEID)
					res = process_record(&sql3, &sql3_allocated, &sql3_offset, sender_nodeid, nodeid, table_sync, r, newline ? 0 : 1);
			}
		}

		if (newline != NULL) {
			*newline = '\n';
			r = newline + 1;
		} else
			break;
	}
	if (res == SUCCEED)
		DBcommit();
	else
		DBrollback();

	zbx_free(tmp);
	zbx_free(sql1);
	zbx_free(sql2);
	zbx_free(sql3);
	zbx_free(buffer);

/*	zabbix_set_log_level(LOG_LEVEL_WARNING);*/
	return res;
}
