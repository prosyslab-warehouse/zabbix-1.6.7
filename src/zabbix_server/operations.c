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

#include <signal.h>

#include <string.h>

#include <time.h>

#include "common.h"
#include "comms.h"
#include "db.h"
#include "log.h"
#include "zlog.h"

#include "poller/poller.h"
#include "poller/checks_agent.h"
#include "poller/checks_ipmi.h"

/******************************************************************************
 *                                                                            *
 * Function: send_to_user_medias                                              *
 *                                                                            *
 * Purpose: send notifications to user's medias (email, sms, whatever)        *
 *                                                                            *
 * Parameters: trigger - trigger data                                         *
 *             action  - action data                                          *
 *             userid  - user id                                              *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments: Cannot use action->userid as it may also be groupid              *
 *                                                                            *
 ******************************************************************************/
/*static	void	send_to_user_medias(DB_EVENT *event,DB_OPERATION *operation, zbx_uint64_t userid)
{
	DB_MEDIA media;
	DB_RESULT result;
	DB_ROW	row;

	zabbix_log( LOG_LEVEL_DEBUG, "In send_to_user_medias(objectid:" ZBX_FS_UI64 ")",
		event->objectid);

	result = DBselect("select mediatypeid,sendto,active,severity,period from media where active=%d and userid=" ZBX_FS_UI64,
		MEDIA_STATUS_ACTIVE,
		userid);

	while((row=DBfetch(result)))
	{
		ZBX_STR2UINT64(media.mediatypeid, row[0]);

		media.sendto	= row[1];
		media.active	= atoi(row[2]);
		media.severity	= atoi(row[3]);
		media.period	= row[4];

		zabbix_log( LOG_LEVEL_DEBUG, "Trigger severity [%d] Media severity [%d] Period [%s]",
			event->trigger_priority,
			media.severity,
			media.period);
		if(((1<<event->trigger_priority)&media.severity)==0)
		{
			zabbix_log( LOG_LEVEL_DEBUG, "Won't send message (severity)");
			continue;
		}
		if(check_time_period(media.period, (time_t)NULL) == 0)
		{
			zabbix_log( LOG_LEVEL_DEBUG, "Won't send message (period)");
			continue;
		}

		DBadd_alert(operation->actionid, userid, event->eventid, media.mediatypeid,media.sendto,operation->shortdata,operation->longdata);
	}
	DBfree_result(result);

	zabbix_log(LOG_LEVEL_DEBUG, "End send_to_user_medias()");
}
*/
/******************************************************************************
 *                                                                            *
 * Function: check_user_active                                                *
 *                                                                            *
 * Purpose: checks if user in any users_disabled group                        *
 *                                                                            *
 * Parameters: userid - user id                                               *
 *                                                                            *
 * Return value: int SUCCEED / FAIL-if user disabled                          *
 *                                                                            *
 * Author: Aly                                                                *
 *                                                                            *
 * Comments:                    			                      *
 *                                                                            *
 ******************************************************************************/
/*int check_user_active(zbx_uint64_t userid){
	DB_RESULT	result;
	DB_ROW		row;
	int		rtrn = SUCCEED;

	result = DBselect("SELECT COUNT(g.usrgrpid) FROM users_groups ug, usrgrp g WHERE ug.userid=" ZBX_FS_UI64 " AND g.usrgrpid=ug.usrgrpid AND g.users_status=%d", userid, GROUP_STATUS_DISABLED);
	
	row = DBfetch(result);
	if(row && (DBis_null(row[0])!=SUCCEED) && (atoi(row[0])>0)) 
		rtrn=FAIL;

	DBfree_result(result);

return rtrn;
}
*/
/******************************************************************************
 *                                                                            *
 * Function: op_notify_user                                                   *
 *                                                                            *
 * Purpose: send notifications to user or user groupd                         *
 *                                                                            *
 * Parameters: trigger - trigger data                                         *
 *             action  - action data                                          *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments: action->recipient specifies user or group                        *
 *                                                                            *
 ******************************************************************************/
/*void	op_notify_user(DB_EVENT *event, DB_ACTION *action, DB_OPERATION *operation)
{
	DB_RESULT	result;
	DB_ROW		row;
	zbx_uint64_t	userid;

	zabbix_log(LOG_LEVEL_DEBUG, "In send_to_user()");

	if(operation->object == OPERATION_OBJECT_USER)
	{
		if(check_user_active(operation->objectid) == SUCCEED)
		{
			send_to_user_medias(event, operation, operation->objectid);
		}
	}
	else if(operation->object == OPERATION_OBJECT_GROUP)
	{
		result = DBselect("select u.userid from users u, users_groups ug, usrgrp g where ug.usrgrpid=" ZBX_FS_UI64 " and ug.userid=u.userid and g.usrgrpid=ug.usrgrpid and g.users_status=%d",
			operation->objectid,GROUP_STATUS_ACTIVE);
		while((row=DBfetch(result)))
		{
			ZBX_STR2UINT64(userid, row[0]);
			send_to_user_medias(event, operation, userid);
		}
		DBfree_result(result);
	}
	else
	{
		zabbix_log( LOG_LEVEL_WARNING, "Unknown object type [%d] for operationid [" ZBX_FS_UI64 "]",
			operation->object,
			operation->operationid);
		zabbix_syslog("Unknown object type [%d] for operationid [" ZBX_FS_UI64 "]",
			operation->object,
			operation->operationid);
	}
	zabbix_log(LOG_LEVEL_DEBUG, "End send_to_user()");
}
*/

/******************************************************************************
 *                                                                            *
 * Function: run_remote_commands                                              *
 *                                                                            *
 * Purpose: run remote command on specific host                               *
 *                                                                            *
 * Parameters: host_name - host name                                          *
 *             command - remote command                                       *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/

static void run_remote_command(char* host_name, char* command)
{
	int		ret = 9;
	AGENT_RESULT	agent_result;
	DB_ITEM         item;
	DB_RESULT	result;
	DB_ROW		row;
	char		*p, *host_esc, key[ITEM_KEY_LEN_MAX];
#ifdef HAVE_OPENIPMI
	int		val;
	char		error[MAX_STRING_LEN];
#endif

	assert(host_name);
	assert(command);

	zabbix_log(LOG_LEVEL_DEBUG, "In run_remote_command(hostname:%s,command:%s)",
		host_name,
		command);

	host_esc = DBdyn_escape_string(host_name);
	result = DBselect("select distinct host,ip,useip,port,dns,useipmi,ipmi_port,ipmi_authtype,"
			"ipmi_privilege,ipmi_username,ipmi_password from hosts where host='%s'" DB_NODE,
			host_esc,
			DBnode_local("hostid"));
	zbx_free(host_esc);

	if (NULL != (row = DBfetch(result)))
	{
		*key			= '\0';
		item.key		= key;
		item.host_name		= row[0];
		item.host_ip		= row[1];
		item.useip		= atoi(row[2]);
		item.port		= atoi(row[3]);
		item.host_dns		= row[4];

		item.useipmi		= atoi(row[5]);
		item.ipmi_port		= atoi(row[6]);
		item.ipmi_authtype	= atoi(row[7]);
		item.ipmi_privilege	= atoi(row[8]);
		item.ipmi_username	= row[9];
		item.ipmi_password	= row[10];

		p = command;
		while (*p == ' ' && *p != '\0')
			p++;

#ifdef HAVE_OPENIPMI
		if (0 == strncmp(p, "IPMI", 4))
		{
			if (SUCCEED == (ret = parse_ipmi_command(p, &item.ipmi_sensor, &val)))
				ret = set_ipmi_control_value(&item, val, error, sizeof(error));
		}
		else
		{
#endif
			zbx_snprintf(key, sizeof(key), "system.run[%s,nowait]", p);

			alarm(CONFIG_TIMEOUT);

			ret = get_value_agent(&item, &agent_result);

			free_result(&agent_result);
	
			alarm(0);
#ifdef HAVE_OPENIPMI
		}
#endif
	}
	DBfree_result(result);
	
	zabbix_log(LOG_LEVEL_DEBUG, "End run_remote_command(result:%d)",
		ret);
}

/******************************************************************************
 *                                                                            *
 * Function: get_next_command                                                 *
 *                                                                            *
 * Purpose: parse action script on remote commands                            *
 *                                                                            *
 * Parameters: command_list - command list                                    *
 *             alias - (output) of host name or group name                    *
 *             is_group - (output) 0 if alias is a host name                  *
 *                               1 if alias is a group name                   *
 *             command - (output) remote command                              *
 *                                                                            *
 * Return value: 0 - correct comand is readed                                 *
 *               1 - EOL                                                      *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/

#define CMD_ALIAS 0
#define CMD_REM_COMMAND 1

static int get_next_command(char** command_list, char** alias, int* is_group, char** command)
{
	int state = CMD_ALIAS;
	int len = 0;
	int i = 0;
	
	assert(alias);
	assert(is_group);
	assert(command);

	zabbix_log(LOG_LEVEL_DEBUG, "In get_next_command(command_list:%s)",
		*command_list);

	*alias = NULL;
	*is_group = 0;
	*command = NULL;
	

	if((*command_list)[0] == '\0' || (*command_list)==NULL) {
		zabbix_log(LOG_LEVEL_DEBUG, "Result get_next_command [EOL]");
		return 1;
	}

	*alias = *command_list;
	len = strlen(*command_list);

	for(i=0; i < len; i++)
	{
		if(state == CMD_ALIAS)
		{
			if((*command_list)[i] == '#'){
				*is_group = 1;
				(*command_list)[i] = '\0';
				state = CMD_REM_COMMAND;
				*command = &(*command_list)[i+1];
			}else if((*command_list)[i] == ':'){
				*is_group = 0;
				(*command_list)[i] = '\0';
				state = CMD_REM_COMMAND;
				*command = &(*command_list)[i+1];
			}
		} else if(state == CMD_REM_COMMAND) {
			if((*command_list)[i] == '\r')
			{
				(*command_list)[i] = '\0';
			} else if((*command_list)[i] == '\n')
			{
				(*command_list)[i] = '\0';
				(*command_list) = &(*command_list)[i+1];
				break;
			}
		}
		if((*command_list)[i+1] == '\0')
		{
			(*command_list) = &(*command_list)[i+1];
			break;
		}
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End get_next_command(alias:%s,is_group:%i,command:%s)",
		*alias,
		*is_group,
		*command);
	
	return 0;
}

/******************************************************************************
 *                                                                            *
 * Function: run_commands                                                     *
 *                                                                            *
 * Purpose: run remote commandlist for specific action                        *
 *                                                                            *
 * Parameters: trigger - trigger data                                         *
 *             action  - action data                                          *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments: commands devided with newline                                    *
 *                                                                            *
 ******************************************************************************/
void	op_run_commands(char *cmd_list)
{
	DB_RESULT	result;
	DB_ROW		row;
	char		*alias, *alias_esc, *command;
	int		is_group;

	assert(cmd_list);

	zabbix_log(LOG_LEVEL_DEBUG, "In run_commands()");

	while (1 != get_next_command(&cmd_list, &alias, &is_group, &command)) {
		if (!alias || *alias == '\0' || !command || *command == '\0')
			continue;

		if (is_group) {
			alias_esc = DBdyn_escape_string(alias);
			result = DBselect("select distinct h.host from hosts_groups hg,hosts h,groups g"
					" where hg.hostid=h.hostid and hg.groupid=g.groupid and g.name='%s'" DB_NODE,
					alias_esc,
					DBnode_local("h.hostid"));
			zbx_free(alias_esc);

			while (NULL != (row = DBfetch(result)))
				run_remote_command(row[0], command);

			DBfree_result(result);
		} else
			run_remote_command(alias, command);
	}
	zabbix_log( LOG_LEVEL_DEBUG, "End run_commands()");
}

/******************************************************************************
 *                                                                            *
 * Function: select dhostid by dserviceid                                     *
 *                                                                            *
 * Purpose: select discovered host id                                         *
 *                                                                            *
 * Parameters: dserviceid - servce id                                         *
 *                                                                            *
 * Return value: dhostid - existing dhostid, 0 - if not found                   *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static zbx_uint64_t	select_dhostid_by_dserviceid(zbx_uint64_t dserviceid)
{
	DB_RESULT	result;
	DB_ROW		row;
	zbx_uint64_t	dhostid = 0;

	zabbix_log(LOG_LEVEL_DEBUG, "In select_dhostid_by_dserviceid(dserviceid:" ZBX_FS_UI64 ")",
		dserviceid);

	result = DBselect("select dhostid from dservices where dserviceid=" ZBX_FS_UI64,
		dserviceid);
	row = DBfetch(result);
	if(row && DBis_null(row[0]) != SUCCEED)
	{
		ZBX_STR2UINT64(dhostid, row[0]);
	}
	DBfree_result(result);

	zabbix_log(LOG_LEVEL_DEBUG, "End select_dhostid_by_dserviceid()");

	return dhostid;
}

/******************************************************************************
 *                                                                            *
 * Function: select hostid of discovered host                                 *
 *                                                                            *
 * Purpose: select discovered host                                            *
 *                                                                            *
 * Parameters: dhostid - discovered host id                                   *
 *                                                                            *
 * Return value: hostid - existing hostid, o - if not found                   *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static zbx_uint64_t	select_discovered_host(zbx_uint64_t dhostid)
{
	DB_RESULT	result;
	DB_ROW		row;
	zbx_uint64_t	hostid = 0;

	zabbix_log(LOG_LEVEL_DEBUG, "In select_discovered_host(dhostid:" ZBX_FS_UI64 ")",
		dhostid);

	result = DBselect("select h.hostid from dhosts d,hosts h where h.ip=d.ip and d.dhostid=" ZBX_FS_UI64,
		dhostid);
	row = DBfetch(result);
	if(row && DBis_null(row[0]) != SUCCEED)
	{
		ZBX_STR2UINT64(hostid, row[0]);
	}
	DBfree_result(result);

	zabbix_log(LOG_LEVEL_DEBUG, "End select_discovered_host()");

	return hostid;
}

/******************************************************************************
 *                                                                            *
 * Function: add host if not added already                                    *
 *                                                                            *
 * Purpose: add discovered host                                               *
 *                                                                            *
 * Parameters: dhostid - discovered host id                                   *
 *                                                                            *
 * Return value: hostid - new/existing hostid                                 *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
static zbx_uint64_t	add_discovered_host(zbx_uint64_t dhostid)
{
	DB_RESULT	result;
	DB_RESULT	result2;
	DB_ROW		row;
	DB_ROW		row2;
	zbx_uint64_t	hostid = 0, proxy_hostid;
	char		*ip;
	char		host[MAX_STRING_LEN], *host_esc, *ip_esc;

	zabbix_log(LOG_LEVEL_DEBUG, "In add_discovered_host(dhostid:" ZBX_FS_UI64 ")",
			dhostid);

	result = DBselect("select dr.proxy_hostid,dh.ip from dhosts dh, drules dr"
			" where dr.druleid=dh.druleid and dh.dhostid=" ZBX_FS_UI64,
			dhostid);

	if (NULL != (row = DBfetch(result)) && DBis_null(row[1]) != SUCCEED) {
		proxy_hostid = zbx_atoui64(row[0]);
		ip = row[1];

		alarm(CONFIG_TIMEOUT);
		zbx_gethost_by_ip(ip, host, sizeof(host));
		alarm(0);

		host_esc = DBdyn_escape_string_len(host, HOST_HOST_LEN);
		ip_esc = DBdyn_escape_string_len(ip, HOST_IP_LEN);

		result2 = DBselect("select hostid from hosts where ip='%s'" DB_NODE,
				ip_esc,
				DBnode_local("hostid"));

		if (NULL == (row2 = DBfetch(result2)) || DBis_null(row2[0]) == SUCCEED) {
			hostid = DBget_maxid("hosts","hostid");
			DBexecute("insert into hosts (hostid,proxy_hostid,host,useip,ip,dns)"
					" values (" ZBX_FS_UI64 "," ZBX_FS_UI64 ",'%s',1,'%s','%s')",
					hostid,
					proxy_hostid,
					(*host != '\0' ? host_esc : ip_esc), /* Use host name if exists, IP otherwise */
					ip_esc,
					host_esc);
		} else {
			ZBX_STR2UINT64(hostid, row2[0]);
			if (host_esc[0] != '\0') {
				DBexecute("update hosts set dns='%s' where hostid=" ZBX_FS_UI64,
					host_esc,
					hostid);
			}
		}
		DBfree_result(result2);

		zbx_free(host_esc);
		zbx_free(ip_esc);
	}
	DBfree_result(result);

	zabbix_log(LOG_LEVEL_DEBUG, "End add_discovered_host()");

	return hostid;
}

/******************************************************************************
 *                                                                            *
 * Function: op_host_add                                                      *
 *                                                                            *
 * Purpose: add discovered host                                               *
 *                                                                            *
 * Parameters: trigger - trigger data                                         *
 *             action  - action data                                          *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	op_host_add(DB_EVENT *event)
{
	zbx_uint64_t	hostid;
	zbx_uint64_t	dhostid = 0;

	zabbix_log(LOG_LEVEL_DEBUG, "In op_host_add()");

	if(event->object == EVENT_OBJECT_DHOST)
	{
		dhostid = event->objectid;
	}
	else if(event->object == EVENT_OBJECT_DSERVICE)
	{
		dhostid = select_dhostid_by_dserviceid(event->objectid);
	}

	hostid = add_discovered_host(dhostid);

	zabbix_log(LOG_LEVEL_DEBUG, "End op_host_add()");
}

/******************************************************************************
 *                                                                            *
 * Function: op_host_del                                                      *
 *                                                                            *
 * Purpose: delete host                                                       *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	op_host_del(DB_EVENT *event)
{
	zbx_uint64_t	hostid, dhostid;

	zabbix_log(LOG_LEVEL_DEBUG, "In op_host_del()");

	if(event->object == EVENT_OBJECT_DSERVICE)
	{
		dhostid = select_dhostid_by_dserviceid(event->objectid);
	}
	else
	{
		dhostid = event->objectid;
	}

	hostid = select_discovered_host(dhostid);
	if(hostid != 0)
	{
		DBdelete_host(hostid);
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End op_host_del()");
}

/******************************************************************************
 *                                                                            *
 * Function: op_group_add                                                     *
 *                                                                            *
 * Purpose: add group to discovered host                                      *
 *                                                                            *
 * Parameters: trigger - trigger data                                         *
 *             action  - action data                                          *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	op_group_add(DB_EVENT *event, DB_ACTION *action, DB_OPERATION *operation)
{
	DB_RESULT	result;
	DB_ROW		row;
	zbx_uint64_t	hostgroupid, groupid, hostid, dhostid;

	zabbix_log(LOG_LEVEL_DEBUG, "In op_group_add(object:%d)",
		event->object);

	if(operation->operationtype != OPERATION_TYPE_GROUP_ADD)				return;
	if(event->object != EVENT_OBJECT_DHOST && event->object != EVENT_OBJECT_DSERVICE)	return;


	if(event->object == EVENT_OBJECT_DSERVICE)
	{
		dhostid = select_dhostid_by_dserviceid(event->objectid);
	}
	else
	{
		dhostid = event->objectid;
	}

	hostid = add_discovered_host(dhostid);
	if(hostid != 0)
	{
		groupid = operation->objectid;
		result = DBselect("select hostgroupid from hosts_groups where groupid=" ZBX_FS_UI64 " and hostid=" ZBX_FS_UI64,
			groupid,
			hostid);
		row = DBfetch(result);
		if(!row || DBis_null(row[0]) == SUCCEED)
		{
			hostgroupid = DBget_maxid("hosts_groups","hostgroupid");
			DBexecute("insert into hosts_groups (hostgroupid,hostid,groupid) values (" ZBX_FS_UI64 "," ZBX_FS_UI64 "," ZBX_FS_UI64 ")",
				hostgroupid,
				hostid,
				groupid);
		}
		DBfree_result(result);
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End op_group_add()");
}

/******************************************************************************
 *                                                                            *
 * Function: op_group_del                                                     *
 *                                                                            *
 * Purpose: delete group from discovered host                                 *
 *                                                                            *
 * Parameters: trigger - trigger data                                         *
 *             action  - action data                                          *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Alexei Vladishev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	op_group_del(DB_EVENT *event, DB_ACTION *action, DB_OPERATION *operation)
{
	zbx_uint64_t	groupid, hostid, dhostid;

	zabbix_log(LOG_LEVEL_DEBUG, "In op_group_del()");

	if(operation->operationtype != OPERATION_TYPE_GROUP_REMOVE)	return;
	if(event->object != EVENT_OBJECT_DHOST && event->object != EVENT_OBJECT_DSERVICE)	return;

	if(event->object == EVENT_OBJECT_DSERVICE)
	{
		dhostid = select_dhostid_by_dserviceid(event->objectid);
	}
	else
	{
		dhostid = event->objectid;
	}

	hostid = select_discovered_host(dhostid);
	if(hostid != 0)
	{
		groupid = operation->objectid;
		DBexecute("delete from hosts_groups where hostid=" ZBX_FS_UI64 " and groupid=" ZBX_FS_UI64,
				hostid,
				groupid);
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End op_group_del()");
}

/******************************************************************************
 *                                                                            *
 * Function: op_template_add                                                  *
 *                                                                            *
 * Purpose: link host with template                                           *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	op_template_add(DB_EVENT *event, DB_ACTION *action, DB_OPERATION *operation)
{
	DB_RESULT	result;
	DB_ROW		row;
	zbx_uint64_t	hosttemplateid, templateid, hostid, dhostid;

	zabbix_log(LOG_LEVEL_DEBUG, "In op_template_add(object:%d)",
		event->object);

	if(operation->operationtype != OPERATION_TYPE_TEMPLATE_ADD)				return;
	if(event->object != EVENT_OBJECT_DHOST && event->object != EVENT_OBJECT_DSERVICE)	return;


	if(event->object == EVENT_OBJECT_DSERVICE)
	{
		dhostid = select_dhostid_by_dserviceid(event->objectid);
	}
	else
	{
		dhostid = event->objectid;
	}

	hostid = add_discovered_host(dhostid);
	if(hostid != 0)
	{
		templateid = operation->objectid;

		result = DBselect("select hosttemplateid from hosts_templates where templateid=" ZBX_FS_UI64 " and hostid=" ZBX_FS_UI64,
			templateid,
			hostid);
		row = DBfetch(result);
		if(!row || DBis_null(row[0]) == SUCCEED)
		{
			hosttemplateid = DBget_maxid("hosts_templates","hosttemplateid");
			DBexecute("begin;");

			DBexecute("insert into hosts_templates (hosttemplateid,hostid,templateid) values (" ZBX_FS_UI64 "," ZBX_FS_UI64 "," ZBX_FS_UI64 ")",
				hosttemplateid,
				hostid,
				templateid);

			DBsync_host_with_template(hostid, templateid);

			DBexecute("commit;");
		}
		DBfree_result(result);
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End op_template_add()");
}

/******************************************************************************
 *                                                                            *
 * Function: op_template_del                                                  *
 *                                                                            *
 * Purpose: unlink and clear host from template                               *
 *                                                                            *
 * Parameters:                                                                *
 *                                                                            *
 * Return value: nothing                                                      *
 *                                                                            *
 * Author: Eugene Grigorjev                                                   *
 *                                                                            *
 * Comments:                                                                  *
 *                                                                            *
 ******************************************************************************/
void	op_template_del(DB_EVENT *event, DB_ACTION *action, DB_OPERATION *operation)
{
	DB_RESULT	result;
	DB_ROW		row;
	zbx_uint64_t	templateid, hostid, dhostid;

	zabbix_log(LOG_LEVEL_DEBUG, "In op_template_del(object:%d)",
		event->object);

	if(operation->operationtype != OPERATION_TYPE_TEMPLATE_REMOVE)				return;
	if(event->object != EVENT_OBJECT_DHOST && event->object != EVENT_OBJECT_DSERVICE)	return;


	if(event->object == EVENT_OBJECT_DSERVICE)
	{
		dhostid = select_dhostid_by_dserviceid(event->objectid);
	}
	else
	{
		dhostid = event->objectid;
	}

	hostid = select_discovered_host(dhostid);
	if(hostid != 0)
	{
		templateid = operation->objectid;

		result = DBselect("select hosttemplateid from hosts_templates where templateid=" ZBX_FS_UI64 " and hostid=" ZBX_FS_UI64,
			templateid,
			hostid);

		if( (row = DBfetch(result)) )
		{
			DBexecute("begin;");

			DBdelete_template_elements(hostid, templateid, 0 /* not a unlink mode */);

			DBexecute("delete from hosts_templates where "
					"hostid=" ZBX_FS_UI64 " and templateid=" ZBX_FS_UI64,
				hostid,
				templateid);

			DBexecute("commit;");
		}
		DBfree_result(result);
	}

	zabbix_log(LOG_LEVEL_DEBUG, "End op_template_del()");
}

