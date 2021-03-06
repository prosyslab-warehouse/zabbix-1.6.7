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
	require_once "include/items.inc.php";
	require_once "include/graphs.inc.php";

	$page["file"]	= "history.php";
	$page["title"]	= "S_HISTORY";
	$page['hist_arg'] = array('itemid', 'hostid','grouid','graphid','period','dec','inc','left','right','stime');
	$page['scripts'] = array('gmenu.js','scrollbar.js','sbox.js','sbinit.js');

	$page['type'] = detect_page_type(PAGE_TYPE_HTML);

	if(isset($_REQUEST['plaintext'])){
		define('ZBX_PAGE_NO_MENU', 1);
	}
	else if(PAGE_TYPE_HTML == $page['type']){
		define('ZBX_PAGE_DO_REFRESH', 1);
	}

include_once "include/page_header.php";

?>
<?php
//		VAR			TYPE	OPTIONAL FLAGS	VALIDATION	EXCEPTION
	$fields=array(
		"itemid"=>	array(T_ZBX_INT, O_OPT, P_SYS,	DB_ID,	'!isset({favobj})'),

		"from"=>	array(T_ZBX_INT, O_OPT,	 null,	'{}>=0', null),
		"period"=>	array(T_ZBX_INT, O_OPT,	 null,	null, null),
		"dec"=>		array(T_ZBX_INT, O_OPT,	 null,	null, null),
		"inc"=>		array(T_ZBX_INT, O_OPT,	 null,	null, null),
		"left"=>	array(T_ZBX_INT, O_OPT,	 null,	null, null),
		"right"=>	array(T_ZBX_INT, O_OPT,	 null,	null, null),
		"stime"=>	array(T_ZBX_STR, O_OPT,	 null,	null, null),

		"filter_task"=>	array(T_ZBX_STR, O_OPT,	 null,
			IN(FILTER_TAST_SHOW.','.FILTER_TAST_HIDE.','.FILTER_TAST_MARK.','.FILTER_TAST_INVERT_MARK), null),
		"filter"=>	array(T_ZBX_STR, O_OPT,	 null,	null, null),
		"mark_color"=>	array(T_ZBX_STR, O_OPT,	 null,	IN(MARK_COLOR_RED.','.MARK_COLOR_GREEN.','.MARK_COLOR_BLUE), null),

		"cmbloglist"=>	array(T_ZBX_INT, O_OPT,	 null,	DB_ID, null),

		"plaintext"=>	array(T_ZBX_STR, O_OPT,	 null,	null, null),
		"action"=>	array(T_ZBX_STR, O_OPT,	 null,	IN('"showgraph","showvalues","showlatest","add","remove"'), null),
//ajax
		'favobj'=>		array(T_ZBX_STR, O_OPT, P_ACT,	NULL,			NULL),
		'favid'=>		array(T_ZBX_STR, O_OPT, P_ACT,  NOT_EMPTY,		'isset({favobj})'),

/* actions */
		"remove_log"=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	null,	null),
		"reset"=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	null,	null),
		"cancel"=>		array(T_ZBX_STR, O_OPT, P_SYS,	null,	null),
/* other */
		"form"=>		array(T_ZBX_STR, O_OPT, P_SYS,	null,	null),
		"form_copy_to"=>	array(T_ZBX_STR, O_OPT, P_SYS,	null,	null),
		"form_refresh"=>	array(T_ZBX_INT, O_OPT,	null,	null,	null),
		"fullscreen"=>		array(T_ZBX_INT, O_OPT,	P_SYS,	null,	null)
	);

	check_fields($fields);
?>
<?php
	if(isset($_REQUEST['favobj'])){
		if(in_array($_REQUEST['favobj'],array('itemid','graphid'))){
			$result = false;
			if('add' == $_REQUEST['action']){
				$result = add2favorites('web.favorite.graphids',$_REQUEST['favid'],$_REQUEST['favobj']);
				if($result){
					print('$("addrm_fav").title = "'.S_REMOVE_FROM.' '.S_FAVOURITES.'";'."\n");
					print('$("addrm_fav").onclick = function(){rm4favorites("itemid","'.$_REQUEST['favid'].'",0);}'."\n");
				}
			}
			else if('remove' == $_REQUEST['action']){
				$result = rm4favorites('web.favorite.graphids',$_REQUEST['favid'],ZBX_FAVORITES_ALL,$_REQUEST['favobj']);

				if($result){
					print('$("addrm_fav").title = "'.S_ADD_TO.' '.S_FAVOURITES.'";'."\n");
					print('$("addrm_fav").onclick = function(){ add2favorites("itemid","'.$_REQUEST['favid'].'");}'."\n");
				}
			}

			if((PAGE_TYPE_JS == $page['type']) && $result){
				print('switchElementsClass("addrm_fav","iconminus","iconplus");');
			}
		}
	}

	if((PAGE_TYPE_JS == $page['type']) || (PAGE_TYPE_HTML_BLOCK == $page['type'])){
		exit();
	}

	$_REQUEST["action"] = get_request("action", "showgraph");

/*** Prepare page header - start ***/
	if(is_array($_REQUEST['itemid'])){
		$_REQUEST['itemid'] = array_unique($_REQUEST['itemid']);

		if(isset($_REQUEST['remove_log']) && isset($_REQUEST['cmbloglist'])){
			foreach($_REQUEST['itemid'] as $id => $itemid)
				if((bccomp($itemid , $_REQUEST['cmbloglist'])==0))
					unset($_REQUEST['itemid'][$id]);
		}

		$items_count = count($_REQUEST['itemid']);
		if($items_count > 1){
			$main_header = count($_REQUEST['itemid']).' log files';
		}
		else{
			$_REQUEST['itemid'] = array_pop($_REQUEST['itemid']);
		}
	}

	$available_hosts = get_accessible_hosts_by_user($USER_DETAILS,PERM_READ_ONLY,PERM_RES_IDS_ARRAY);

	$sql = 'SELECT h.host,i.hostid,i.description,i.key_ '.
			' FROM items i,hosts h '.
			' WHERE i.itemid IN ('.(is_array($_REQUEST['itemid']) ? implode(',', $_REQUEST['itemid']) : $_REQUEST['itemid']).') '.
				' AND h.hostid=i.hostid '.
				' AND '.DBcondition('h.hostid',$available_hosts, true);

	if(DBfetch(DBselect($sql))){
		access_deny();
	}

	$sql = 'SELECT h.host,i.hostid,i.* '.
			' FROM items i,hosts h '.
			' WHERE i.itemid in ('.(is_array($_REQUEST['itemid']) ? implode(',', $_REQUEST['itemid']) : $_REQUEST['itemid']).') '.
				' AND h.hostid=i.hostid ';
	$item_data = DBfetch(DBselect($sql));

	$item_type = $item_data['value_type'];
	$l_header = null;

	if(!is_array($_REQUEST['itemid'])){
		$main_header = $item_data['host'].': '.item_description($item_data);

		if(isset($_REQUEST['plaintext']))
			echo $main_header.SBR;

		$_REQUEST['period'] = get_request('period',get_profile('web.item.graph.period', ZBX_PERIOD_DEFAULT, PROFILE_TYPE_INT, $_REQUEST['itemid']));
		if($_REQUEST['period'] >= ZBX_MIN_PERIOD){
			update_profile('web.item.graph.period',$_REQUEST['period'], PROFILE_TYPE_INT, $_REQUEST['itemid']);
		}

		$l_header = array(new CLink($item_data['host'],'latest.php?hostid='.$item_data['hostid']),': ',
			item_description($item_data));

		if('showgraph' == $_REQUEST['action']){
			if(infavorites('web.favorite.graphids',$_REQUEST['itemid'],'itemid')){
				$icon = new CDiv(SPACE,'iconminus');
				$icon->addOption('title',S_REMOVE_FROM.' '.S_FAVOURITES);
				$icon->addAction('onclick',new CScript("javascript: rm4favorites('itemid','".$_REQUEST['itemid']."',0);"));
			}
			else{
				$icon = new CDiv(SPACE,'iconplus');
				$icon->addOption('title',S_ADD_TO.' '.S_FAVOURITES);
				$icon->addAction('onclick',new CScript("javascript: add2favorites('itemid','".$_REQUEST['itemid']."');"));
			}
			$icon->addOption('id','addrm_fav');

			$icon_tab = new CTable();
			$icon_tab->addRow(array($icon,SPACE,$l_header));

			$l_header = $icon_tab;
		}
	}

	unset($item_data);

	$to_save_request = null;

	if( !isset($_REQUEST['plaintext']) && ($_REQUEST['fullscreen']==0) ){
		if($item_type == ITEM_VALUE_TYPE_LOG){
			$l_header = new CForm();
			$l_header->setName('loglist');
			$l_header->setMethod('get');
			$l_header->addVar('action',$_REQUEST['action']);
			$l_header->addVar('from',get_request('from', 0));
			$l_header->addVar('period',$_REQUEST['period']);
			$l_header->addVar('itemid',$_REQUEST['itemid']);

			if(isset($_REQUEST['filter_task']))	$l_header->addVar('filter_task',$_REQUEST['filter_task']);
			if(isset($_REQUEST['filter']))		$l_header->addVar('filter',$_REQUEST['filter']);
			if(isset($_REQUEST['mark_color']))	$l_header->addVar('mark_color',$_REQUEST['mark_color']);

			$cmbLogList = new CComboBox('cmbloglist');
			if(is_array($_REQUEST['itemid'])){
				$cmbLogList->addItem(0, $main_header);
				foreach($_REQUEST['itemid'] as $itemid){
					if(!($item = get_item_by_itemid($itemid)) || $item['value_type'] != ITEM_VALUE_TYPE_LOG){
						invalid_url();
					}

					$host = get_host_by_hostid($item['hostid']);
					$cmbLogList->addItem($itemid,$host['host'].': '.item_description($item));
				}
			}
			else{
				$cmbLogList->addItem($_REQUEST['itemid'], $main_header);
			}

			$l_header->addItem(array(
				'Log files list',SPACE,
				$cmbLogList,SPACE,
				new CButton('add_log','add',"return PopUp('popup.php?".
					"dstfrm=".$l_header->getName()."&srctbl=logitems&dstfld1=itemid&srcfld1=itemid');"),SPACE,
				$cmbLogList->ItemsCount() > 1 ? new CButton("remove_log","Remove selected") : null
				));
		}

		$form = new CForm();
		$form->setMethod('get');

		$form->addVar('itemid',$_REQUEST['itemid']);

		if(isset($_REQUEST['filter_task']))	$form->addVar('filter_task',$_REQUEST['filter_task']);
		if(isset($_REQUEST['filter']))		$form->addVar('filter',$_REQUEST['filter']);
		if(isset($_REQUEST['mark_color']))	$form->addVar('mark_color',$_REQUEST['mark_color']);

		$cmbAction = new CComboBox('action',$_REQUEST['action'],'submit()');

		if(str_in_array($item_type,array(ITEM_VALUE_TYPE_FLOAT,ITEM_VALUE_TYPE_UINT64))){
			$cmbAction->addItem('showgraph',S_GRAPH);
		}

		$cmbAction->addItem('showvalues',S_VALUES);
		$cmbAction->addItem('showlatest',S_500_LATEST_VALUES);

		$form->addItem($cmbAction);

		if($_REQUEST['action']!='showgraph')
			$form->addItem(array(SPACE,new CButton('plaintext',S_AS_PLAIN_TEXT)));

		show_table_header($l_header, $form);
	}
?>
<?php
	$effectiveperiod = navigation_bar_calc();
		$bstime = $_REQUEST['stime'] = get_request('stime',
										get_profile('web.item.graph.stime',date('YmdHi',(time()-$_REQUEST['period'])),
										PROFILE_TYPE_STR,
										$_REQUEST['itemid']));
	update_profile('web.item.graph.stime', $_REQUEST['stime'], PROFILE_TYPE_STR);

	if($_REQUEST['action']=='showgraph' && ($item_type != ITEM_VALUE_TYPE_LOG)){
		$dom_graph_id = 'graph';
		show_history($_REQUEST['itemid'],$_REQUEST['from'],$bstime,$effectiveperiod);
	}
	else if($_REQUEST['action']=='showvalues' || $_REQUEST['action']=='showlatest'){
	
		if($_REQUEST['action']=='showvalues') {
			$time = mktime(substr($bstime,8,2),substr($bstime,10,2),0,substr($bstime,4,2),substr($bstime,6,2),substr($bstime,0,4));
			$till = $time + $effectiveperiod;
		}
			$l_header = null;
		
		if(!isset($_REQUEST['plaintext'])){
			if($item_type==ITEM_VALUE_TYPE_LOG){
				$to_save_request = array('filter_task', 'filter', 'mark_color');

				$filter_task = get_request('filter_task',0);
				$filter = get_request('filter','');
				$mark_color = get_request('mark_color',0);

				$r_header = new CForm();
				$r_header->setMethod('get');

				$r_header->addVar('action',$_REQUEST['action']);
				$r_header->addVar('from',$_REQUEST['from']);
				$r_header->addVar('period',$_REQUEST['period']);
				$r_header->addVar('itemid',$_REQUEST['itemid']);

				$cmbFTask = new CComboBox('filter_task',$filter_task,'submit()');
				$cmbFTask->addItem(FILTER_TAST_SHOW,S_SHOW_SELECTED);
				$cmbFTask->addItem(FILTER_TAST_HIDE,S_HIDE_SELECTED);
				$cmbFTask->addItem(FILTER_TAST_MARK,S_MARK_SELECTED);
				$cmbFTask->addItem(FILTER_TAST_INVERT_MARK,S_MARK_OTHERS);

				$r_header->addItem(array(
					S_SELECT_ROWS_WITH_VALUE_LIKE,SPACE,
					new CTextBox('filter',$filter,25),
					$cmbFTask,SPACE));

				if(str_in_array($filter_task,array(FILTER_TAST_MARK,FILTER_TAST_INVERT_MARK))){
					$cmbColor = new CComboBox('mark_color',$mark_color);
					$cmbColor->addItem(MARK_COLOR_RED,S_AS_RED);
					$cmbColor->addItem(MARK_COLOR_GREEN,S_AS_GREEN);
					$cmbColor->addItem(MARK_COLOR_BLUE,S_AS_BLUE);
					$r_header->addItem(array($cmbColor,SPACE));
				}
				$r_header->addItem(new CButton('select','Select'));
			}
			else{
				$r_header = null;
			}

			if(($l_header || $r_header) &&	($_REQUEST['fullscreen']==0))
					show_table_header($l_header,$r_header);
		}
		else{
			$txt = new CTag('p','yes',$l_header);
			$txt->Show();
			echo "\n";
		}

		$cond_clock = '';
		$limit = 'NO';
		if($_REQUEST['action']=='showlatest'){
			$limit = 500;
		}
		else if($_REQUEST['action']=='showvalues'){
			$cond_clock = " and h.clock>$time and h.clock<$till";
		}

		if($item_type==ITEM_VALUE_TYPE_LOG){
			$itemid_lst = '';

			if(is_array($_REQUEST['itemid'])){
				$itemid_lst = implode(',',$_REQUEST['itemid']);
				$item_cout = count($_REQUEST['itemid']);
			}
			else{
				$itemid_lst = $_REQUEST['itemid'];
				$item_cout = 1;
			}

			$sql_filter = '';
			if(isset($_REQUEST['filter']) && $_REQUEST['filter']!=''){
				if($_REQUEST['filter_task'] == FILTER_TAST_SHOW)
					$sql_filter = ' AND h.value LIKE '.zbx_dbstr('%'.$_REQUEST['filter'].'%');
				else if($_REQUEST['filter_task'] == FILTER_TAST_HIDE)
					$sql_filter = ' AND h.value NOT LIKE '.zbx_dbstr('%'.$_REQUEST['filter'].'%');
			}

			$sql = 'SELECT hst.host,i.itemid,i.key_,i.description,h.clock,h.value,i.valuemapid,h.timestamp,h.source,h.severity '.
					' FROM history_log h, items i, hosts hst '.
					' WHERE hst.hostid=i.hostid '.
						' AND h.itemid=i.itemid'.$sql_filter.
						' AND i.itemid in ('.$itemid_lst.')'.
						$cond_clock.
					' ORDER BY h.clock desc, h.id DESC';
			$result=DBselect($sql,$limit);

			if(!isset($_REQUEST['plaintext'])){
				$table = new CTableInfo('...','log_history_table');
				$table->addOption('id','graph');
				$table->setHeader(array(S_TIMESTAMP,
						($item_cout>1)?S_ITEM:null,
						S_LOCAL_TIME,
						S_SOURCE,
						S_SEVERITY,
						S_VALUE),'header');

				$table->ShowStart(); // to solve memory leak we call 'Show' method by steps
			}
			else{
				echo '<span class="textcolorstyles"><pre>'."\n";
			}

			while($row=DBfetch($result)){
//				$color_style = null;
				$color_style = 'textcolorstyles';

				if(isset($_REQUEST['filter']) && $_REQUEST['filter']!=''){
					$contain = zbx_stristr($row['value'],$_REQUEST['filter']) ? TRUE : FALSE;

					if(!isset($_REQUEST['mark_color'])) $_REQUEST['mark_color'] = MARK_COLOR_RED;

					if(($contain) && ($_REQUEST['filter_task'] == FILTER_TAST_MARK))
						$color_style = $_REQUEST['mark_color'];
					if((!$contain) && ($_REQUEST['filter_task'] == FILTER_TAST_INVERT_MARK))
						$color_style = $_REQUEST['mark_color'];

					switch($color_style){
						case MARK_COLOR_RED:	$color_style='mark_as_red'; break;
						case MARK_COLOR_GREEN:	$color_style='mark_as_green'; break;
						case MARK_COLOR_BLUE:	$color_style='mark_as_blue'; break;
					}
				}

				$new_row = array(nbsp(date('[Y.M.d H:i:s]',$row['clock'])));

				if($item_cout > 1)
					array_push($new_row,$row['host'].':'.item_description($row));

				if($row['timestamp'] == 0){
					array_push($new_row,new CCol(' - '));
				}
				else{
					array_push($new_row,date('Y.M.d H:i:s',$row['timestamp']));
				}

				if($row['source'] == ''){
					array_push($new_row,new CCol(' - '));
				}
				else{
					array_push($new_row,$row['source']);
				}

				array_push($new_row,
						new CCol(
							get_severity_description($row['severity']),
							get_severity_style($row['severity'])
							)
					);

				$row['value'] = trim($row['value'],"\r\n");
				$row['value'] = encode_log($row['value']);
//				array_push($new_row,htmlspecialchars($row['value']));
				array_push($new_row, zbx_nl2br($row['value']));

				if(!isset($_REQUEST['plaintext'])){

					$crow = new CRow($new_row);

					if(is_null($color_style) && is_array($_REQUEST['itemid'])){
						$min_color = 0x98;
						$max_color = 0xF8;
						$int_color = ($max_color - $min_color) / count($_REQUEST['itemid']);
						$int_color *= array_search($row['itemid'],$_REQUEST['itemid']);
						$int_color += $min_color;
						$crow->addOption('style','background-color: '.sprintf("#%X%X%X",$int_color,$int_color,$int_color));
					}
					else {
						$crow->setClass($color_style);
					}

					$crow->Show();	// to solve memory leak we call 'Show' method for each element
				}
				else{
					echo date('Y-m-d H:i:s',$row['clock']);
					echo "\t".$row['clock']."\t".htmlspecialchars($row['value'])."\n";
				}
			}

			if(!isset($_REQUEST['plaintext']))
				$table->ShowEnd();	// to solve memory leak we call 'Show' method by steps
			else
				echo '</pre></span>';
		}
		else{
			switch($item_type){
				case ITEM_VALUE_TYPE_FLOAT:	$h_table = 'history';		break;
				case ITEM_VALUE_TYPE_UINT64:	$h_table = 'history_uint';	break;
				case ITEM_VALUE_TYPE_TEXT:	$h_table = 'history_text';	break;
				default:			$h_table = 'history_str';
			}

			$sql = 'SELECT h.clock,h.value,i.valuemapid '.
							' FROM '.$h_table.' h, items i '.
							' WHERE h.itemid=i.itemid '.
								' AND i.itemid='.$_REQUEST['itemid'].
								$cond_clock.
							' ORDER BY clock desc';
			$result = DBselect($sql,$limit);
			if(!isset($_REQUEST['plaintext'])){
				$table = new CTableInfo();
				$table->setHeader(array(S_TIMESTAMP, S_VALUE));
				$table->addOption('id','graph');
				$table->ShowStart(); // to solve memory leak we call 'Show' method by steps
			}
			else{
				echo '<span class="textcolorstyles"><pre>'."\n";
			}

COpt::profiling_start('history');
			while($row=DBfetch($result)){

				if($DB['TYPE'] == 'ORACLE' && $item_type == ITEM_VALUE_TYPE_TEXT){
					if(isset($row['value']))
						$row['value'] = $row['value']->load();
					else
						$row['value'] = '';
				}

				if($row['valuemapid'] > 0)
					$value = replace_value_by_map($row['value'], $row['valuemapid']);
				else
					$value = $row['value'];

				$new_row = array(date('Y.M.d H:i:s',$row['clock']));
				if(str_in_array($item_type,array(ITEM_VALUE_TYPE_FLOAT,ITEM_VALUE_TYPE_UINT64))){
					array_push($new_row,$value);
				}
				else{
					$pre = new CTag('pre','yes');
					$pre->addItem($value);

					array_push($new_row,$pre);
				}

				if(!isset($_REQUEST['plaintext'])){
					$table->ShowRow($new_row);
				}
				else{
					echo date('Y-m-d H:i:s',$row['clock']);
					echo "\t".$row['clock']."\t".htmlspecialchars($row['value'])."\n";
				}
			}

			if(!isset($_REQUEST['plaintext'])){
				$table->ShowEnd();	// to solve memory leak we call 'Show' method by steps
				echo SBR;
			}
			else{
				echo '</pre></span>';
			}
COpt::profiling_stop('history');
		}
	}

	if(!isset($_REQUEST['plaintext'])){

		if(str_in_array($_REQUEST['action'],array('showvalues','showgraph'))){
		
			$stime = get_min_itemclock_by_itemid($_REQUEST['itemid']);
			$stime = (is_null($stime))?0:$stime;
			$bstime = time()-$effectiveperiod;
			
			if(isset($_REQUEST['stime'])){
				$bstime = $_REQUEST['stime'];
				$bstime = mktime(substr($bstime,8,2),substr($bstime,10,2),0,substr($bstime,4,2),substr($bstime,6,2),substr($bstime,0,4));
			}
		
 			$script = 'scrollinit(0,'.$effectiveperiod.','.$stime.',0,'.$bstime.'); showgraphmenu("graph");';
			if(isset($dom_graph_id))
				$script.='graph_zoom_init("'.$dom_graph_id.'",'.$bstime.','.$effectiveperiod.',ZBX_G_WIDTH, 200, true);';
			zbx_add_post_js($script);
			//		navigation_bar("history.php",$to_save_request);		}
		}
	}
?>
<?php

include_once "include/page_footer.php";

?>
