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


#ifndef ZABBIX_DB_H
#define ZABBIX_DB_H

/* time_t */
#include <time.h>

#include "common.h"
#include "zbxdb.h"
#include "dbschema.h"

extern	char	*CONFIG_DBHOST;
extern	char	*CONFIG_DBNAME;
extern	char	*CONFIG_DBUSER;
extern	char	*CONFIG_DBPASSWORD;
extern	char	*CONFIG_DBSOCKET;
extern	int	CONFIG_DBPORT;
extern	int	CONFIG_NODEID;
extern	int	CONFIG_MASTER_NODEID;
extern	int	CONFIG_DBSYNCER_FORKS;
extern	int	CONFIG_NODE_NOHISTORY;
extern  int     CONFIG_REFRESH_UNSUPPORTED;
extern	int	CONFIG_UNAVAILABLE_DELAY;

typedef enum {
	GRAPH_TYPE_NORMAL = 0,
	GRAPH_TYPE_STACKED = 1
} zbx_graph_types;

typedef enum {
	SCREEN_RESOURCE_GRAPH = 0,
	SCREEN_RESOURCE_SIMPLE_GRAPH,
	SCREEN_RESOURCE_MAP,
	SCREEN_RESOURCE_PLAIN_TEXT,
	SCREEN_RESOURCE_HOSTS_INFO,
	SCREEN_RESOURCE_TRIGGERS_INFO,
	SCREEN_RESOURCE_SERVER_INFO,
	SCREEN_RESOURCE_CLOCK,
	SCREEN_RESOURCE_SCREEN,
	SCREEN_RESOURCE_TRIGGERS_OVERVIEW,
	SCREEN_RESOURCE_DATA_OVERVIEW,
	SCREEN_RESOURCE_URL,
	SCREEN_RESOURCE_ACTIONS,
	SCREEN_RESOURCE_EVENTS
} zbx_screen_resources;

typedef enum {
	CALC_FNC_MIN = 1,
	CALC_FNC_AVG = 2,
	CALC_FNC_MAX = 4,
	CALC_FNC_ALL = 7
} zbx_graph_item_calc_function;

typedef enum {
	GRAPH_ITEM_SIMPLE = 0,
	GRAPH_ITEM_AGGREGATED = 1
} zbx_graph_item_type;

#define	ZBX_DB_CONNECT_NORMAL	0
#define	ZBX_DB_CONNECT_EXIT	1

#define DB_FULL_DELETE	0
#define DB_PART_DELETE	1

#define DB_ACTION	struct zbx_action_type
#define DB_ALERT	struct zbx_alert_type
#define DB_CONDITION	struct zbx_condition_type
#define DB_DHOST	struct zbx_dhost_type
#define DB_DRULE	struct zbx_drule_type
#define DB_DSERVICE	struct zbx_dservice_type
#define DB_DCHECK	struct zbx_dcheck_type
#define DB_EVENT	struct zbx_event_type
#define DB_FUNCTION	struct zbx_function_type
#define DB_GRAPH	struct zbx_graph_type
#define DB_GRAPH_ITEM	struct zbx_graph_item_type
#define DB_HOST		struct zbx_host_type
#define DB_HOUSEKEEPER	struct zbx_housekeeper_type
#define DB_ITEM		struct zbx_item_type
#define DB_MEDIA	struct zbx_media_type
#define DB_MEDIATYPE	struct zbx_mediatype_type
#define DB_OPERATION	struct zbx_operation_type
#define DB_TRIGGER	struct zbx_trigger_type
#define DB_HTTPTEST	struct zbx_httptest_type
#define DB_HTTPSTEP	struct zbx_httpstep_type
#define DB_HTTPSTEPITEM	struct zbx_httpstepitem_type
#define DB_HTTPTESTITEM	struct zbx_httptestitem_type
#define DB_ESCALATION	struct zbx_escalation_type

/* Trigger related defines */
#define TRIGGER_DESCRIPTION_LEN		255
#define TRIGGER_DESCRIPTION_LEN_MAX	TRIGGER_DESCRIPTION_LEN+1
#define TRIGGER_EXPRESSION_LEN		255
#define TRIGGER_EXPRESSION_LEN_MAX	TRIGGER_EXPRESSION_LEN+1
#define TRIGGER_URL_LEN			255
#define TRIGGER_URL_LEN_MAX		TRIGGER_URL_LEN+1
#define TRIGGER_ERROR_LEN		128
#define TRIGGER_ERROR_LEN_MAX		TRIGGER_ERROR_LEN+1

#define CONDITION_VALUE_LEN		255
#define CONDITION_VALUE_LEN_MAX		CONDITION_VALUE_LEN+1

#define HOST_HOST_LEN			64
#define HOST_HOST_LEN_MAX		HOST_HOST_LEN+1
#define HOST_DNS_LEN			64
#define HOST_DNS_LEN_MAX		HOST_DNS_LEN+1
#define HOST_IP_LEN			39
#define HOST_IP_LEN_MAX			HOST_IP_LEN+1
#define HOST_ADDR_LEN			64 /* MAX(HOST_DNS_LEN,HOST_IP_LEN) */
#define HOST_ADDR_LEN_MAX		HOST_ADDR_LEN+1
#define HOST_ERROR_LEN			128
#define HOST_ERROR_LEN_MAX		HOST_ERROR_LEN+1
#define HOST_IPMI_USERNAME_LEN		128
#define HOST_IPMI_USERNAME_LEN_MAX	HOST_IPMI_USERNAME_LEN+1
#define HOST_IPMI_PASSWORD_LEN		128
#define HOST_IPMI_PASSWORD_LEN_MAX	HOST_IPMI_PASSWORD_LEN+1

#define ITEM_KEY_LEN			255
#define ITEM_KEY_LEN_MAX		ITEM_KEY_LEN+1
#define ITEM_DESCRIPTION_LEN		255
#define ITEM_DESCRIPTION_LEN_MAX	ITEM_DESCRIPTION_LEN+1
#define ITEM_SNMP_COMMUNITY_LEN		64
#define ITEM_SNMP_COMMUNITY_LEN_MAX	ITEM_SNMP_COMMUNITY_LEN+1
#define ITEM_SNMP_OID_LEN		255
#define ITEM_SNMP_OID_LEN_MAX		ITEM_SNMP_OID_LEN+1
#define ITEM_LASTVALUE_LEN		255
#define ITEM_LASTVALUE_LEN_MAX		ITEM_LASTVALUE_LEN+1
#define ITEM_ERROR_LEN			128
#define ITEM_ERROR_LEN_MAX		ITEM_ERROR_LEN+1
#define ITEM_TRAPPER_HOSTS_LEN		255
#define ITEM_TRAPPER_HOSTS_LEN_MAX	ITEM_TRAPPER_HOSTS_LEN+1
#define ITEM_UNITS_LEN			10
#define ITEM_UNITS_LEN_MAX		ITEM_UNITS_LEN+1
#define ITEM_SNMPV3_SECURITYNAME_LEN		64
#define ITEM_SNMPV3_SECURITYNAME_LEN_MAX	ITEM_SNMPV3_SECURITYNAME_LEN+1
#define ITEM_SNMPV3_AUTHPASSPHRASE_LEN		64
#define ITEM_SNMPV3_AUTHPASSPHRASE_LEN_MAX	ITEM_SNMPV3_AUTHPASSPHRASE_LEN+1
#define ITEM_SNMPV3_PRIVPASSPHRASE_LEN		64
#define ITEM_SNMPV3_PRIVPASSPHRASE_LEN_MAX	ITEM_SNMPV3_PRIVPASSPHRASE_LEN+1
#define ITEM_FORMULA_LEN		255
#define ITEM_FORMULA_LEN_MAX		ITEM_FORMULA_LEN+1
#define ITEM_LOGTIMEFMT_LEN		64
#define ITEM_LOGTIMEFMT_LEN_MAX		ITEM_LOGTIMEFMT_LEN+1
#define ITEM_DELAY_FLEX_LEN		255
#define ITEM_DELAY_FLEX_LEN_MAX		ITEM_DELAY_FLEX_LEN+1

#define FUNCTION_LASTVALUE_LEN		255
#define FUNCTION_LASTVALUE_LEN_MAX	FUNCTION_LASTVALUE_LEN+1
#define FUNCTION_FUNCTION_LEN		12
#define FUNCTION_FUNCTION_LEN_MAX	FUNCTION_FUNCTION_LEN+1
#define FUNCTION_PARAMETER_LEN		255
#define FUNCTION_PARAMETER_LEN_MAX	FUNCTION_PARAMETER_LEN+1

#define HISTORY_STR_VALUE_LEN		255
#define HISTORY_STR_VALUE_LEN_MAX	HISTORY_STR_VALUE_LEN+1

#define	HISTORY_TEXT_VALUE_LEN		65535
#define	HISTORY_TEXT_VALUE_LEN_MAX	HISTORY_TEXT_VALUE_LEN+1

#define	HISTORY_LOG_VALUE_LEN		65535
#define	HISTORY_LOG_VALUE_LEN_MAX	HISTORY_LOG_VALUE_LEN+1
#define HISTORY_LOG_SOURCE_LEN		64
#define HISTORY_LOG_SOURCE_LEN_MAX	HISTORY_LOG_SOURCE_LEN+1

#define ALERT_SENDTO_LEN		100
#define ALERT_SENDTO_LEN_MAX		ALERT_SENDTO_LEN+1
#define ALERT_SUBJECT_LEN		255
#define ALERT_SUBJECT_LEN_MAX		ALERT_SUBJECT_LEN+1
#define ALERT_ERROR_LEN			128
#define ALERT_ERROR_LEN_MAX		ALERT_ERROR_LEN+1

#define APPLICATION_NAME_LEN		255
#define APPLICATION_NAME_LEN_MAX	APPLICATION_NAME_LEN+1

#define GRAPH_NAME_LEN			128
#define GRAPH_NAME_LEN_MAX		GRAPH_NAME_LEN+1

#define GRAPH_ITEM_COLOR_LEN		32
#define GRAPH_ITEM_COLOR_LEN_MAX	GRAPH_ITEM_COLOR_LEN+1

#define ACTION_SUBJECT_LEN		255
#define ACTION_SUBJECT_LEN_MAX		ACTION_SUBJECT_LEN+1

#define DHOST_IP_LEN			39
#define DHOST_IP_LEN_MAX		DHOST_IP_LEN+1

#define DSERVICE_KEY_LEN		255
#define DSERVICE_KEY_LEN_MAX		DSERVICE_KEY_LEN+1
#define DSERVICE_VALUE_LEN		255
#define DSERVICE_VALUE_LEN_MAX		DSERVICE_VALUE_LEN+1

#define PROXY_DHISTORY_IP_LEN		39
#define PROXY_DHISTORY_IP_LEN_MAX	PROXY_DHISTORY_IP_LEN+1
#define PROXY_DHISTORY_KEY_LEN		255
#define PROXY_DHISTORY_KEY_LEN_MAX	PROXY_DHISTORY_KEY_LEN+1
#define PROXY_DHISTORY_VALUE_LEN	255
#define PROXY_DHISTORY_VALUE_LEN_MAX	PROXY_DHISTORY_VALUE_LEN+1

#define HTTPTEST_ERROR_LEN		255
#define HTTPTEST_ERROR_LEN_MAX		HTTPTEST_ERROR_LEN+1

#define HTTPSTEP_STATUS_LEN		255
#define HTTPSTEP_STATUS_LEN_MAX		HTTPSTEP_STATUS_LEN+1

#define HTTPSTEP_REQUIRED_LEN		255
#define HTTPSTEP_REQUIRED_LEN_MAX	HTTPSTEP_REQUIRED_LEN+1

#define ZBX_SQL_ITEM_FIELDS	"i.itemid,i.key_,h.host,h.port,i.delay,i.description,i.nextcheck,i.type,i.snmp_community,i.snmp_oid,h.useip,h.ip,i.history,i.lastvalue,i.prevvalue,i.hostid,h.status,i.value_type,h.errors_from,i.snmp_port,i.delta,i.prevorgvalue,i.lastclock,i.units,i.multiplier,i.snmpv3_securityname,i.snmpv3_securitylevel,i.snmpv3_authpassphrase,i.snmpv3_privpassphrase,i.formula,h.available,i.status,i.trapper_hosts,i.logtimefmt,i.valuemapid,i.delay_flex,h.dns,i.params,i.trends,h.useipmi,h.ipmi_port,h.ipmi_authtype,h.ipmi_privilege,h.ipmi_username,h.ipmi_password,i.ipmi_sensor,i.lastlogsize"
#define ZBX_SQL_ITEM_TABLES	"hosts h, items i"
#define ZBX_SQL_ITEM_FIELDS_NUM	47
#define ZBX_SQL_ITEM_SELECT	ZBX_SQL_ITEM_FIELDS " from " ZBX_SQL_ITEM_TABLES

#define ZBX_MAX_SQL_LEN			65535

DB_DRULE
{
	zbx_uint64_t	druleid;
	char		*iprange;
	int		delay;
	int		nextcheck;
	char		*name;
	int		status;
};

DB_DCHECK
{
	zbx_uint64_t	dcheckid;
	zbx_uint64_t	druleid;
	int		type;
	char		*ports;
	char		*key_;
	char		*snmp_community;
	int		status;
	char		value[DSERVICE_VALUE_LEN_MAX];
};

DB_DHOST
{
	zbx_uint64_t	dhostid;
	zbx_uint64_t	druleid;
	char		ip[HOST_IP_LEN_MAX];
	int		status;
	int		lastup;
	int		lastdown;
};

DB_DSERVICE
{
	zbx_uint64_t	dserviceid;
	zbx_uint64_t	dhostid;
	int		type;
	int		port;
	int		status;
	int		lastup;
	int		lastdown;
	char		value[DSERVICE_VALUE_LEN_MAX];
	char		key_[ITEM_KEY_LEN_MAX];
};

DB_EVENT
{
	zbx_uint64_t	eventid;
	int		source;
	int		object;
	zbx_uint64_t	objectid;
	int		clock;
	int		value;
	int		acknowledged;
	int		skip_actions;
	char		trigger_description[TRIGGER_DESCRIPTION_LEN_MAX];
	int		trigger_priority;
	char		*trigger_url;
	char		*trigger_comments;
	int		trigger_type;
};

DB_HOST
{
	zbx_uint64_t     hostid;
	char    host[HOST_HOST_LEN_MAX];
	char    dns[HOST_DNS_LEN_MAX];
	int     useip;
	char    ip[HOST_IP_LEN_MAX];
	int	port;
	int	status;
	int	disable_until;
	int	errors_from;
	char	error[HOST_ERROR_LEN_MAX];
	int	available;
};

DB_GRAPH
{
	zbx_uint64_t	graphid;
	char	name[GRAPH_NAME_LEN_MAX];
	int	width;
	int	height;
	int	yaxistype;
	double	yaxismin;
	double	yaxismax;
};

DB_GRAPH_ITEM
{
	zbx_uint64_t	gitemid;
	zbx_uint64_t	graphid;
	zbx_uint64_t	itemid;
	int	drawtype;
	int	sortorder;
	char	color[GRAPH_ITEM_COLOR_LEN_MAX];
};

DB_ITEM
{
	zbx_uint64_t	itemid;
	zbx_uint64_t	hostid;
	zbx_item_type_t	type;
	zbx_item_status_t	status;
	char	*description;
	char	*key;
	char	*key_orig;
	char	*host_name;
	char	*host_ip;
	char	*host_dns;
	int	host_status;
	int	host_available;
	int	host_errors_from;
	int	useip;
	char	*snmp_community;
	char	*snmp_oid;
	int	snmp_port;
	char	*trapper_hosts;
	int     port;
	int     delay;
	int     history;
	int	trends;
	char	*prevorgvalue_str;
	double	prevorgvalue_dbl;
	zbx_uint64_t	prevorgvalue_uint64;
	int	prevorgvalue_null;
	char	*lastvalue_str;
	double	lastvalue_dbl;
	zbx_uint64_t	lastvalue_uint64;
	int	lastclock;
	int     lastvalue_null;
	char	*prevvalue_str;
	double	prevvalue_dbl;
	zbx_uint64_t	prevvalue_uint64;
	int     prevvalue_null;
	time_t  lastcheck;
	time_t	nextcheck;
	zbx_item_value_type_t	value_type;
	int	delta;
	int	multiplier;
	char	*units;

	char	*snmpv3_securityname;
	int	snmpv3_securitylevel;
	char	*snmpv3_authpassphrase;
	char	*snmpv3_privpassphrase;

	char	*formula;
	int	lastlogsize;
	int	timestamp;
	int	eventlog_severity;
	char	*eventlog_source;

	char	*logtimefmt;
	zbx_uint64_t	valuemapid;
	char	*delay_flex;
	char	*params;

	int	useipmi;
	char	*ipmi_ip;
	int	ipmi_port;
	int	ipmi_authtype;
	int	ipmi_privilege;
	char	*ipmi_username;
	char	*ipmi_password;
	char	*ipmi_sensor;
};
 
DB_FUNCTION
{
	zbx_uint64_t     functionid;
	zbx_uint64_t     itemid;
	zbx_uint64_t     triggerid;
	double  lastvalue;
	int	lastvalue_null;
	char    *function;
/*	int     parameter;*/
	char	*parameter;
};

DB_MEDIA
{
	zbx_uint64_t	mediaid;
/*	char	*type;*/
	zbx_uint64_t	mediatypeid;
	char	*sendto;
	char	*period;
	int	active;
	int	severity;
};

DB_MEDIATYPE
{
	zbx_uint64_t		mediatypeid;
	zbx_media_type_t	type;
	char	*description;
	char	*smtp_server;
	char	*smtp_helo;
	char	*smtp_email;
	char	*exec_path;
	char	*gsm_modem;
	char	*username;
	char	*passwd;
};

DB_TRIGGER
{
	zbx_uint64_t	triggerid;
	char	expression[TRIGGER_EXPRESSION_LEN_MAX];
	char	description[TRIGGER_DESCRIPTION_LEN_MAX];
	char	*url;
	char	*comments;
	int	status;
	int	value;
/*	int	prevvalue; */
	int	priority;
	int	type;
};

DB_ACTION
{
	zbx_uint64_t	actionid;
	int		actiontype;
	int		evaltype;
	int		status;
	int		eventsource;
	int		esc_period;
	char		*shortdata;
	char		*longdata;
	int		recovery_msg;
};

DB_OPERATION
{
	zbx_uint64_t	operationid;
	zbx_uint64_t	actionid;
	int		operationtype;
	int		object;
	zbx_uint64_t	objectid;
	char		*shortdata;
	char		*longdata;
	int		esc_period;
	int		default_msg;
	int		evaltype;
};

DB_CONDITION
{
	zbx_uint64_t	conditionid;
	zbx_uint64_t	actionid;
	zbx_condition_type_t	conditiontype;
	zbx_condition_op_t	operator;
	char	*value;
};

DB_ALERT
{
	zbx_uint64_t	alertid;
	zbx_uint64_t 	actionid;
	int 	clock;
/*	char	*type;*/
	zbx_uint64_t	mediatypeid;
	char	*sendto;
	char	*subject;
	char	*message;
	zbx_alert_status_t	status;
	int	retries;
/*	int	delay;*/
};

DB_HOUSEKEEPER
{
	zbx_uint64_t	housekeeperid;
	char	*tablename;
	char	*field;
	zbx_uint64_t	value;
};

DB_HTTPTEST
{
	zbx_uint64_t	httptestid;
	char		*name;
	zbx_uint64_t	applicationid;
	int		nextcheck;
	int		status;
	int		delay;
	char		*macros;
	char		*agent;
	double		speed;
	double		time;
};

DB_HTTPSTEP
{
	zbx_uint64_t	httpstepid;
	zbx_uint64_t	httptestid;
	int		no;
	char		*name;
	char		url[MAX_STRING_LEN];	/* excessive length is required to support macros */
	int		timeout;
	char		posts[MAX_STRING_LEN];
	char		required[HTTPSTEP_REQUIRED_LEN_MAX];
	char		status_codes[HTTPSTEP_STATUS_LEN_MAX];
};

DB_HTTPSTEPITEM
{
	zbx_uint64_t	httpstepitemid;
	zbx_uint64_t	httpstepid;
	zbx_uint64_t	itemid;
	zbx_httpitem_type_t	type;
};

DB_HTTPTESTITEM
{
	zbx_uint64_t	httptestitemid;
	zbx_uint64_t	httptestid;
	zbx_uint64_t	itemid;
	zbx_httpitem_type_t	type;
};

DB_ESCALATION
{
	zbx_uint64_t		escalationid;
	zbx_uint64_t		actionid;
	zbx_uint64_t		triggerid;
	zbx_uint64_t		eventid;
	zbx_uint64_t		r_eventid;
	int			esc_step;
	zbx_escalation_status_t	status;
	int			nextcheck;
};

#define DB_NODE "%s"
#define DBnode_local(fieldid) DBnode(fieldid, CONFIG_NODEID)
const char *DBnode(const char *fieldid, const int nodeid);

int	DBping(void);

void    DBconnect(int flag);
void	DBinit();

void    DBclose(void);
void    DBvacuum(void);

#ifdef HAVE___VA_ARGS__
#	define DBexecute(fmt, ...) __zbx_DBexecute(ZBX_CONST_STRING(fmt), ##__VA_ARGS__)
#else
#	define DBexecute __zbx_DBexecute
#endif /* HAVE___VA_ARGS__ */
int	__zbx_DBexecute(const char *fmt, ...);

#ifdef HAVE___VA_ARGS__
#	define DBselect(fmt, ...) __zbx_DBselect(ZBX_CONST_STRING(fmt), ##__VA_ARGS__)
#else
#	define DBselect __zbx_DBselect
#endif /* HAVE___VA_ARGS__ */
DB_RESULT	__zbx_DBselect(const char *fmt, ...);

DB_RESULT	DBselectN(char *query, int n);
DB_ROW		DBfetch(DB_RESULT result);
zbx_uint64_t	DBinsert_id(int exec_result, const char *table, const char *field);
int		DBis_null(char *field);
void		DBbegin();
void		DBcommit();
void		DBrollback();

const ZBX_TABLE	*DBget_table(const char *tablename);
const ZBX_FIELD	*DBget_field(const ZBX_TABLE *table, const char *fieldname);
#define DBget_maxid(table, field)	DBget_maxid_num(table, field, 1)
zbx_uint64_t	DBget_maxid_num(char *table, char *field, int num);

int     DBget_function_result(char **result,char *functionid, char *error, int maxerrlen);
void	DBupdate_host_availability(DB_ITEM *item, int available, int clock, const char *error);
void	DBproxy_update_host_availability(DB_ITEM *item, int available, int clock);
int	DBupdate_item_status_to_notsupported(DB_ITEM *item, int clock, const char *error);
int	DBadd_service_alarm(zbx_uint64_t serviceid,int status,int clock);
int	DBadd_alert(zbx_uint64_t actionid, zbx_uint64_t eventid, zbx_uint64_t userid, zbx_uint64_t mediatypeid, char *sendto, char *subject, char *message);
int	DBstart_escalation(zbx_uint64_t actionid, zbx_uint64_t triggerid, zbx_uint64_t eventid);
int	DBstop_escalation(zbx_uint64_t actionid, zbx_uint64_t triggerid, zbx_uint64_t eventid);
int	DBremove_escalation(zbx_uint64_t escalationid);
void	DBupdate_triggers_status_after_restart(void);
int	DBget_prev_trigger_value(zbx_uint64_t triggerid);
int     DBupdate_trigger_value(DB_TRIGGER *trigger, int new_value, int now, const char *reason);

int	DBget_items_count(void);
int	DBget_items_unsupported_count(void);
int	DBget_history_count(void);
int	DBget_history_str_count(void);
int	DBget_trends_count(void);
int	DBget_triggers_count(void);
int	DBget_queue_count(void);
double	DBget_requiredperformance(void);
zbx_uint64_t DBget_proxy_lastaccess(const char *hostname);

void    DBescape_string(const char *from, char *to, int maxlen);
char*   DBdyn_escape_string(const char *str);
char*	DBdyn_escape_string_len(const char *src, int max_src_len);

void    DBget_item_from_db(DB_ITEM *item,DB_ROW row);

zbx_uint64_t	DBadd_host(char *server, int port, int status, int useip, char *ip, int disable_until, int available);
int	DBhost_exists(char *server);
int	DBget_host_by_hostid(int hostid,DB_HOST *host);
int	DBadd_templates_to_host(int hostid,int host_templateid);

int	DBadd_template_linkage(int hostid,int templateid,int items,int triggers,int graphs);

int	DBget_item_by_itemid(int itemid,DB_ITEM *item);

int	DBget_trigger_by_triggerid(int triggerid,DB_TRIGGER *trigger);
int	DBadd_trigger_to_linked_hosts(int triggerid,int hostid);
void	DBdelete_sysmaps_hosts_by_hostid(zbx_uint64_t hostid);

int	DBget_graph_item_by_gitemid(int gitemid, DB_GRAPH_ITEM *graph_item);
int	DBget_graph_by_graphid(int graphid, DB_GRAPH *graph);
int	DBadd_graph_item_to_linked_hosts(int gitemid,int hostid);
void	get_latest_event_status(zbx_uint64_t triggerid, int *prev_status, int *latest_status);


void	DBdelete_template_elements(
		zbx_uint64_t  hostid,
		zbx_uint64_t templateid,
		unsigned char unlink_mode
	);
int	DBcopy_template_elements(
		zbx_uint64_t hostid,
		zbx_uint64_t templateid,
		unsigned char copy_mode
	);
int	DBsync_host_with_template(
		zbx_uint64_t hostid,
		zbx_uint64_t templateid
	);
int	DBsync_host_with_templates(
		zbx_uint64_t hostid
	);
int	DBdelete_host(
		zbx_uint64_t hostid
	);
void	DBupdate_services_rec(
		zbx_uint64_t serviceid
	);
void	DBupdate_services(
		zbx_uint64_t triggerid,
		int status
	);

/* History related functions */
int	DBadd_trend(zbx_uint64_t itemid, double value, int clock);
int	DBadd_trend_uint(zbx_uint64_t itemid, zbx_uint64_t value, int clock);

int	DBadd_history(zbx_uint64_t itemid, double value, int clock);
int	DBadd_history_log(zbx_uint64_t itemid, char *value, int clock, int timestamp, char *source, int severity, int lastlogsize);
int	DBadd_history_str(zbx_uint64_t itemid, char *value, int clock);
int	DBadd_history_text(zbx_uint64_t itemid, char *value, int clock);
int	DBadd_history_uint(zbx_uint64_t itemid, zbx_uint64_t value, int clock);

void	DBproxy_add_history(zbx_uint64_t itemid, double value, int clock);
void	DBproxy_add_history_uint(zbx_uint64_t itemid, zbx_uint64_t value, int clock);
void	DBproxy_add_history_str(zbx_uint64_t itemid, char *value, int clock);
void	DBproxy_add_history_text(zbx_uint64_t itemid, char *value, int clock);
void	DBproxy_add_history_log(zbx_uint64_t itemid, char *value, int clock, int timestamp, char *source, int severity, int lastlogsize);


void	DBadd_condition_alloc(char **sql, int *sql_alloc, int *sql_offset, const char *fieldname, const zbx_uint64_t *values, const int num);
char	*zbx_host_key_string(zbx_uint64_t itemid);
char	*zbx_host_key_string_by_item(DB_ITEM *item);
char	*zbx_host_key_function_string(zbx_uint64_t functionid);

double	DBmultiply_value_float(DB_ITEM *item, double value);
zbx_uint64_t	DBmultiply_value_uint64(DB_ITEM *item, zbx_uint64_t value);
#endif
