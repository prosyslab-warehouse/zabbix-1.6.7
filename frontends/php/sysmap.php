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
	require_once('include/config.inc.php');
	require_once('include/maps.inc.php');
	require_once('include/forms.inc.php');

	$page['title'] = "S_CONFIGURATION_OF_NETWORK_MAPS";
	$page['file'] = 'sysmap.php';
	$page['hist_arg'] = array('sysmapid');
	
include_once('include/page_header.php');

?>
<?php

//		VAR			TYPE	OPTIONAL FLAGS	VALIDATION	EXCEPTION
	$fields=array(
		'sysmapid'=>	array(T_ZBX_INT, O_MAND, P_SYS,	DB_ID,NULL),

		'selementid'=>	array(T_ZBX_INT, O_OPT,	 P_SYS,	DB_ID,		NULL),
		'elementid'=>	array(T_ZBX_INT, O_OPT,  NULL, DB_ID,	'isset({save})'),
		'elementtype'=>	array(T_ZBX_INT, O_OPT,  NULL, IN('0,1,2,3,4'),	'isset({save})'),
		'label'=>	array(T_ZBX_STR, O_OPT,  NULL, NOT_EMPTY,	'isset({save})'),
		'x'=>		array(T_ZBX_INT, O_OPT,  NULL,  BETWEEN(0,65535),'isset({save})'),
		'y'=>           array(T_ZBX_INT, O_OPT,  NULL,  BETWEEN(0,65535),'isset({save})'),
		'iconid_off'=>	array(T_ZBX_INT, O_OPT,  NULL, DB_ID,		'isset({save})'),
		'iconid_on'=>	array(T_ZBX_INT, O_OPT,  NULL, DB_ID,		'isset({save})'),
		'iconid_unknown'=>	array(T_ZBX_INT, O_OPT,  NULL, DB_ID,		'isset({save})'),
		'iconid_disabled'=>	array(T_ZBX_INT, O_OPT,  NULL, DB_ID,		'isset({save})'),
		'url'=>		array(T_ZBX_STR, O_OPT,  NULL, NULL,		'isset({save})'),
		'label_location'=>array(T_ZBX_INT, O_OPT, NULL,	IN('-1,0,1,2,3'),'isset({save})'),

		'linkid'=>	array(T_ZBX_INT, O_OPT,	 P_SYS,	DB_ID,NULL),
		'selementid1'=>	array(T_ZBX_INT, O_OPT,  NULL, DB_ID.'{}!={selementid2}','isset({save_link})'),
		'selementid2'=> array(T_ZBX_INT, O_OPT,  NULL, DB_ID.'{}!={selementid1}','isset({save_link})'),
		'triggers'=>	array(T_ZBX_STR, O_OPT,  NULL, null,null),
		'drawtype'=>array(T_ZBX_INT, O_OPT,  NULL, IN('0,1,2,3,4'),'isset({save_link})'),
		'color'=>	array(T_ZBX_STR, O_OPT,  NULL, NOT_EMPTY,'isset({save_link})'),

/* actions */
		'save'=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	NULL,	NULL),
		'save_link'=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	NULL,	NULL),
		'delete'=>		array(T_ZBX_STR, O_OPT, P_SYS|P_ACT,	NULL,	NULL),
		'cancel'=>		array(T_ZBX_STR, O_OPT, P_SYS,	NULL,	NULL),
/* other */
		'form'=>		array(T_ZBX_STR, O_OPT, P_SYS,	NULL,	NULL),
		'form_refresh'=>	array(T_ZBX_INT, O_OPT,	NULL,	NULL,	NULL)
	);

	check_fields($fields);
?>
<?php
	show_table_header(S_CONFIGURATION_OF_NETWORK_MAPS_BIG);

	if(!sysmap_accessible($_REQUEST['sysmapid'],PERM_READ_WRITE)) access_deny();
	
	$sysmap = DBfetch(DBselect('select * from sysmaps where sysmapid='.$_REQUEST['sysmapid']));
?>
<?php
	if(isset($_REQUEST['save'])){
		$result = false;
		$selementid = get_request('selementid');

		if(isset($_REQUEST['selementid'])){ // update element
			if(($_REQUEST['elementid']>0) || ($_REQUEST['elementtype'] == SYSMAP_ELEMENT_TYPE_IMAGE)){
				$result=update_sysmap_element($_REQUEST['selementid'],
					$_REQUEST['sysmapid'],$_REQUEST['elementid'],$_REQUEST['elementtype'],
					$_REQUEST['label'],$_REQUEST['x'],$_REQUEST['y'],
					$_REQUEST['iconid_off'],$_REQUEST['iconid_unknown'],$_REQUEST['iconid_on'],
					$_REQUEST['iconid_disabled'],$_REQUEST['url'],$_REQUEST['label_location']);
				$selementid = $_REQUEST['selementid'];
			}
			else{
				info('Map element is not selected');
			}
			
			show_messages($result,'Element updated','Cannot update element');
		}
		else{ // add element
			if(($_REQUEST['elementid']>0) || ($_REQUEST['elementtype'] == SYSMAP_ELEMENT_TYPE_IMAGE)){
				$result=add_element_to_sysmap($_REQUEST['sysmapid'],$_REQUEST['elementid'],
					$_REQUEST['elementtype'],$_REQUEST['label'],$_REQUEST['x'],$_REQUEST['y'],
					$_REQUEST['iconid_off'],$_REQUEST['iconid_unknown'],$_REQUEST['iconid_on'],
					$_REQUEST['iconid_disabled'],$_REQUEST['url'],$_REQUEST['label_location']);
				$selementid = $result;
			}
			else{
				info('Map element is not selected');
			}
			
			show_messages($result,'Element added','Cannot add element');
		}
		add_audit_if($result,AUDIT_ACTION_UPDATE,AUDIT_RESOURCE_MAP,'Name ['.$sysmap['name'].'] Element ['.$selementid.'] updated ');
		if($result)	unset($_REQUEST['form']);
	}
	
	if(isset($_REQUEST['save_link'])){
		if(isset($_REQUEST['linkid'])){ // update link
			$result=update_link($_REQUEST['linkid'],
				$_REQUEST['sysmapid'],$_REQUEST['selementid1'],$_REQUEST['selementid2'],
				get_request('triggers',array()), $_REQUEST['drawtype'],$_REQUEST['color']);
			$linkid = $_REQUEST['linkid'];

			show_messages($result,'Link updated','Cannot update link');
		}
		else{ // add link
			$result=add_link($_REQUEST['sysmapid'],$_REQUEST['selementid1'],$_REQUEST['selementid2'],
				get_request('triggers',array()),	$_REQUEST['drawtype'],$_REQUEST['color']);
			$linkid = $result;

			show_messages($result,'Link added','Cannot add link');
		}
		add_audit_if($result,AUDIT_ACTION_UPDATE,AUDIT_RESOURCE_MAP,'Name ['.$sysmap['name'].'] Link ['.$linkid.'] updated ');
		if($result)	unset($_REQUEST['form']);
	}
	else if(isset($_REQUEST['delete'])){
		
		if(isset($_REQUEST['linkid'])){
			$result=delete_link($_REQUEST['linkid']);
			show_messages($result,'Link deleted','Cannot delete link');
			add_audit_if($result,AUDIT_ACTION_UPDATE,AUDIT_RESOURCE_MAP,
				'Name ['.$sysmap['name'].'] Link ['.$_REQUEST['linkid'].'] deleted');

			if($result)
			{
				unset($_REQUEST['linkid']);
				unset($_REQUEST['form']);
			}
		}
		else if(isset($_REQUEST['selementid'])){
			$result=delete_sysmaps_element($_REQUEST['selementid']);
			show_messages($result,'Element deleted','Cannot delete element');
			add_audit_if($result,AUDIT_ACTION_UPDATE,AUDIT_RESOURCE_MAP,
				'Name ['.$sysmap['name'].'] Element ['.$_REQUEST['selementid'].'] deleteed ');

			if($result){
				unset($_REQUEST['selementid']);
				unset($_REQUEST['form']);
			}
		}
	}
?>
<?php
	echo SBR;
	if(isset($_REQUEST['form']) && ($_REQUEST['form']=='add_element' ||
		($_REQUEST['form']=='update' && isset($_REQUEST['selementid']))))
	{
		show_table_header(S_DISPLAYED_ELEMENTS);
		echo SBR;
		insert_map_element_form();
	}
	else if(isset($_REQUEST['form']) && ($_REQUEST['form']=='add_link' || ($_REQUEST['form']=='update' && isset($_REQUEST['linkid'])))){
		$row = DBfetch(DBselect('select count(*) as count from sysmaps_elements where sysmapid='.$_REQUEST['sysmapid']));
		if($row['count']>1){
			show_table_header(S_CONNECTORS);
			echo SBR;
			insert_map_link_form();
		}
		else{
			info('Not enough elements in this map');
		}
	}
	else{
		show_table_header(S_DISPLAYED_ELEMENTS, new CButton('form',S_ADD_ELEMENT,
			"return redirect('".$page['file'].'?form=add_element'.url_param('sysmapid')."');"));

		$table = new CTableInfo();
		$table->setHeader(array(S_LABEL,S_TYPE,S_X,S_Y,S_ICON_OK,S_ICON_PROBLEM,S_ICON_UNKNOWN,S_ICON_DISABLED));

		$map_elements = array();
		$sql = 'select * '.
				' from sysmaps_elements '.
				' where sysmapid='.$_REQUEST['sysmapid'];
		$db_elements = DBselect($sql);
		while($db_element = DBfetch($db_elements)){
			$map_elements[$db_element['selementid']] = $db_element;
		}
		order_result($map_elements, 'label');
		
		foreach($map_elements as $selementid => $db_element){
			if(    $db_element['elementtype'] == SYSMAP_ELEMENT_TYPE_HOST)		$type = S_HOST;
			elseif($db_element['elementtype'] == SYSMAP_ELEMENT_TYPE_MAP)		$type = S_MAP;
			elseif($db_element['elementtype'] == SYSMAP_ELEMENT_TYPE_TRIGGER)	$type = S_TRIGGER;
			elseif($db_element['elementtype'] == SYSMAP_ELEMENT_TYPE_HOST_GROUP)	$type = S_HOST_GROUP;
			else $type = 'Map element';

			$table->addRow(array(
				new CLink(
					$db_element['label'],
					'sysmap.php?sysmapid='.$db_element['sysmapid'].
					'&form=update&selementid='.$db_element['selementid'],
					'action'),
				nbsp($type),
				$db_element['x'],
				$db_element['y'],
				$db_element['iconid_off'] ? new CImg('image.php?height=24&imageid='.$db_element['iconid_off'],'no image',NULL) : '-',
				$db_element['iconid_on'] ? new CImg('image.php?height=24&imageid='.$db_element['iconid_on'],'no image',NULL) : '-',
				$db_element['iconid_unknown'] ? new CImg('image.php?height=24&imageid='.$db_element['iconid_unknown'],'no image',NULL) : '-',
				$db_element['iconid_disabled'] ? new CImg('image.php?height=24&imageid='.$db_element['iconid_disabled'],'no image',NULL) : '-'
				));
		}
		$table->show();

		echo SBR;
		show_table_header(S_CONNECTORS, new CButton('form',S_CREATE_CONNECTION,
			"return redirect('".$page['file'].'?form=add_link'.
			url_param('sysmapid')."');"));

		$table = new CTableInfo();
		$table->setHeader(array(S_LINK,S_ELEMENT_1,S_ELEMENT_2,S_LINK_STATUS_INDICATOR));

		$i = 1;
		
		$links = array();
		$selementids = array();
		$sql = 'SELECT linkid,selementid1,selementid2 '.
				' FROM sysmaps_links '.
				' WHERE sysmapid='.$_REQUEST['sysmapid'];
		$result=DBselect($sql);
		while($row=DBfetch($result)){
			$links[$row['linkid']] = $row;
			$selementids[$row['selementid1']] = $row['selementid1'];
			$selementids[$row['selementid2']] = $row['selementid2'];
		}
		
		$labels = array();
		$sql = 'SELECT selementid, label '.
				' FROM sysmaps_elements '.
				' WHERE '.DBcondition('selementid',$selementids);
		$result=DBselect($sql);
		while($row=DBfetch($result)){
			$labels[$row['selementid']] = $row['label'];
		}
		
		foreach($links as $linkid => $row){
			$links[$linkid]['label1']=$labels[$row['selementid1']];
			$links[$linkid]['label2']=$labels[$row['selementid2']];
		}
		
		order_result($links, 'label1');

		foreach($links as $linkid => $row){
			$label1 = $row['label1'];
			$label2 = $row['label2'];
			
	/* prepare description */
	
			$triggers = get_link_triggers($row['linkid']);
			$description=array();

			foreach($triggers as $id => $trigger){
				if(isset($trigger['triggerid'])){
					if(!empty($description)) $description[]=BR();
					$triggers[$id]['description'] = expand_trigger_description($trigger['triggerid']);
				}
				$description[]= $triggers[$id]['description'];
			}
			
			if(empty($description))
				$description='-';

	/* draw row */
			$table->addRow(array(
				new CLink('link '.$i++,
					'sysmap.php?sysmapid='.$_REQUEST['sysmapid'].
					'&form=update&linkid='.$row['linkid'],
					'action'),
				$label1,
				$label2,
				$description
				));
		}
		$table->Show();
	}
	
	show_messages();
	
	echo SBR;
	$map=get_sysmap_by_sysmapid($_REQUEST['sysmapid']);
	show_table_header($map['name']);

	$table = new CTable(NULL,'map');
	if(isset($_REQUEST['sysmapid'])){
		$linkMap = new CMap('links'.$_REQUEST['sysmapid'].'_'.rand(0,100000));

		$db_elements = DBselect('select * from sysmaps_elements where sysmapid='.$_REQUEST['sysmapid']);
		while($db_element = DBfetch($db_elements)){
			$tmp_img = get_png_by_selementid($db_element['selementid']);
			if(!$tmp_img) continue;

			$x1_	= $db_element['x'];
			$y1_	= $db_element['y'];
			$x2_	= $db_element['x'] + imagesx($tmp_img);
			$y2_	= $db_element['y'] + imagesy($tmp_img);

			$linkMap->addRectArea($x1_,$y1_,$x2_,$y2_,
				'sysmap.php?form=update&sysmapid='.$_REQUEST['sysmapid'].
				'&selementid='.$db_element['selementid'],
				$db_element['label']);

		}
		
		$imgMap = new CImg('map.php?sysmapid='.$_REQUEST['sysmapid']);
		$imgMap->setMap($linkMap->GetName());
		$table->addRow($linkMap);
		$table->addRow($imgMap);
	}
	$table->Show();
?>
<?php
	
include_once('include/page_footer.php');

?>
