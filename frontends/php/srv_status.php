<?php
/* 
** ZABBIX
** Copyright (C) 2000-2007 SIA Zabbix
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
	require_once("include/config.inc.php");
	require_once("include/services.inc.php");
	require_once('include/classes/ctree.inc.php');
		
	$page["title"] = "S_IT_SERVICES";
	$page["file"] = "srv_status.php";
	$page['scripts'] = array('services.js');
	$page['hist_arg'] = array();

	define('ZBX_PAGE_DO_REFRESH', 1);

include_once "include/page_header.php";

?>
<?php
//		VAR			TYPE	OPTIONAL FLAGS	VALIDATION	EXCEPTION
	$fields=array(
		'serviceid'=>		array(T_ZBX_INT, O_OPT,	P_SYS|P_NZERO,	DB_ID,			NULL),
		'showgraph'=>		array(T_ZBX_INT, O_OPT,	P_SYS,			IN('1'),		'isset({serviceid})'),
		'period'=>	array(T_ZBX_STR, O_OPT,	P_SYS,			NULL,	NULL),
// ajax
		'favobj'=>		array(T_ZBX_STR, O_OPT, P_ACT,	IN('"hat"'),		NULL),
		'favid'=>		array(T_ZBX_STR, O_OPT, P_ACT,  NOT_EMPTY,	'isset({favobj})'),
		'state'=>		array(T_ZBX_INT, O_OPT, P_ACT,	NOT_EMPTY,	'isset({favobj})'),
	);

	check_fields($fields);

/* AJAX */	
	if(isset($_REQUEST['favobj'])){
		if('hat' == $_REQUEST['favobj']){
			update_profile('web.srv_status.hats.'.$_REQUEST['favid'].'.state',$_REQUEST['state'],PROFILE_TYPE_INT);
		}
	}	

	if((PAGE_TYPE_JS == $page['type']) || (PAGE_TYPE_HTML_BLOCK == $page['type'])){
		exit();
	}
//--------
?>
<?php
	$available_hosts = get_accessible_hosts_by_user($USER_DETAILS,PERM_READ_ONLY,PERM_RES_IDS_ARRAY);
	$available_triggers = get_accessible_triggers(PERM_READ_ONLY, array(), PERM_RES_IDS_ARRAY);

	if(isset($_REQUEST['serviceid'])){
		$sql = 'SELECT DISTINCT serviceid, triggerid '.
				' FROM services '.
				' WHERE serviceid='.$_REQUEST['serviceid'];		
		if($service = DBfetch(DBselect($sql))){
			if(isset($service['triggerid']) && !isset($available_triggers[$service['triggerid']])){
				access_deny();
			}
		}
		else{
			unset($service);
		}
	}
	
	unset($_REQUEST['serviceid']);
?>
<?php
//	show_table_header(S_IT_SERVICES_BIG);

	if(isset($service) && isset($_REQUEST['showgraph'])){
		$table  = new CTable(null,'chart');
		$table->AddRow(new CImg('chart5.php?serviceid='.$service['serviceid'].url_param('path')));
		$table->Show();
	} 
	else{
		$periods = array(
			'today' => S_TODAY,
			'week' => S_THIS_WEEK,
			'month' => S_THIS_MONTH,
			'year' => S_THIS_YEAR,
			24 => S_LAST_24_HOURS,
			24*7 => S_LAST_7_DAYS,
			24*30 => S_LAST_30_DAYS,
			24*365 => S_LAST_365_DAYS,
		);
	
		$period = get_request('period', 7*24);
		$period_end = time();
		
		switch($period){
			case 'today':
				$period_start = mktime(0, 0, 0, date('n'), date('j'), date('Y'));
			break;
			case 'week':
				$period_start = strtotime('last sunday');
			break;
			case 'month':
				$period_start = mktime(0, 0, 0, date('n'), 1, date('Y'));
			break;
			case 'year':
				$period_start = mktime(0, 0, 0, 1, 1, date('Y'));
			break;
			case 24:
			case 24*7:
			case 24*30:
			case 24*365:
				$period_start = $period_end - ($period * 3600);
			break;
		}
		
		$query = 'SELECT DISTINCT s.serviceid, sl.servicedownid, sl_p.serviceupid as serviceupid, s.triggerid, '.
				' s.name as caption, s.algorithm, t.description, t.expression, s.sortorder, sl.linkid, s.showsla, s.goodsla, s.status '.
			' FROM services s '.
				' LEFT JOIN triggers t ON s.triggerid = t.triggerid '.
				' LEFT JOIN services_links sl ON  s.serviceid = sl.serviceupid and NOT(sl.soft=0) '.
				' LEFT JOIN services_links sl_p ON  s.serviceid = sl_p.servicedownid and sl_p.soft=0 '.
			' WHERE '.DBin_node('s.serviceid').
				' AND (t.triggerid IS NULL OR '.DBcondition('t.triggerid',$available_triggers).') '.
			' ORDER BY s.sortorder, sl_p.serviceupid, s.serviceid';

		$result=DBSelect($query);
		
		$services = array();
		$row = array(
						'id' => 0,
						'serviceid' => 0,
						'serviceupid' => 0,
						'caption' => S_ROOT_SMALL,
						'status' => SPACE,
						'reason' => SPACE,
						'sla' => SPACE,
						'sla2' => SPACE,
						'graph' => SPACE,
						'linkid'=>''
						);
		
		$services[0]=$row;
		
		while($row = DBFetch($result)){
			$row['id'] = $row['serviceid'];
		
			if(empty($row['serviceupid'])) $row['serviceupid']='0';
			if(empty($row['description'])) $row['description']='None';
			
			$row['graph'] = new CLink(S_SHOW,"srv_status.php?serviceid=".$row["serviceid"]."&showgraph=1".url_param('path'),"action");
			
			if(isset($row["triggerid"]) && !empty($row["triggerid"])){

				$url = new CLink(expand_trigger_description($row['triggerid']),'events.php?triggerid='.$row['triggerid']);
				$row['caption'] = array($row['caption'].' [',$url,']');

			}
			
			if($row["status"]==0 || (isset($service) && (bccomp($service["serviceid"] , $row["serviceid"]) == 0))){
				$row['reason']='-';
			} 
			else {
				$row['reason']='-';
				$result2=DBselect('SELECT s.triggerid,s.serviceid '.
								' FROM services s, triggers t '.
								' WHERE s.status>0 '.
									' AND s.triggerid is not NULL '.
									' AND t.triggerid=s.triggerid '.
									' AND '.DBcondition('t.triggerid',$available_triggers).
									' AND '.DBin_node('s.serviceid').
								' ORDER BY s.status DESC, t.description');
					
				while($row2=DBfetch($result2)){
					if(is_string($row['reason']) && ($row['reason'] == '-'))
						$row['reason'] = new CList(null,"itservices");
					if(does_service_depend_on_the_service($row["serviceid"],$row2["serviceid"])){
						$row['reason']->AddItem(new CLink(
										expand_trigger_description($row2["triggerid"]),
										"events.php?triggerid=".$row2["triggerid"]));
					}
				}
			}
			
			if($row["showsla"]==1){
			
				$stat = calculate_service_availability($row["serviceid"], $period_start, $period_end);
				$p = min($stat['problem'], 20);
				$sla_style = $row['goodsla'] > $stat['ok'] ? 'red' : 'green';  
				
				$sizeX = 160;
				$sizeY = 15;
				$sizeX_red = $sizeX*$p/20;
				$sizeX_green = $sizeX - $sizeX_red;

				$chart = array(
					'chart1'	=> new CImg('images/gradients/sla_green.png',$stat['ok'].'%' ,$sizeX_green,$sizeY),
					'chart2'	=> new CImg('images/gradients/sla_red.png',$stat['problem'].'%' ,$sizeX_red,$sizeY),
					'text'		=> new CSpan(SPACE.sprintf("%.2f",$stat['problem']),$sla_style));
		
				$row['sla'] = new CLink($chart, "report3.php?serviceid=".$row['serviceid']."&year=".date("Y"));

				if($row["goodsla"] > $stat["ok"]){
					$sla_style='red';
				} 
				else {
					$sla_style='green';
				}
				
				$row['sla2'] = array(new CSpan(sprintf("%.2f",$row['goodsla']),'green'),'/', new CSpan(sprintf("%.2f",$stat['ok']),$sla_style));
			} 
			else {
				$row['sla']= "-";
				$row['sla2']= "-";
			}
			
			if(isset($services[$row['serviceid']])){
				$services[$row['serviceid']] = array_merge($services[$row['serviceid']],$row);
			} 
			else {
				$services[$row['serviceid']] = $row;
			}
		
			if(isset($row['serviceupid']))
			$services[$row['serviceupid']]['childs'][] = array('id' => $row['serviceid'], 'soft' => 0, 'linkid' => 0);
	
			if(isset($row['servicedownid']))
			$services[$row['serviceid']]['childs'][] = array('id' => $row['servicedownid'], 'soft' => 1, 'linkid' => $row['linkid']);
		}

		$treeServ = array();
		createShowServiceTree($services,$treeServ);	//return into $treeServ parametr

		//permission issue
		$treeServ = del_empty_nodes($treeServ);

		$tree = new CTree($treeServ,array('caption' => bold(S_SERVICE),
						'status' => bold(S_STATUS), 
						'reason' => bold(S_REASON),
						'sla' => bold('SLA ('.$periods[$period].')'),
						'sla2' => bold(nbsp(S_SLA)),
						'graph' => bold(S_GRAPH)));
		
		if($tree){
			// creates form for choosing a preset interval
			$r_form = new CForm();
			$r_form->setClass('nowrap');
			$r_form->setMethod('get');	
			$r_form->addOption('name', 'period_choice');
			$period_combo = new CComboBox('period', $period, 'javascript: submit();');
			foreach($periods as $key => $val){
				$period_combo->AddItem($key, $val);
			}
			$r_form->AddItem(array(S_PERIOD, $period_combo));	
	
			$tab = create_hat(
					S_IT_SERVICES_BIG,
					$tree->getHTML(),
					$r_form,
					'hat_services',
					get_profile('web.srv_status.hats.hat_services.state',1)
				);
				
			$tab->Show();
			unset($tab);
		} 
		else {
			error('Can not format Tree. Check logik structure in service links');
		}
	}
?>
<?php

include_once "include/page_footer.php";

?>
