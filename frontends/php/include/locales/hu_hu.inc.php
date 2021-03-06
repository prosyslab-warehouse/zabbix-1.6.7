<?php
/*
** ZABBIX
** Copyright (C) 2000-2008 SIA Zabbix
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
?>
<?php
	global $TRANSLATION;

	$TRANSLATION=array(

	'S_DATE_FORMAT_YMDHMS'=>			'M d H:i:s',
	'S_HTML_CHARSET'=>			'UTF-8',
	'S_ACTIVATE_SELECTED'=>			'A kiválasztott aktíválása',
	'S_DISABLE_SELECTED'=>			'Kiválasztott tiltása',
	'S_DELETE_SELECTED'=>			'Kiválasztott törlése',
	'S_COPY_SELECTED_TO'=>			'A kiválasztott másolása ...',
	'S_HOST_IP'=>			'Host ip',
	'S_SERVICE_TYPE'=>			'Szervíz típus',
	'S_SERVICE_PORT'=>			'Szervíz port',
	'S_DISCOVERY_STATUS'=>			'Felderítés állapota',
	'S_RECEIVED_VALUE'=>			'Kapott érték',
	'S_UPTIME_DOWNTIME'=>			'Uptime/Downtime',
	'S_DISCOVERY_RULE'=>			'Felderítési szabály',
	'S_DISCOVERY'=>			'Felderítés',
	'S_DISCOVERY_BIG'=>			'FELDERÍTÉS',
	'S_CONFIGURATION_OF_DISCOVERY'=>			'A felderítés konfigurációja',
	'S_CONFIGURATION_OF_DISCOVERY_BIG'=>			'A FELDERÍTÉS KONFIGURÁCIÓJA',
	'S_NO_DISCOVERY_RULES_DEFINED'=>			'Nincs felderítési szabály definiálva',
	'S_IP_RANGE'=>			'IP tartomány',
	'S_CHECKS'=>			'Vizsgálat',
	'S_ENABLE_SELECTED_RULES_Q'=>			'Engedélyezed a kiválasztott szabályt?',
	'S_DISABLE_SELECTED_RULES_Q'=>			'Kikapcsolod a kiválasztott szabályt?',
	'S_DELETE_SELECTED_RULES_Q'=>			'Törlöd a kiválasztott szabályt?',
	'S_CREATE_RULE'=>			'Új szabály',
	'S_DELETE_RULE_Q'=>			'Szabály törlése?',
	'S_EVENT_SOURCE'=>			'Esemény forrása',
	'S_NEW_CHECK'=>			'Új vizsgálat',
	'S_SSH'=>			'SSH',
	'S_LDAP'=>			'LDAP',
	'S_SMTP'=>			'SMTP',
	'S_FTP'=>			'FTP',
	'S_HTTP'=>			'HTTP',
	'S_POP'=>			'POP',
	'S_NNTP'=>			'NNTP',
	'S_IMAP'=>			'IMAP',
	'S_TCP'=>			'TCP',
	'S_PORTS_SMALL'=>			'portok',
	'S_DISCOVERY_RULES_DELETED'=>			'Felderítési szabályok törölve',
	'S_DISCOVERY_RULE_DELETED'=>			'Felderítési szabály törölve',
	'S_CANNOT_DELETE_DISCOVERY_RULE'=>			'Nem lehet törölni a felderítési szabályt',
	'S_DISCOVERY_RULES_UPDATED'=>			'Felderítési szabályok módositva',
	'S_DISCOVERY_RULE_UPDATED'=>			'Felderítési szabály módositva',
	'S_CANNOT_UPDATE_DISCOVERY_RULE'=>			'Nem lehet módosítani a felderítési szabályt',
	'S_DISCOVERY_RULE_ADDED'=>			'Felderítési szabály rögzitve',
	'S_CANNOT_ADD_DISCOVERY_RULE'=>			'Nem lehet hozzáadni felderítési szabályt',
	'S_STATUS_OF_DISCOVERY_BIG'=>			'FELDERÍTÉS ÁLLAPOTA',
	'S_STATUS_OF_DISCOVERY'=>			'Felderítés állapota',
	'S_DETAILS_OF_SCENARIO'=>			'Details of scenario',
	'S_DETAILS_OF_SCENARIO_BIG'=>			'DETAILS OF SCENARIO',
	'S_SPEED'=>			'Speed',
	'S_RESPONSE_CODE'=>			'Response code',
	'S_TOTAL_BIG'=>			'TOTAL',
	'S_RESPONSE_TIME'=>			'Response time',
	'S_IN_PROGRESS'=>			'In progress',
	'S_OF_SMALL'=>			'of',
	'S_IN_CHECK'=>			'In check',
	'S_IDLE_TILL'=>			'Idle till',
	'S_FAILED_ON'=>			'Failed on',
	'S_STATUS_OF_WEB_MONITORING'=>			'Web felügyelet állapota',
	'S_STATUS_OF_WEB_MONITORING_BIG'=>			'WEB FELÜGYELET ÁLLAPOTA',
	'S_STATE'=>			'Állapot',
	'S_STATUS_CODES'=>			'Státusz kódok',
	'S_WEB'=>			'Web',
	'S_CONFIGURATION_OF_WEB_MONITORING'=>			'Web felügyelet beállításai',
	'S_CONFIGURATION_OF_WEB_MONITORING_BIG'=>			'WEB FELÜGYELET BEÁLLÍTÁSAI',
	'S_SCENARIO'=>			'Scenario',
	'S_SCENARIOS'=>			'Scenarios',
	'S_SCENARIOS_BIG'=>			'SCENARIOS',
	'S_CREATE_SCENARIO'=>			'Create scenario',
	'S_HIDE_DISABLED_SCENARIOS'=>			'Hide disabled scenarios',
	'S_SHOW_DISABLED_SCENARIOS'=>			'Show disabled scenarios',
	'S_NUMBER_OF_STEPS'=>			'Number of steps',
	'S_SCENARIO_DELETED'=>			'Scenario deleted',
	'S_SCENARIO_ACTIVATED'=>			'Scenario activated',
	'S_SCENARIO_DISABLED'=>			'Scenario disabled',
	'S_ACTIVATE_SELECTED_SCENARIOS_Q'=>			'Activate selected scenarios?',
	'S_DISABLE_SELECTED_SCENARIOS_Q'=>			'Disable selected scenarios?',
	'S_DELETE_SELECTED_SCENARIOS_Q'=>			'Delete selected scenarios?',
	'S_DELETE_SCENARIO_Q'=>			'Delete scenario?',
	'S_CLEAN_HISTORY_SELECTED_SCENARIOS'=>			'Clean history selected scenarios',
	'S_SCENARIO_UPDATED'=>			'Scenario updated',
	'S_CANNOT_UPDATE_SCENARIO'=>			'Cannot update scenario',
	'S_SCENARIO_ADDED'=>			'Scenario added',
	'S_CANNOT_ADD_SCENARIO'=>			'Cannot add scenario',
	'S_CANNOT_DELETE_SCENARIO'=>			'Cannot delete scenario',
	'S_AGENT'=>			'Agent',
	'S_VARIABLES'=>			'Variables',
	'S_STEP'=>			'Step',
	'S_STEPS'=>			'Steps',
	'S_TIMEOUT'=>			'Timeout',
	'S_POST'=>			'Post',
	'S_REQUIRED'=>			'Required',
	'S_STEP_OF_SCENARIO'=>			'Step of scenario',
	'S_ELEMENT'=>			'Element',
	'S_ELEMENTS'=>			'Elements',
	'S_ONLY_HOST_INFO'=>			'Only host info',
	'S_EXPORT_IMPORT'=>			'Export/Import',
	'S_IMPORT_FILE'=>			'Import file',
	'S_IMPORT'=>			'Import',
	'S_IMPORT_BIG'=>			'IMPORT',
	'S_EXPORT'=>			'Export',
	'S_EXPORT_BIG'=>			'EXPORT',
	'S_PREVIEW'=>			'Előnézet',
	'S_BACK'=>			'Back',
	'S_NO_DATA_FOR_EXPORT'=>			'No data for export',
	'S_RULES'=>			'Rules',
	'S_SKIP'=>			'Skip',
	'S_EXISTING'=>			'Existing',
	'S_MISSING'=>			'Missing',
	'S_REFRESH'=>			'Refresh',
	'S_PREVIOUS'=>			'Előző',
	'S_NEXT'=>			'Következő',
	'S_RETRY'=>			'Újra',
	'S_FINISH'=>			'Vége',
	'S_FAIL'=>			'Hamis',
	'S_UPDATE_BIG'=>			'UPDATE',
	'S_INSTALLATION'=>			'Installation',
	'S_NEW_INSTALLATION'=>			'New installation',
	'S_NEW_INSTALLATION_BIG'=>			'NEW INSTALLATION',
	'S_INSTALLATION_UPDATE'=>			'Installation/Update',
	'S_TIME_ZONE'=>			'Time zone',
	'S_DO_NOT_KEEP_HISTORY_OLDER_THAN'=>			'Do not keep history older than (in days)',
	'S_DO_NOT_KEEP_TRENDS_OLDER_THAN'=>			'Do not keep trends older than (in days)',
	'S_MASTER_NODE'=>			'Master node',
	'S_REMOTE'=>			'Remote',
	'S_MASTER'=>			'Master',
	'S_NODE_UPDATED'=>			'Node updated',
	'S_CANNOT_UPDATE_NODE'=>			'Cannot update node',
	'S_NODE_ADDED'=>			'Node added',
	'S_CANNOT_ADD_NODE'=>			'Cannot add node',
	'S_NODE_DELETED'=>			'Node deleted',
	'S_CANNOT_DELETE_NODE'=>			'Cannot delete node',
	'S_CURRENT_NODE'=>			'Current node',
	'S_CURRENT_NODE_ONLY'=>			'Current node only',
	'S_WITH_SUBNODES'=>			'With subnodes',
	'S_ACKNOWLEDGES'=>			'Igazolások',
	'S_ACKNOWLEDGE'=>			'Igazolás',
	'S_ACKNOWLEDGE_ALARM_BY'=>			'Acknowledge alarm by',
	'S_ADD_COMMENT_BY'=>			'Add comment by',
	'S_COMMENT_ADDED'=>			'Comment added',
	'S_CANNOT_ADD_COMMENT'=>			'Cannot add coment',
	'S_ALARM_ACKNOWLEDGES_BIG'=>			'ALARM ACKNOWLEDGES',
	'S_ACKNOWLEDGE_ADDED'=>			'Acknowledge added',
	'S_CONFIGURATION_OF_ACTIONS'=>			'Eljárás beállítása',
	'S_CONFIGURATION_OF_ACTIONS_BIG'=>			'ELJÁRÁS BEÁLLÍTÁSA',
	'S_OPERATION_TYPE'=>			'Operation type',
	'S_SEND_MESSAGE'=>			'Send message',
	'S_REMOTE_COMMAND'=>			'Remote command',
	'S_REMOTE_COMMANDS'=>			'Remote commands',
	'S_FILTER'=>			'Filter',
	'S_TRIGGER_SEVERITY'=>			'Trigger severity',
	'S_TRIGGER_VALUE'=>			'Trigger value',
	'S_TIME_PERIOD'=>			'Time period',
	'S_TRIGGER_DESCRIPTION'=>			'Trigger description',
	'S_CONDITIONS'=>			'Feltételek',
	'S_CONDITION'=>			'Feltétel',
	'S_NEW_CONDITION'=>			'Új feltétel',
	'S_OPERATIONS'=>			'Műveletek',
	'S_EDIT_OPERATION'=>			'Művelet szerkesztése',
	'S_NO_CONDITIONS_DEFINED'=>			'Nincs feltétel meghatározva',
	'S_ACTIONS_DELETED'=>			'Eljárás törölve',
	'S_CANNOT_DELETE_ACTIONS'=>			'Az eljárás nem törölhető',
	'S_NO_OPERATIONS_DEFINED'=>			'Nincs meghatározott művelet',
	'S_NEW'=>			'Új',
	'S_ADD_HOST'=>			'Új host',
	'S_REMOVE_HOST'=>			'Host törlése',
	'S_LINK_TO_TEMPLATE'=>			'Kapcsolás a mintához',
	'S_UNLINK_FROM_TEMPLATE'=>			'Lekapcsolás a mintáról',
	'S_INCORRECT_TRIGGER'=>			'Hibás trigger',
	'S_INCORRECT_HOST'=>			'Hibás host',
	'S_INCORRECT_PERIOD'=>			'Hibás period',
	'S_INCORRECT_IP'=>			'Hibás ip',
	'S_INCORRECT_DISCOVERY_CHECK'=>			'Incorrect discovery check',
	'S_INCORRECT_PORT'=>			'Incorrect port',
	'S_INCORRECT_DISCOVERY_STATUS'=>			'Incorrect discovery status',
	'S_INCORRECT_CONDITION_TYPE'=>			'Incorrect condition type',
	'S_INCORRECT_OPERATION_TYPE'=>			'Incorrect operation type',
	'S_INCORRECT_USER'=>			'Incorrect user',
	'S_ACTIONS'=>			'Eljárások',
	'S_ACTIONS_BIG'=>			'ELJÁRÁSOK',
	'S_ACTION_ADDED'=>			'Action added',
	'S_CANNOT_ADD_ACTION'=>			'Cannot add action',
	'S_ACTION_UPDATED'=>			'Action updated',
	'S_CANNOT_UPDATE_ACTION'=>			'Cannot update action',
	'S_ACTION_DELETED'=>			'Action deleted',
	'S_CANNOT_DELETE_ACTION'=>			'Cannot delete action',
	'S_SEND_MESSAGE_TO'=>			'Send message to',
	'S_RUN_REMOTE_COMMANDS'=>			'Run remote commands',
	'S_DELAY'=>			'Delay',
	'S_SUBJECT'=>			'Subject',
	'S_ON'=>			'BE',
	'S_OFF'=>			'KI',
	'S_NO_ACTIONS_DEFINED'=>			'Nincs létező eljárás',
	'S_SINGLE_USER'=>			'Single user',
	'S_USER_GROUP'=>			'Felhasználói csoport',
	'S_GROUP'=>			'Csoport',
	'S_USER'=>			'Felhasználó',
	'S_MESSAGE'=>			'Üzenet',
	'S_NOT_CLASSIFIED'=>			'Nincs osztályozva',
	'S_INFORMATION'=>			'Informatív',
	'S_WARNING'=>			'Figyelmezetés',
	'S_AVERAGE'=>			'Átlagos',
	'S_HIGH'=>			'Magas',
	'S_DISASTER'=>			'Katasztrofális',
	'S_AND_OR_BIG'=>			'AND / OR',
	'S_AND_BIG'=>			'AND',
	'S_AND'=>			'and',
	'S_OR_BIG'=>			'OR',
	'S_OR'=>			'or',
	'S_TYPE_OF_CALCULATION'=>			'Képzés tipusa',
	'S_CREATE_ACTION'=>			'Új eljárás',
	'S_ENABLE_SELECTED_ACTIONS_Q'=>			'Engedélyezi a kiválasztott eljárást?',
	'S_DISABLE_SELECTED_ACTIONS_Q'=>			'Tiltja a kiválasztott eljárást?',
	'S_DELETE_SELECTED_ACTIONS_Q'=>			'Törli a kiválasztott eljárásokat?',
	'S_DELETE_SELECTED_ACTION_Q'=>			'Törli a kiválasztott eljárást?',
	'S_LIKE_SMALL'=>			'like',
	'S_NOT_LIKE_SMALL'=>			'not like',
	'S_IN_SMALL'=>			'in',
	'S_NOT_IN_SMALL'=>			'not in',
	'S_SHOW_ALL'=>			'Mutasd mindet',
	'S_TIME'=>			'Idő',
	'S_STATUS'=>			'Státusz',
	'S_DURATION'=>			'Időtartam',
	'S_UNKNOWN_BIG'=>			'ISMERETLEN',
	'S_TYPE'=>			'Típus',
	'S_RECIPIENTS'=>			'Recipient(s)',
	'S_ERROR'=>			'Hiba',
	'S_SENT'=>			'küldve',
	'S_NOT_SENT'=>			'nincs küldve',
	'S_NO_ACTIONS_FOUND'=>			'No actions found',
	'S_CUSTOM_GRAPHS'=>			'Custom graphs',
	'S_GRAPHS_BIG'=>			'GRAPHS',
	'S_SELECT_GRAPH_TO_DISPLAY'=>			'Select graph to display',
	'S_PERIOD'=>			'Period',
	'S_MOVE'=>			'Move',
	'S_NAVIGATE'=>			'Navigate',
	'S_INCREASE'=>			'Increase',
	'S_DECREASE'=>			'Decrease',
	'S_RIGHT_DIR'=>			'Jobb',
	'S_LEFT_DIR'=>			'Bal',
	'S_SELECT_GRAPH_DOT_DOT_DOT'=>			'Select graph...',
	'S_CANNNOT_UPDATE_VALUE_MAP'=>			'Cannot update value map',
	'S_VALUE_MAP_ADDED'=>			'Value map added',
	'S_CANNNOT_ADD_VALUE_MAP'=>			'Cannot add value map',
	'S_VALUE_MAP_DELETED'=>			'Value map deleted',
	'S_CANNNOT_DELETE_VALUE_MAP'=>			'Cannot delete value map',
	'S_VALUE_MAP_UPDATED'=>			'Value map updated',
	'S_VALUE_MAPPING_BIG'=>			'VALUE MAPPING',
	'S_VALUE_MAPPING'=>			'Value mapping',
	'S_VALUE_MAP'=>			'Value map',
	'S_MAPPING'=>			'Mapping',
	'S_NEW_MAPPING'=>			'New mapping',
	'S_NO_MAPPING_DEFINED'=>			'No mapping defined',
	'S_CREATE_VALUE_MAP'=>			'Create value map',
	'S_CONFIGURATION_OF_ZABBIX'=>			'Configuration of ZABBIX',
	'S_CONFIGURATION_OF_ZABBIX_BIG'=>			'CONFIGURATION OF ZABBIX',
	'S_CONFIGURATION_UPDATED'=>			'Configuration updated',
	'S_CONFIGURATION_WAS_NOT_UPDATED'=>			'Configuration was not updated',
	'S_ADDED_NEW_MEDIA_TYPE'=>			'Added new media type',
	'S_NEW_MEDIA_TYPE_WAS_NOT_ADDED'=>			'New media type was not added',
	'S_MEDIA_TYPE_UPDATED'=>			'Media type updated',
	'S_MEDIA_TYPE_WAS_NOT_UPDATED'=>			'Media type was not updated',
	'S_MEDIA_TYPE_DELETED'=>			'Media type deleted',
	'S_MEDIA_TYPE_WAS_NOT_DELETED'=>			'Media type was not deleted',
	'S_CONFIGURATION'=>			'Konfiguráció',
	'S_ADMINISTRATION'=>			'Adminisztráció',
	'S_DO_NOT_KEEP_ACTIONS_OLDER_THAN'=>			'Do not keep actions older than (in days)',
	'S_DO_NOT_KEEP_EVENTS_OLDER_THAN'=>			'Do not keep events older than (in days)',
	'S_NO_MEDIA_TYPES_DEFINED'=>			'No media types defined',
	'S_SMTP_SERVER'=>			'SMTP server',
	'S_SMTP_HELO'=>			'SMTP helo',
	'S_SMTP_EMAIL'=>			'SMTP email',
	'S_SCRIPT_NAME'=>			'Script name',
	'S_DELETE_SELECTED_MEDIA'=>			'Delete selected media?',
	'S_DELETE_SELECTED_IMAGE'=>			'Delete selected image?',
	'S_HOUSEKEEPER'=>			'Házfelügyelő',
	'S_MEDIA_TYPES'=>			'Értesítési módok',
	'S_ESCALATION_RULES'=>			'Escalation rules',
	'S_DEFAULT'=>			'Default',
	'S_IMAGES'=>			'Images',
	'S_IMAGE'=>			'Image',
	'S_IMAGES_BIG'=>			'IMAGES',
	'S_ICON'=>			'Icon',
	'S_NO_IMAGES_DEFINED'=>			'No images defined',
	'S_BACKGROUND'=>			'Background',
	'S_UPLOAD'=>			'Upload',
	'S_IMAGE_ADDED'=>			'Image added',
	'S_CANNOT_ADD_IMAGE'=>			'Cannot add image',
	'S_IMAGE_DELETED'=>			'Image deleted',
	'S_CANNOT_DELETE_IMAGE'=>			'Cannot delete image',
	'S_IMAGE_UPDATED'=>			'Image updated',
	'S_CANNOT_UPDATE_IMAGE'=>			'Cannot update image',
	'S_OTHER'=>			'Other',
	'S_OTHER_PARAMETERS'=>			'Other parameters',
	'S_REFRESH_UNSUPPORTED_ITEMS'=>			'Refresh unsupported items (in sec)',
	'S_CREATE_MEDIA_TYPE'=>			'Új értesítési mód',
	'S_CREATE_IMAGE'=>			'Create Image',
	'S_WORKING_TIME'=>			'Munkaidő',
	'S_USER_GROUP_FOR_DATABASE_DOWN_MESSAGE'=>			'User group for database down message',
	'S_INCORRECT_GROUP'=>			'Érvénytelen csoport',
	'S_NOTHING_TO_DO'=>			'Nincs feladat',
	'S_INCORRECT_WORK_PERIOD'=>			'Icorrect work period',
	'S_NODE'=>			'Node',
	'S_NODES'=>			'Nodes',
	'S_NODES_BIG'=>			'NODES',
	'S_NEW_NODE'=>			'New node',
	'S_NO_NODES_DEFINED'=>			'No nodes defined',
	'S_NO_PERMISSIONS'=>			'No permissions !',
	'S_LATEST_DATA_BIG'=>			'AKTUÁLIS ADATOK',
	'S_ALL_SMALL'=>			'mind',
	'S_MINUS_OTHER_MINUS'=>			'- egyéb -',
	'S_GRAPH'=>			'Grafikon',
	'S_COPYRIGHT_BY'=>			'Copyright 2001-2007 by',
	'S_CONNECTED_AS'=>			'Connected as',
	'S_SIA_ZABBIX'=>			'SIA Zabbix',
	'S_ITEM_ADDED'=>			'Item added',
	'S_ITEM_UPDATED'=>			'Item updated',
	'S_ITEMS_UPDATED'=>			'Items updated',
	'S_PARAMETER'=>			'Parameter',
	'S_COLOR'=>			'Color',
	'S_UP'=>			'Fut',
	'S_DOWN'=>			'Áll',
	'S_NEW_ITEM_FOR_THE_GRAPH'=>			'New item for the graph',
	'S_SORT_ORDER_0_100'=>			'Sort order (0->100)',
	'S_YAXIS_SIDE'=>			'Y axis side',
	'S_LEFT'=>			'Balra',
	'S_FUNCTION'=>			'Function',
	'S_MIN_SMALL'=>			'min',
	'S_AVG_SMALL'=>			'avg',
	'S_MAX_SMALL'=>			'max',
	'S_DRAW_STYLE'=>			'Draw style',
	'S_SIMPLE'=>			'Simple',
	'S_GRAPH_TYPE'=>			'Graph type',
	'S_STACKED'=>			'Stacked',
	'S_NORMAL'=>			'Normal',
	'S_AGGREGATED'=>			'Aggregated',
	'S_AGGREGATED_PERIODS_COUNT'=>			'Aggregated periods count',
	'S_CONFIGURATION_OF_GRAPHS'=>			'Configuration of graphs',
	'S_CONFIGURATION_OF_GRAPHS_BIG'=>			'CONFIGURATION OF GRAPHS',
	'S_GRAPH_ADDED'=>			'Graph added',
	'S_GRAPH_UPDATED'=>			'Graph updated',
	'S_CANNOT_UPDATE_GRAPH'=>			'Cannot update graph',
	'S_GRAPH_DELETED'=>			'Graph deleted',
	'S_CANNOT_DELETE_GRAPH'=>			'Cannot delete graph',
	'S_CANNOT_ADD_GRAPH'=>			'Cannot add graph',
	'S_ID'=>			'Id',
	'S_NO_GRAPHS_DEFINED'=>			'No graphs defined',
	'S_NO_GRAPH_DEFINED'=>			'No graph defined',
	'S_DELETE_GRAPH_Q'=>			'Delete graph?',
	'S_YAXIS_TYPE'=>			'Y axis type',
	'S_YAXIS_MIN_VALUE'=>			'Y axis MIN value',
	'S_YAXIS_MAX_VALUE'=>			'Y axis MAX value',
	'S_CALCULATED'=>			'Calculated',
	'S_FIXED'=>			'Fixed',
	'S_CREATE_GRAPH'=>			'Create Graph',
	'S_SHOW_WORKING_TIME'=>			'Show working time',
	'S_SHOW_TRIGGERS'=>			'Show triggers',
	'S_GRAPH_ITEM'=>			'Graph item',
	'S_REQUIRED_ITEMS_FOR_GRAPH'=>			'Required items for graph',
	'S_LAST_HOUR_GRAPH'=>			'Last hour graph',
	'S_LAST_WEEK_GRAPH'=>			'Last week graph',
	'S_LAST_MONTH_GRAPH'=>			'Last month graph',
	'S_500_LATEST_VALUES'=>			'500 latest values',
	'S_TIMESTAMP'=>			'Timestamp',
	'S_LOCAL'=>			'Local',
	'S_SOURCE'=>			'Source',
	'S_SHOW_UNKNOWN'=>			'Show unknown',
	'S_SHOW_SELECTED'=>			'Show selected',
	'S_HIDE_SELECTED'=>			'Hide selectede',
	'S_MARK_SELECTED'=>			'Mark selected',
	'S_MARK_OTHERS'=>			'Mark others',
	'S_AS_RED'=>			'as Red',
	'S_AS_GREEN'=>			'as Green',
	'S_AS_BLUE'=>			'as Blue',
	'S_APPLICATION'=>			'Application',
	'S_APPLICATIONS'=>			'Applications',
	'S_APPLICATIONS_BIG'=>			'APPLICATIONS',
	'S_CREATE_APPLICATION'=>			'Create application',
	'S_DELETE_SELECTED_APPLICATIONS_Q'=>			'Delete selected applications?',
	'S_DISABLE_ITEMS_FROM_SELECTED_APPLICATIONS_Q'=>			'Disable items from selected applications?',
	'S_ACTIVATE_ITEMS'=>			'Activate Items',
	'S_DISABLE_ITEMS'=>			'Disable Items',
	'S_ACTIVATE_ITEMS_FROM_SELECTED_APPLICATIONS_Q'=>			'Activate items from selected applications?',
	'S_APPLICATION_UPDATED'=>			'Application updated',
	'S_CANNOT_UPDATE_APPLICATION'=>			'Cannot update application',
	'S_APPLICATION_ADDED'=>			'Application added',
	'S_CANNOT_ADD_APPLICATION'=>			'Cannot add application',
	'S_APPLICATION_DELETED'=>			'Application deleted',
	'S_CANNOT_DELETE_APPLICATION'=>			'Cannot delete application',
	'S_NO_APPLICATIONS_DEFINED'=>			'No applications defined',
	'S_HOSTS'=>			'Hosts',
	'S_ITEMS'=>			'Adat sorok',
	'S_ITEMS_BIG'=>			'ADAT SOROK',
	'S_TRIGGERS'=>			'Trigger-ek',
	'S_GRAPHS'=>			'Grafikonok',
	'S_HOST_ADDED'=>			'Host added',
	'S_CANNOT_ADD_HOST'=>			'Cannot add host',
	'S_HOST_UPDATED'=>			'Host updated',
	'S_CANNOT_UPDATE_HOST'=>			'Cannot update host',
	'S_HOST_STATUS_UPDATED'=>			'Host status updated',
	'S_CANNOT_UPDATE_HOST_STATUS'=>			'Cannot update host status',
	'S_HOST_DELETED'=>			'Host deleted',
	'S_CANNOT_DELETE_HOST'=>			'Cannot delete host',
	'S_CONFIGURATION_OF_HOSTS_GROUPS_AND_TEMPLATES'=>			'CONFIGURATION OF HOSTS, GROUPS AND TEMPLATES',
	'S_HOST_GROUPS_BIG'=>			'HOST GROUPS',
	'S_NO_HOST_GROUPS_DEFINED'=>			'No host groups defined',
	'S_NO_HOSTS_DEFINED'=>			'No hosts defined',
	'S_NO_TEMPLATES_DEFINED'=>			'No templates defined',
	'S_HOSTS_BIG'=>			'HOSTS',
	'S_HOST'=>			'Host',
	'S_CONNECT_TO'=>			'Kapcsolódva',
	'S_DNS'=>			'DNS',
	'S_IP'=>			'IP',
	'S_PORT'=>			'Port',
	'S_MONITORED'=>			'Felügyelt',
	'S_NOT_MONITORED'=>			'Nem felügyelt',
	'S_TEMPLATE'=>			'Minta',
	'S_DELETED'=>			'Törölve',
	'S_UNKNOWN'=>			'Ismeretlen',
	'S_GROUPS'=>			'Csoportok',
	'S_NO_GROUPS_DEFINED'=>			'No groups defined',
	'S_NEW_GROUP'=>			'New group',
	'S_DNS_NAME'=>			'DNS name',
	'S_IP_ADDRESS'=>			'IP address',
	'S_LINK_WITH_TEMPLATE'=>			'Link with Template',
	'S_USE_PROFILE'=>			'Use profile',
	'S_DELETE_SELECTED_HOST_Q'=>			'Delete selected host?',
	'S_DELETE_SELECTED_GROUPS_Q'=>			'Delete selected groups?',
	'S_DELETE_SELECTED_WITH_LINKED_ELEMENTS'=>			'Delete selected with linked elements',
	'S_GROUP_NAME'=>			'Group name',
	'S_HOST_GROUP'=>			'Host group',
	'S_HOST_GROUPS'=>			'Host groups',
	'S_UPDATE'=>			'Update',
	'S_AVAILABILITY'=>			'Elérhetőség',
	'S_AVAILABLE'=>			'Elérhető',
	'S_NOT_AVAILABLE'=>			'Nem elérhető',
	'S_HOST_PROFILE'=>			'Host profile',
	'S_DEVICE_TYPE'=>			'Device type',
	'S_OS'=>			'OS',
	'S_SERIALNO'=>			'SerialNo',
	'S_TAG'=>			'Tag',
	'S_HARDWARE'=>			'Hardware',
	'S_SOFTWARE'=>			'Software',
	'S_CONTACT'=>			'Contact',
	'S_LOCATION'=>			'Location',
	'S_NOTES'=>			'Notes',
	'S_MACADDRESS'=>			'MAC Address',
	'S_ADD_TO_GROUP'=>			'Add to group',
	'S_DELETE_FROM_GROUP'=>			'Delete from group',
	'S_UPDATE_IN_GROUP'=>			'Update in group',
	'S_DELETE_SELECTED_HOSTS_Q'=>			'Delete selected hosts?',
	'S_DISABLE_SELECTED_HOSTS_Q'=>			'Disable selected hosts?',
	'S_ACTIVATE_SELECTED_HOSTS_Q'=>			'Activate selected hosts?',
	'S_CREATE_HOST'=>			'Create Host',
	'S_CREATE_TEMPLATE'=>			'Create Template',
	'S_TEMPLATE_LINKAGE'=>			'Template linkage',
	'S_TEMPLATE_LINKAGE_BIG'=>			'TEMPLATE LINKAGE',
	'S_NO_LINKAGES'=>			'No Linkages',
	'S_TEMPLATES'=>			'Minták',
	'S_TEMPLATES_BIG'=>			'MINTÁK',
	'S_UNLINK'=>			'Unlink',
	'S_UNLINK_AND_CLEAR'=>			'Unlink and clear',
	'S_NO_ITEMS_DEFINED'=>			'No items defined',
	'S_NO_ITEM_DEFINED'=>			'No item defined',
	'S_HISTORY_CLEANED'=>			'History cleaned',
	'S_CLEAN_HISTORY_SELECTED_ITEMS'=>			'Clean history selected items',
	'S_CLEAN_HISTORY'=>			'Clean history',
	'S_CANNOT_CLEAN_HISTORY'=>			'Cannot clean history',
	'S_CONFIGURATION_OF_ITEMS'=>			'Configuration of items',
	'S_CONFIGURATION_OF_ITEMS_BIG'=>			'CONFIGURATION OF ITEMS',
	'S_CANNOT_UPDATE_ITEM'=>			'Cannot update item',
	'S_STATUS_UPDATED'=>			'Status updated',
	'S_CANNOT_UPDATE_STATUS'=>			'Cannot update status',
	'S_CANNOT_ADD_ITEM'=>			'Cannot add item',
	'S_ITEM_DELETED'=>			'Item deleted',
	'S_CANNOT_DELETE_ITEM'=>			'Cannot delete item',
	'S_ITEMS_DELETED'=>			'Items deleted',
	'S_CANNOT_DELETE_ITEMS'=>			'Cannot delete items',
	'S_ITEMS_ACTIVATED'=>			'Items activated',
	'S_ITEMS_DISABLED'=>			'Items disabled',
	'S_KEY'=>			'Kulcs',
	'S_DESCRIPTION'=>			'Leírás',
	'S_UPDATE_INTERVAL'=>			'Update interval',
	'S_HISTORY'=>			'History',
	'S_TRENDS'=>			'Trends',
	'S_ZABBIX_AGENT'=>			'ZABBIX agent',
	'S_ZABBIX_AGENT_ACTIVE'=>			'ZABBIX agent (active)',
	'S_SNMPV1_AGENT'=>			'SNMPv1 agent',
	'S_ZABBIX_TRAPPER'=>			'ZABBIX trapper',
	'S_SIMPLE_CHECK'=>			'Simple check',
	'S_SNMPV2_AGENT'=>			'SNMPv2 agent',
	'S_SNMPV3_AGENT'=>			'SNMPv3 agent',
	'S_ZABBIX_INTERNAL'=>			'ZABBIX internal',
	'S_ZABBIX_AGGREGATE'=>			'ZABBIX aggregate',
	'S_EXTERNAL_CHECK'=>			'Külső ellenőrzés',
	'S_WEB_MONITORING'=>			'Web felügyelet',
	'S_ACTIVE'=>			'Aktív',
	'S_NOT_SUPPORTED'=>			'Nem támogatott',
	'S_ACTIVATE_SELECTED_ITEMS_Q'=>			'Aktiválja a kiválasztott elemeket?',
	'S_DISABLE_SELECTED_ITEMS_Q'=>			'Tiltja a kiválasztott elemeket?',
	'S_DELETE_SELECTED_ITEMS_Q'=>			'Törli a kiválasztott elemeket?',
	'S_EMAIL'=>			'Email',
	'S_JABBER'=>			'Jabber',
	'S_JABBER_IDENTIFIER'=>			'Jabber Azonosító',
	'S_SMS'=>			'SMS',
	'S_SCRIPT'=>			'Script',
	'S_GSM_MODEM'=>			'GSM modem',
	'S_UNITS'=>			'Units',
	'S_UPDATE_INTERVAL_IN_SEC'=>			'Update interval (in sec)',
	'S_KEEP_HISTORY_IN_DAYS'=>			'Keep history (in days)',
	'S_KEEP_TRENDS_IN_DAYS'=>			'Keep trends (in days)',
	'S_TYPE_OF_INFORMATION'=>			'Type of information',
	'S_STORE_VALUE'=>			'Store value',
	'S_SHOW_VALUE'=>			'Show value',
	'S_NUMERIC_UNSIGNED'=>			'Numeric (integer 64bit)',
	'S_NUMERIC_FLOAT'=>			'Numeric (float)',
	'S_CHARACTER'=>			'Character',
	'S_LOG'=>			'Log',
	'S_TEXT'=>			'Text',
	'S_AS_IS'=>			'Ahogy van',
	'S_DELTA_SPEED_PER_SECOND'=>			'Delta (speed per second)',
	'S_DELTA_SIMPLE_CHANGE'=>			'Delta (simple change)',
	'S_ITEM'=>			'Elem',
	'S_SNMP_COMMUNITY'=>			'SNMP community',
	'S_SNMP_OID'=>			'SNMP OID',
	'S_SNMP_PORT'=>			'SNMP port',
	'S_ALLOWED_HOSTS'=>			'Engedélyezett hosts',
	'S_SNMPV3_SECURITY_NAME'=>			'SNMPv3 security name',
	'S_SNMPV3_SECURITY_LEVEL'=>			'SNMPv3 security level',
	'S_SNMPV3_AUTH_PASSPHRASE'=>			'SNMPv3 auth passphrase',
	'S_SNMPV3_PRIV_PASSPHRASE'=>			'SNMPv3 priv passphrase',
	'S_CUSTOM_MULTIPLIER'=>			'Custom multiplier',
	'S_DO_NOT_USE'=>			'Ne használja',
	'S_USE_MULTIPLIER'=>			'Use multiplier',
	'S_LOG_TIME_FORMAT'=>			'Log time format',
	'S_CREATE_ITEM'=>			'Create Item',
	'S_X_ELEMENTS_COPY_TO_DOT_DOT_DOT'=>			'elements copy to ...',
	'S_MODE'=>			'Mode',
	'S_TARGET'=>			'Target',
	'S_TARGET_TYPE'=>			'Target type',
	'S_SKIP_EXISTING_ITEMS'=>			'Skip existing items',
	'S_UPDATE_EXISTING_NON_LINKED_ITEMS'=>			'update existing non linked items',
	'S_COPY'=>			'Copy',
	'S_SHOW_ITEMS_WITH_DESCRIPTION_LIKE'=>			'Mutasd azokat melyek leírásában szerepel a(z)...',
	'S_SHOW_DISABLED_ITEMS'=>			'Show disabled items',
	'S_HIDE_DISABLED_ITEMS'=>			'Hide disabled items',
	'S_HISTORY_CLEANING_CAN_TAKE_A_LONG_TIME_CONTINUE_Q'=>			'History cleaning can take a long time. Continue?',
	'S_SELECTION_MODE'=>			'Selection mode',
	'S_ADVANCED'=>			'Advanced',
	'S_MASS_UPDATE'=>			'Mass update',
	'S_ORIGINAL'=>			'Original',
	'S_NEW_FLEXIBLE_INTERVAL'=>			'New flexible interval',
	'S_FLEXIBLE_INTERVALS'=>			'Flexible intervals (sec)',
	'S_LATEST_EVENTS'=>			'Latest events',
	'S_HISTORY_OF_EVENTS_BIG'=>			'HISTORY OF EVENTS',
	'S_NO_EVENTS_FOUND'=>			'No events found',
	'S_LAST_CHECK'=>			'Ellenőrzés ideje',
	'S_LAST_VALUE'=>			'Mért érték',
	'S_LINK'=>			'Link',
	'S_LABEL'=>			'Label',
	'S_X'=>			'X',
	'S_Y'=>			'Y',
	'S_ICON_UNKNOWN'=>			'Icon (unknown)',
	'S_ELEMENT_1'=>			'Element 1',
	'S_ELEMENT_2'=>			'Element 2',
	'S_LINK_STATUS_INDICATOR'=>			'Link status indicator',
	'S_CONFIGURATION_OF_NETWORK_MAPS'=>			'CONFIGURATION OF NETWORK MAPS',
	'S_MAPS_BIG'=>			'MAPS',
	'S_NO_MAPS_DEFINED'=>			'No maps defined',
	'S_CREATE_MAP'=>			'Create Map',
	'S_ICON_LABEL_LOCATION'=>			'Icon label location',
	'S_BOTTOM'=>			'Aljára',
	'S_TOP'=>			'Felülre',
	'S_OK_BIG'=>			'OK',
	'S_ZABBIX_URL'=>			'http://www.zabbix.com',
	'S_NETWORK_MAPS'=>			'Network maps',
	'S_NETWORK_MAPS_BIG'=>			'NETWORK MAPS',
	'S_BACKGROUND_IMAGE'=>			'Background image',
	'S_ICON_LABEL_TYPE'=>			'Icon label type',
	'S_LABEL_LOCATION'=>			'Label location',
	'S_ELEMENT_NAME'=>			'Element name',
	'S_STATUS_ONLY'=>			'Status only',
	'S_NOTHING'=>			'Nothing',
	'S_CONFIGURATION_OF_MEDIA_TYPES_BIG'=>			'ÉRTESÍTÉSI MÓDOK BEÁLLÍTÁSAI',
	'S_MEDIA'=>			'Értesítési mód',
	'S_SEND_TO'=>			'Küld ..',
	'S_WHEN_ACTIVE'=>			'Amikor aktív',
	'S_NO_MEDIA_DEFINED'=>			'Nincs értesítési mód definiálva',
	'S_NEW_MEDIA'=>			'Új értesítési mód',
	'S_USE_IF_SEVERITY'=>			'Használja ha komoly',
	'S_SAVE'=>			'Mentés',
	'S_CANCEL'=>			'Mégse',
	'S_OVERVIEW'=>			'Áttekintés',
	'S_OVERVIEW_BIG'=>			'ÁTTEKINTÉS',
	'S_DATA'=>			'Adat',
	'S_SHOW_GRAPH_OF_ITEM'=>			'Show graph of item',
	'S_SHOW_VALUES_OF_ITEM'=>			'Show values of item',
	'S_VALUES'=>			'Értékek',
	'S_5_MIN'=>			'5 perc',
	'S_15_MIN'=>			'15 perc',
	'S_QUEUE_BIG'=>			'QUEUE',
	'S_QUEUE_OF_ITEMS_TO_BE_UPDATED_BIG'=>			'QUEUE OF ITEMS TO BE UPDATED',
	'S_NEXT_CHECK'=>			'Next check',
	'S_THE_QUEUE_IS_EMPTY'=>			'The queue is empty',
	'S_TOTAL'=>			'Total',
	'S_COUNT'=>			'Count',
	'S_5_SECONDS'=>			'5 seconds',
	'S_10_SECONDS'=>			'10 seconds',
	'S_30_SECONDS'=>			'30 seconds',
	'S_1_MINUTE'=>			'1 minute',
	'S_5_MINUTES'=>			'5 minutes',
	'S_STATUS_OF_ZABBIX'=>			'ZABBIX állapot',
	'S_STATUS_OF_ZABBIX_BIG'=>			'ZABBIX ÁLLAPOT',
	'S_VALUE'=>			'Value',
	'S_ZABBIX_SERVER_IS_RUNNING'=>			'ZABBIX server is running',
	'S_VALUES_STORED'=>			'Values stored',
	'S_TRENDS_STORED'=>			'Trends stored',
	'S_NUMBER_OF_EVENTS'=>			'Number of events',
	'S_NUMBER_OF_ALERTS'=>			'Number of alerts',
	'S_NUMBER_OF_TRIGGERS'=>			'Number of triggers (enabled/disabled)[true/unknown/false]',
	'S_NUMBER_OF_TRIGGERS_SHORT'=>			'Triggers (e/d)[t/u/f]',
	'S_NUMBER_OF_ITEMS'=>			'Number of items (monitored/disabled/not supported)[trapper]',
	'S_NUMBER_OF_ITEMS_SHORT'=>			'Items (m/d/n)[t]',
	'S_NUMBER_OF_USERS'=>			'Number of users (online)',
	'S_NUMBER_OF_USERS_SHORT'=>			'Users (online)',
	'S_NUMBER_OF_HOSTS'=>			'Number of hosts (monitored/not monitored/templates/deleted)',
	'S_NUMBER_OF_HOSTS_SHORT'=>			'Hosts (m/n/t/d)',
	'S_YES'=>			'Igen',
	'S_NO'=>			'Nem',
	'S_RUNNING'=>			'fut',
	'S_NOT_RUNNING'=>			'nem fut',
	'S_AVAILABILITY_REPORT'=>			'Létező jelentések',
	'S_AVAILABILITY_REPORT_BIG'=>			'LÉTEZŐ JELENTÉSEK',
	'S_SHOW'=>			'Mutasd',
	'S_TRUE'=>			'Igaz',
	'S_FALSE'=>			'Hamis',
	'S_IT_SERVICES_AVAILABILITY_REPORT'=>			'IT services availability report',
	'S_IT_SERVICES_AVAILABILITY_REPORT_BIG'=>			'IT SERVICES AVAILABILITY REPORT',
	'S_FROM'=>			'-tól',
	'S_FROM_SMALL'=>			'-tól',
	'S_TILL'=>			'-Ig',
	'S_OK'=>			'Ok',
	'S_PROBLEMS'=>			'Problémák',
	'S_PERCENTAGE'=>			'Százalék',
	'S_SLA'=>			'SLA',
	'S_DAY'=>			'Nap',
	'S_MONTH'=>			'Hónap',
	'S_YEAR'=>			'Év',
	'S_DAILY'=>			'Napi',
	'S_WEEKLY'=>			'Heti',
	'S_MONTHLY'=>			'Havi',
	'S_YEARLY'=>			'Évi',
	'S_NOTIFICATIONS'=>			'Értesítések',
	'S_NOTIFICATIONS_BIG'=>			'ÉRTESÍTÉSEK',
	'S_IT_NOTIFICATIONS'=>			'Értesítési beszámolók',
	'S_TRIGGERS_TOP_100'=>			'Leggyakoribb trigger-ek (top 100)',
	'S_TRIGGERS_TOP_100_BIG'=>			'LEGGYAKORIBB TRIGGER-EK (TOP 100)',
	'S_NUMBER_OF_STATUS_CHANGES'=>			'Állapotváltozások száma',
	'S_WEEK'=>			'Hét',
	'S_LAST'=>			'Korábbi',
	'S_SCREENS'=>			'Screens',
	'S_SCREEN'=>			'Screen',
	'S_CONFIGURATION_OF_SCREENS_BIG'=>			'CONFIGURATION OF SCREENS',
	'S_CONFIGURATION_OF_SCREENS'=>			'Configuration of screens',
	'S_SCREEN_ADDED'=>			'Screen added',
	'S_CANNOT_ADD_SCREEN'=>			'Cannot add screen',
	'S_SCREEN_UPDATED'=>			'Screen updated',
	'S_CANNOT_UPDATE_SCREEN'=>			'Cannot update screen',
	'S_SCREEN_DELETED'=>			'Screen deleted',
	'S_CANNOT_DELETE_SCREEN'=>			'Cannot deleted screen',
	'S_COLUMNS'=>			'Oszlop',
	'S_ROWS'=>			'Sor',
	'S_NO_SCREENS_DEFINED'=>			'No screens defined',
	'S_DELETE_SCREEN_Q'=>			'Delete screen?',
	'S_CONFIGURATION_OF_SCREEN_BIG'=>			'CONFIGURATION OF SCREEN',
	'S_SCREEN_CELL_CONFIGURATION'=>			'Screen cell configuration',
	'S_RESOURCE'=>			'Resource',
	'S_RIGHTS_OF_RESOURCES'=>			'User rights',
	'S_NO_RESOURCES_DEFINED'=>			'No resources defined',
	'S_SIMPLE_GRAPH'=>			'Simple graph',
	'S_GRAPH_NAME'=>			'Graph name',
	'S_WIDTH'=>			'Width',
	'S_HEIGHT'=>			'Height',
	'S_CREATE_SCREEN'=>			'Create Screen',
	'S_EDIT'=>			'Edit',
	'S_DIMENSION_COLS_ROWS'=>			'Dimension (cols x rows)',
	'S_SLIDESHOWS'=>			'Slideshows',
	'S_SLIDESHOW'=>			'Slideshow',
	'S_CONFIGURATION_OF_SLIDESHOWS_BIG'=>			'CONFIGURATION OF SLIDESHOWS',
	'S_SLIDESHOWS_BIG'=>			'SLIDESHOWS',
	'S_NO_SLIDESHOWS_DEFINED'=>			'No slideshows defined',
	'S_COUNT_OF_SLIDES'=>			'Count of slides',
	'S_NO_SLIDES_DEFINED'=>			'No slides defined',
	'S_SLIDES'=>			'Slides',
	'S_NEW_SLIDE'=>			'New slide',
	'S_MAP'=>			'Map',
	'S_AS_PLAIN_TEXT'=>			'As plain text',
	'S_PLAIN_TEXT'=>			'Plain text',
	'S_COLUMN_SPAN'=>			'Column span',
	'S_ROW_SPAN'=>			'Row span',
	'S_SHOW_LINES'=>			'Show lines',
	'S_HOSTS_INFO'=>			'Hosts info',
	'S_TRIGGERS_INFO'=>			'Triggers info',
	'S_SERVER_INFO'=>			'Server info',
	'S_CLOCK'=>			'Clock',
	'S_TRIGGERS_OVERVIEW'=>			'Triggers overview',
	'S_DATA_OVERVIEW'=>			'Data overview',
	'S_HISTORY_OF_ACTIONS'=>			'History of actions',
	'S_HISTORY_OF_EVENTS'=>			'History of events',
	'S_TIME_TYPE'=>			'Idő típusa',
	'S_SERVER_TIME'=>			'Szerver idő',
	'S_LOCAL_TIME'=>			'Helyi idő',
	'S_STYLE'=>			'Stílus',
	'S_VERTICAL'=>			'Függőleges',
	'S_HORIZONTAL'=>			'Vízszintes',
	'S_HORIZONTAL_ALIGN'=>			'Vízszintes igazítás',
	'S_CENTRE'=>			'Középre',
	'S_RIGHT'=>			'Jobbra',
	'S_VERTICAL_ALIGN'=>			'Függőleges igazítás',
	'S_MIDDLE'=>			'Középre',
	'S_CUSTOM_SCREENS'=>			'Custom screens',
	'S_SCREENS_BIG'=>			'SCREENS',
	'S_SLIDESHOW_UPDATED'=>			'Slideshow updated',
	'S_CANNOT_UPDATE_SLIDESHOW'=>			'Cannot_update slideshow',
	'S_SLIDESHOW_ADDED'=>			'Slideshow added',
	'S_CANNOT_ADD_SLIDESHOW'=>			'Cannot add slideshow',
	'S_SLIDESHOW_DELETED'=>			'Slideshow deleted',
	'S_CANNOT_DELETE_SLIDESHOW'=>			'Cannot delete slideshow',
	'S_DELETE_SLIDESHOW_Q'=>			'Delete slideshow?',
	'S_ROOT_SMALL'=>			'root',
	'S_IT_SERVICE'=>			'IT szolgáltatás',
	'S_IT_SERVICES'=>			'IT szolgáltatások',
	'S_SERVICE_UPDATED'=>			'Szolgáltatás módosítva',
	'S_NO_IT_SERVICE_DEFINED'=>			'No IT service defined',
	'S_CANNOT_UPDATE_SERVICE'=>			'Cannot update service',
	'S_SERVICE_ADDED'=>			'Service added',
	'S_CANNOT_ADD_SERVICE'=>			'Cannot add service',
	'S_SERVICE_DELETED'=>			'Service deleted',
	'S_CANNOT_DELETE_SERVICE'=>			'Cannot delete service',
	'S_STATUS_CALCULATION'=>			'Status calculation',
	'S_STATUS_CALCULATION_ALGORITHM'=>			'Status calculation algorithm',
	'S_NONE'=>			'None',
	'S_SOFT'=>			'Soft',
	'S_DO_NOT_CALCULATE'=>			'Do not calculate',
	'S_ACCEPTABLE_SLA_IN_PERCENT'=>			'Acceptabe SLA (in %)',
	'S_LINK_TO_TRIGGER_Q'=>			'Link to trigger?',
	'S_SORT_ORDER_0_999'=>			'Sort order (0->999)',
	'S_TRIGGER'=>			'Trigger',
	'S_SERVER'=>			'Server',
	'S_DELETE'=>			'Delete',
	'S_CLONE'=>			'Clone',
	'S_UPTIME'=>			'Uptime',
	'S_DOWNTIME'=>			'Downtime',
	'S_ONE_TIME_DOWNTIME'=>			'One-time downtime',
	'S_NO_TIMES_DEFINED'=>			'No times defined',
	'S_SERVICE_TIMES'=>			'Service times',
	'S_NEW_SERVICE_TIME'=>			'New service time',
	'S_NOTE'=>			'Note',
	'S_REMOVE'=>			'Remove',
	'S_DEPENDS_ON'=>			'Depends on',
	'S_SUNDAY'=>			'Vasárnap',
	'S_MONDAY'=>			'Hétfő',
	'S_TUESDAY'=>			'Kedd',
	'S_WEDNESDAY'=>			'Szerda',
	'S_THURSDAY'=>			'Csütörtök',
	'S_FRIDAY'=>			'Péntek',
	'S_SATURDAY'=>			'Szombat',
	'S_IT_SERVICES_BIG'=>			'IT SERVICES',
	'S_SERVICE'=>			'Service',
	'S_SERVICES'=>			'Services',
	'S_REASON'=>			'Reason',
	'S_SLA_LAST_7_DAYS'=>			'SLA (last 7 days)',
	'S_NO_TRIGGER'=>			'Nincs trigger',
	'S_NO_TRIGGERS_DEFINED'=>			'Nincs triggers definiálva',
	'S_NO_TRIGGER_DEFINED'=>			'Nincs trigger definiálva',
	'S_CONFIGURATION_OF_TRIGGERS'=>			'Configuration of triggers',
	'S_CONFIGURATION_OF_TRIGGERS_BIG'=>			'CONFIGURATION OF TRIGGERS',
	'S_TRIGGERS_DELETED'=>			'Triggers deleted',
	'S_CANNOT_DELETE_TRIGGERS'=>			'Cannot delete triggers',
	'S_TRIGGER_DELETED'=>			'Trigger deleted',
	'S_CANNOT_DELETE_TRIGGER'=>			'Cannot delete trigger',
	'S_TRIGGER_ADDED'=>			'Trigger added',
	'S_CANNOT_ADD_TRIGGER'=>			'Cannot add trigger',
	'S_SEVERITY'=>			'Szigorúság',
	'S_EXPRESSION'=>			'Kifejezés',
	'S_DISABLED'=>			'Tiltva',
	'S_ENABLED'=>			'Engedélyezve',
	'S_ENABLE_SELECTED'=>			'Kiválasztott engedélyezése',
	'S_ENABLE_SELECTED_TRIGGERS_Q'=>			'Valóban engedélyezi a kiválasztott triggers?',
	'S_DISABLE_SELECTED_TRIGGERS_Q'=>			'Valóban tiltja a kiválasztott triggers?',
	'S_DELETE_SELECTED_TRIGGERS_Q'=>			'Valóban törli a kiválasztott triggers?',
	'S_CHANGE'=>			'Változás',
	'S_TRIGGER_UPDATED'=>			'Trigger módositva',
	'S_CANNOT_UPDATE_TRIGGER'=>			'Nem lehet módositani a trigger',
	'S_URL'=>			'URL',
	'S_CREATE_TRIGGER'=>			'Új Trigger',
	'S_INSERT'=>			'Insert',
	'S_SECONDS'=>			'Seconds',
	'S_SEC_SMALL'=>			'sec',
	'S_LAST_OF'=>			'Last of',
	'S_SHOW_DISABLED_TRIGGERS'=>			'Mutasd a letiltott triggers',
	'S_HIDE_DISABLED_TRIGGERS'=>			'Rejtsd el a letiltott triggers',
	'S_TRIGGER_COMMENTS'=>			'Trigger comments',
	'S_TRIGGER_COMMENTS_BIG'=>			'TRIGGER COMMENTS',
	'S_COMMENT_UPDATED'=>			'Comment updated',
	'S_CANNOT_UPDATE_COMMENT'=>			'Cannot update comment',
	'S_ADD'=>			'Új',
	'S_STATUS_OF_TRIGGERS'=>			'Status of triggers',
	'S_STATUS_OF_TRIGGERS_BIG'=>			'STATUS OF TRIGGERS',
	'S_SHOW_ACTIONS'=>			'Show actions',
	'S_SHOW_DETAILS'=>			'Show details',
	'S_SELECT'=>			'Select',
	'S_TRIGGERS_BIG'=>			'TRIGGEREK',
	'S_LAST_CHANGE'=>			'Aktuális változás',
	'S_COMMENTS'=>			'Megjegyzés',
	'S_ACKNOWLEDGED'=>			'Igazolva',
	'S_ACK'=>			'Igazolom',
	'S_NEVER'=>			'Soha',
	'S_ZABBIX_USER'=>			'ZABBIX User',
	'S_ZABBIX_ADMIN'=>			'ZABBIX Admin',
	'S_SUPER_ADMIN'=>			'ZABBIX Super Admin',
	'S_USER_TYPE'=>			'Felhasználó típus',
	'S_USERS'=>			'Felhasználók',
	'S_USER_ADDED'=>			'Új felhasználó felvéve',
	'S_CANNOT_ADD_USER'=>			'Cannot add user',
	'S_CANNOT_ADD_USER_BOTH_PASSWORDS_MUST'=>			'Cannot add user. Both passwords must be equal.',
	'S_USER_DELETED'=>			'User deleted',
	'S_CANNOT_DELETE_USER'=>			'Cannot delete user',
	'S_USER_UPDATED'=>			'User updated',
	'S_ONLY_FOR_GUEST_ALLOWED_EMPTY_PASSWORD'=>			'Only user \"guest\" may have an empty password.',
	'S_CANNOT_UPDATE_USER'=>			'Cannot update user',
	'S_CANNOT_UPDATE_USER_BOTH_PASSWORDS'=>			'Cannot update user. Both passwords must be equal.',
	'S_GROUP_ADDED'=>			'Group added',
	'S_CANNOT_ADD_GROUP'=>			'Cannot add group',
	'S_GROUP_UPDATED'=>			'Group updated',
	'S_CANNOT_UPDATE_GROUP'=>			'Cannot update group',
	'S_GROUP_DELETED'=>			'Group deleted',
	'S_CANNOT_DELETE_GROUP'=>			'Cannot delete group',
	'S_CONFIGURATION_OF_USERS_AND_USER_GROUPS'=>			'CONFIGURATION OF USERS AND USER GROUPS',
	'S_USER_GROUPS_BIG'=>			'USER GROUPS',
	'S_USERS_BIG'=>			'USERS',
	'S_USER_GROUPS'=>			'User groups',
	'S_MEMBERS'=>			'Members',
	'S_NO_USER_GROUPS_DEFINED'=>			'No user groups defined',
	'S_ALIAS'=>			'Alias',
	'S_NAME'=>			'Név',
	'S_SURNAME'=>			'Surname',
	'S_IS_ONLINE_Q'=>			'Is online?',
	'S_NO_USERS_DEFINED'=>			'No users defined',
	'S_RIGHTS'=>			'Jogok',
	'S_NO_RIGHTS_DEFINED'=>			'Nincs jog definiálva',
	'S_READ_ONLY'=>			'Csak olvasható',
	'S_READ_WRITE'=>			'Irható/olvasható',
	'S_DENY'=>			'Elutasítva',
	'S_HIDE'=>			'Elrejt',
	'S_PASSWORD'=>			'Jelszó',
	'S_CHANGE_PASSWORD'=>			'Jelszó módosítása',
	'S_PASSWORD_ONCE_AGAIN'=>			'Jelszó (mégegyszer)',
	'S_URL_AFTER_LOGIN'=>			'URL (belépés után)',
	'S_SCREEN_REFRESH'=>			'Frissités (másodpercben)',
	'S_CREATE_USER'=>			'Új felhasználó',
	'S_CREATE_GROUP'=>			'Új csoport',
	'S_DELETE_SELECTED_USERS_Q'=>			'Kiválasztott felhasználó(kat) törli?',
	'S_ACTION'=>			'Eljárás',
	'S_DETAILS'=>			'Details',
	'S_UNKNOWN_ACTION'=>			'Unknown action',
	'S_ADDED'=>			'Added',
	'S_UPDATED'=>			'Updated',
	'S_MEDIA_TYPE'=>			'Media type',
	'S_GRAPH_ELEMENT'=>			'Graph element',
	'S_UNKNOWN_RESOURCE'=>			'Unknown resource',
	'S_USER_PROFILE_BIG'=>			'USER PROFILE',
	'S_USER_PROFILE'=>			'User profile',
	'S_LANGUAGE'=>			'Nyelv',
	'S_ENGLISH_GB'=>			'Angol (GB)',
	'S_FRENCH_FR'=>			'Francia (FR)',
	'S_GERMAN_DE'=>			'Német (DE)',
	'S_ITALIAN_IT'=>			'Olasz (IT)',
	'S_LATVIAN_LV'=>			'Litván (LV)',
	'S_RUSSIAN_RU'=>			'Orosz (RU)',
	'S_SPANISH_SP'=>			'Spanyol (SP)',
	'S_SWEDISH_SE'=>			'Svéd (SE)',
	'S_JAPANESE_JP'=>			'Japán (JP)',
	'S_CHINESE_CN'=>			'Kínai (CN)',
	'S_DUTCH_NL'=>			'Dutch (NL)',
	'S_HUNGARY_HU'=>			'Magyar (HU)',
	'S_ZABBIX_BIG'=>			'ZABBIX',
	'S_HOST_PROFILES'=>			'Host profiles',
	'S_HOST_PROFILES_BIG'=>			'HOST PROFILES',
	'S_EMPTY'=>			'Üres',
	'S_STANDARD_ITEMS_BIG'=>			'STANDARD ITEMS',
	'S_NO_ITEMS'=>			'No items',
	'S_HELP'=>			'Segítség',
	'S_PROFILE'=>			'Profile',
	'S_GET_SUPPORT'=>			'Get support',
	'S_MONITORING'=>			'Felügyelet',
	'S_INVENTORY'=>			'Leltár',
	'S_QUEUE'=>			'Sor',
	'S_EVENTS'=>			'Események',
	'S_EVENTS_BIG'=>			'ESEMÉNYEK',
	'S_MAPS'=>			'Térképek',
	'S_REPORTS'=>			'Jelentések',
	'S_GENERAL'=>			'Általános',
	'S_AUDIT'=>			'Audit',
	'S_LOGIN'=>			'Belépés',
	'S_LOGOUT'=>			'Kilépés',
	'S_LATEST_DATA'=>			'Legutolsó adat',
	'S_INCORRECT_DESCRIPTION'=>			'Incorrect description',
	'S_CANT_FORMAT_TREE'=>			'Can\"t format Tree',

	);
?>
