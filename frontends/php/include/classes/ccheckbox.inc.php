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
	class CCheckBox extends CTag{
/* public */
		function CCheckBox($name='checkbox',$checked='no',$action=null,$value='yes'){
			parent::CTag('input','no');
			$this->tag_body_start = '';

			$this->AddOption('class','checkbox');
			$this->AddOption('type','checkbox');
			$this->AddOption('value',$value);
			$this->AddOption('name',$name);
			$this->AddOption('id',$name);

			$this->SetAction($action);
			$this->SetChecked($checked);
		}

		function SetEnabled($value='yes'){
			if($value=='yes' || $value == true || $value === 1)
				return $this->DelOption('disabled');

			return $this->options['disabled'] = 'disabled';
		}

		function SetChecked($value="yes"){
			if((is_string($value)&& ($value=="yes" || $value=="checked" || $value=="on") || $value=="1") || (is_int($value)&&$value<>0))
				return $this->options['checked'] = 'checked';

			$this->DelOption("checked");
		}

		function SetAction($value='submit()', $event='onclick'){
			$this->AddAction('onclick', $value);
		}
	}

	class CVisibilityBox extends CCheckBox{
		function CVisibilityBox($name='visibilitybox', $value='yes', $object_name=null, $replace_to=null){
			$action = '';

			if(!is_array($object_name)) $object_name = array($object_name);

			$this->object_name = $object_name;
			$this->replace_to = unpack_object($replace_to);

			foreach($this->object_name as $obj_name){
				if(empty($obj_name)) continue;
				$action .= 'visibility_status_changeds(this.checked, '.zbx_jsvalue($obj_name).','.zbx_jsvalue($this->replace_to).');';
			}

			parent::CCheckBox($name, $value, $action, '1');

			insert_javascript_for_visibilitybox();
		}

		function ToString($destroy=true){
			global $ZBX_PAGE_POST_JS;

			if(!isset($this->options['checked'])){
				foreach($this->object_name as $obj_name){
					if(empty($obj_name)) continue;
					zbx_add_post_js('visibility_status_changeds(false,'.zbx_jsvalue($obj_name).','.zbx_jsvalue($this->replace_to).');');
				}
			}

			return parent::ToString($destroy);
		}
	}
?>
