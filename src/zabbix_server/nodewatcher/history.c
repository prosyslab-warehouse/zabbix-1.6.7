/* 
** ZABBIX
** Copyright (C) 2000-2006 SIA Zabbix
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

#include "common.h"

#include "cfg.h"
#include "db.h"
#include "log.h"
#include "zlog.h"

#include "history.h"
#include "nodewatcher.h"
#include "nodecomms.h"

/******************************************************************************
 *                                                                            *
 * Function: get_history_lastid:                                              *
 *                                                                            *
 * Purpose: get last history id from master node                              *
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
static int get_history_lastid(int master_nodeid, int nodeid, ZBX_TABLE *table, zbx_uint64_t *lastid)
{
	zbx_sock_t	sock;
	char		data[MAX_STRING_LEN], *answer;
	int		res = FAIL;

	zabbix_log(LOG_LEVEL_DEBUG, "In get_history_lastid()");

	if (SUCCEED == connect_to_node(master_nodeid, &sock)) {
		zbx_snprintf(data, sizeof(data), "ZBX_GET_HISTORY_LAST_ID%c%d%c%d\n%s%c%s",
			ZBX_DM_DELIMITER, CONFIG_NODEID,
			ZBX_DM_DELIMITER, nodeid,
			table->table, ZBX_DM_DELIMITER, table->recid);

		if (FAIL == send_data_to_node(master_nodeid, &sock, data))
			goto disconnect;

		if (FAIL == recv_data_from_node(master_nodeid, &sock, &answer))
			goto disconnect;

		if (0 == strncmp(answer, "FAIL", 4)) {
			zabbix_log( LOG_LEVEL_ERR, "NODE %d: get_history_lastid() FAIL from node %d for node %d",
				CONFIG_NODEID,
				master_nodeid,
				nodeid);
			goto disconnect;
		}

		ZBX_STR2UINT64(*lastid, answer);
		res = SUCCEED;
disconnect:
		disconnect_node(&sock);
	}
	return res;
}

/******************************************************************************
 *                                                                            *
 * Function : process_hstory_table_data:                                      *
 *                                                                            *
 * Purpose: process new history data                                          *
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
void	process_history_table_data(ZBX_TABLE *table, int master_nodeid, int nodeid)
{
	DB_RESULT	result;
	DB_ROW		row;
	char		*data = NULL, *tmp = NULL;
	int		data_allocated = 1024*1024, tmp_allocated = 4096, tmp_offset, data_offset, f, fld, len;
	int		data_found = 0;
	zbx_uint64_t	lastid;

	zabbix_log( LOG_LEVEL_DEBUG, "In process_history_table_data()");

	DBbegin();

	if ((table->flags & ZBX_HISTORY) && FAIL == get_history_lastid(master_nodeid, nodeid, table, &lastid))
		return;

	data = zbx_malloc(data, data_allocated);
	tmp = zbx_malloc(tmp, tmp_allocated);

	data_offset = 0;
	zbx_snprintf_alloc(&data, &data_allocated, &data_offset, 128, "History%c%d%c%d%c%s",
		ZBX_DM_DELIMITER, CONFIG_NODEID,
		ZBX_DM_DELIMITER, nodeid,
		ZBX_DM_DELIMITER, table->table);

	/* Do not send history for current node if CONFIG_NODE_NOHISTORY is set */
/*	if ((CONFIG_NODE_NOHISTORY != 0) && (CONFIG_NODEID == nodeid))
		goto exit;*/

	tmp_offset = 0;
	if (table->flags & ZBX_HISTORY_SYNC) {
		zbx_snprintf_alloc(&tmp, &tmp_allocated, &tmp_offset, 128, "select %s,",
			table->recid);
	} else { /* ZBX_HISTORY */
		zbx_snprintf_alloc(&tmp, &tmp_allocated, &tmp_offset, 16, "select ");
	}

	for (f = 0; table->fields[f].name != 0; f++) {
		if ((table->flags & ZBX_HISTORY_SYNC) && 0 == (table->fields[f].flags & ZBX_HISTORY_SYNC))
			continue;

		zbx_snprintf_alloc(&tmp, &tmp_allocated, &tmp_offset, 128, "%s,",
			table->fields[f].name);
	}
	tmp_offset--;

	if (table->flags & ZBX_HISTORY_SYNC) {
		zbx_snprintf_alloc(&tmp, &tmp_allocated, &tmp_offset, 1024, " from %s where nodeid=%d order by %s",
			table->table,
			nodeid,
			table->recid);
	} else { /* ZBX_HISTORY */
		zbx_snprintf_alloc(&tmp, &tmp_allocated, &tmp_offset, 1024, " from %s where %s>"ZBX_FS_UI64
			DB_NODE " order by %s",
			table->table,
			table->recid,
			lastid,
			DBnode(table->recid, nodeid),
			table->recid);
	}

	result = DBselectN(tmp, 10000);
	while (NULL != (row = DBfetch(result))) {
		if (table->flags & ZBX_HISTORY_SYNC) {
			ZBX_STR2UINT64(lastid, row[0]);
			fld = 1;
		} else
			fld = 0;

		zbx_snprintf_alloc(&data, &data_allocated, &data_offset, 128, "\n");

		for (f = 0; table->fields[f].name != 0; f++) {
			if ((table->flags & ZBX_HISTORY_SYNC) && 0 == (table->fields[f].flags & ZBX_HISTORY_SYNC))
				continue;

			len = (int)strlen(row[fld]);

			if (table->fields[f].type == ZBX_TYPE_INT ||
				table->fields[f].type == ZBX_TYPE_UINT ||
				table->fields[f].type == ZBX_TYPE_ID ||
				table->fields[f].type == ZBX_TYPE_FLOAT)
			{
				zbx_snprintf_alloc(&data, &data_allocated, &data_offset, 128, "%s%c",
					row[fld], ZBX_DM_DELIMITER);
			} else { /* ZBX_TYPE_CHAR ZBX_TYPE_BLOB ZBX_TYPE_TEXT */
				len = zbx_binary2hex((u_char *)row[fld], len, &tmp, &tmp_allocated);
				zbx_snprintf_alloc(&data, &data_allocated, &data_offset, len + 8, "%s%c",
					tmp, ZBX_DM_DELIMITER);
			}
			fld++;
		}
		data_offset--;
		data_found = 1;
	}
	DBfree_result(result);

	data[data_offset] = '\0';

	if (1 == data_found && SUCCEED == send_to_node(table->table, master_nodeid, nodeid, data)) {
		if (table->flags & ZBX_HISTORY_SYNC) {
			DBexecute("delete from %s where nodeid=%d and %s<="ZBX_FS_UI64,
				table->table,
				nodeid,
				table->recid,
				lastid);
		}
	}

	DBcommit();

	zbx_free(tmp);
	zbx_free(data);
}

/******************************************************************************
 *                                                                            *
 * Function: process_history_tables                                           *
 *                                                                            *
 * Purpose: process new history data from tables with ZBX_HISTORY* flags      *
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
static void process_history_tables(int master_nodeid, int nodeid)
{
	int	t;
	int	start = time(NULL);

	zabbix_log(LOG_LEVEL_DEBUG, "In process_history_tables()");

	for (t = 0; tables[t].table != 0; t++) {
		if (tables[t].flags & (ZBX_HISTORY | ZBX_HISTORY_SYNC))
			process_history_table_data(&tables[t], master_nodeid, nodeid);
	}

	zabbix_log(LOG_LEVEL_DEBUG, "NODE %d: Spent %d seconds for node %d in process_history_tables",
		CONFIG_NODEID,
		time(NULL) - start,
		nodeid);
}
/******************************************************************************
 *                                                                            *
 * Function: main_historysender                                               *
 *                                                                            *
 * Purpose: periodically sends historical data to master node                 *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value:                                                              * 
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void main_historysender()
{
	DB_RESULT	result;
	DB_ROW		row;
	int		master_nodeid, nodeid;

	zabbix_log(LOG_LEVEL_DEBUG, "In main_historysender()");

	master_nodeid = CONFIG_MASTER_NODEID;
	if (0 == master_nodeid)
		return;

	result = DBselect("select nodeid from nodes");
	while ((row = DBfetch(result))) {
		nodeid = atoi(row[0]);
		if (SUCCEED == is_master_node(CONFIG_NODEID, nodeid))
			continue;

		process_history_tables(master_nodeid, nodeid);
	}
	DBfree_result(result);
}
