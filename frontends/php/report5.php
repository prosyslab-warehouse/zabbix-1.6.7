<?php
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
?>
<?php
	require_once "include/config.inc.php";
	require_once "include/triggers.inc.php";

	$page["title"]	= "S_TRIGGERS_TOP_100";
	$page["file"]	= "report5.php";
	$page['hist_arg'] = array('period');
	
include_once "include/page_header.php";

?>
<?php
//		VAR			TYPE	OPTIONAL FLAGS	VALIDATION	EXCEPTION
	$fields=array(
		"period"=>		array(T_ZBX_STR, O_OPT,	P_SYS|P_NZERO,	IN('"day","week","month","year"'),		NULL)
	);

	check_fields($fields);
?>
<?php
	$_REQUEST["period"] = get_request("period", "day");

	$form = new CForm();
	$form->SetMethod('get');
	
	$cmbPeriod = new CComboBox("period",$_REQUEST["period"],"submit()");
	$cmbPeriod->AddItem("day",S_DAY);
	$cmbPeriod->AddItem("week",S_WEEK);
	$cmbPeriod->AddItem("month",S_MONTH);
	$cmbPeriod->AddItem("year",S_YEAR);

	$form->AddItem($cmbPeriod);

	show_table_header(S_TRIGGERS_TOP_100_BIG, $form);
?>
<?php
	$table = new CTableInfo();
	$table->setHeader(array(
			is_show_subnodes() ? S_NODE : null,
			S_HOST,
			S_TRIGGER,
			S_SEVERITY,
			S_NUMBER_OF_STATUS_CHANGES
			));

	switch($_REQUEST["period"]){
		case "week":	$time_dif=7*86400;	break;
		case "month":	$time_dif=30*86400;	break;
		case "year":	$time_dif=365*86400;	break;
		case "day":
		default:	$time_dif=86400;	break;
	}

	$available_hosts = get_accessible_hosts_by_user($USER_DETAILS,PERM_READ_ONLY,PERM_RES_IDS_ARRAY);
		$sql = 'SELECT h.host, t.triggerid, t.description, t.expression, t.priority, count(distinct e.eventid) as cnt_event '.
						' FROM hosts h, triggers t, functions f, items i, events e'.
						' WHERE h.hostid = i.hostid '.
							' and i.itemid = f.itemid '.
							' and t.triggerid=f.triggerid '.
							' and t.triggerid=e.objectid '.
							' and e.object='.EVENT_OBJECT_TRIGGER.
							' and e.clock>'.(time()-$time_dif).
							' and '.DBcondition('h.hostid',$available_hosts).
							' and '.DBin_node('t.triggerid').
						' GROUP BY h.host,t.triggerid,t.description,t.expression,t.priority '.
						' ORDER BY cnt_event desc, h.host, t.description, t.triggerid';
        $result=DBselect($sql, 100);
        while($row=DBfetch($result)){
			if(!check_right_on_trigger_by_triggerid(null, $row['triggerid'], $available_hosts))
				continue;

            $table->addRow(array(
				get_node_name_by_elid($row['triggerid']),
				$row["host"],
				expand_trigger_description_by_data($row),
				new CCol(get_severity_description($row["priority"]),get_severity_style($row["priority"])),
				$row["cnt_event"],
			));
	}
	$table->show();
?>
<?php

include_once "include/page_footer.php";

?>
