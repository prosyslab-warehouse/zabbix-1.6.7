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
/************* DYNAMIC REFRESH *************/

function add_refresh_objects($ref_tab){
	$min = 2147483647; // PHP_INT_MAX
	foreach($ref_tab as $id => $obj){
		$obj['interval'] = (isset($obj['interval']))?$obj['interval']:60;
		zbx_add_post_js(get_refresh_obj_script($obj));

		$min = ($min < $obj['interval'])?$min:$obj['interval'];
	}
	zbx_add_post_js('updater.interval = 10; updater.check4Update();');
}

function get_refresh_obj_script($obj){
	$obj['url'] = isset($obj['url'])?$obj['url']:'';
	$obj['url'].= (zbx_empty($obj['url'])?'?':'&').'output=html';

return 'updater.setObj4Update("'.$obj['id'].'",'.$obj['interval'].',"'.$obj['url'].'",{"favobj": "refresh", "favid": "'.$obj['id'].'"});';
}

function make_refresh_menu($id,$cur_interval,&$menu,&$submenu){

	$menu['menu_'.$id][] = array(S_REFRESH, null, null, array('outer'=> array('pum_oheader'), 'inner'=>array('pum_iheader')));
	$intervals = array('10','30','60', '120','600','900');

	foreach($intervals as $key => $value){
		$menu['menu_'.$id][] = array(
					S_EVERY.SPACE.$value.SPACE.S_SECONDS_SMALL,
					'javascript: setRefreshRate("'.$id.'",'.$value.');'.
					'void(0);',
					null,
					array('outer' => ($value == $cur_interval)?'pum_b_submenu':'pum_o_submenu', 'inner'=>array('pum_i_submenu')
			));
	}
	$submenu['menu_'.$id][] = array();
}

/************* END REFRESH *************/

/************ REQUEST ************/
function get_request($name, $def=NULL){
	if(isset($_REQUEST[$name]))
		return $_REQUEST[$name];
	else
		return $def;
}


function inarr_isset($keys, $array=null){
	if(is_null($array)) $array =& $_REQUEST;

	if(is_array($keys)){
		foreach($keys as $id => $key){
			if( !isset($array[$key]) )
				return false;
		}
		return true;
	}

	return isset($array[$keys]);
}
/************ END REQUEST ************/

/************ COOKIES ************/
/* function:
 *      get_cookie
 *
 * description:
 *      return cookie value by name,
 *      if cookie is not present return $default_value.
 *
 * author: Eugene Grigorjev
 */
function get_cookie($name, $default_value=null){
	if(isset($_COOKIE[$name]))	return $_COOKIE[$name];
	// else
	return $default_value;
}

/* function:
 *      zbx_setcookie
 *
 * description:
 *      set cookies.
 *
 * author: Eugene Grigorjev
 */
function zbx_setcookie($name, $value, $time=null){
	setcookie($name, $value, isset($time) ? $time : (0));
	$_COOKIE[$name] = $value;
}

/* function:
 *      zbx_unsetcookie
 *
 * description:
 *      unset and clear cookies.
 *
 * author: Aly
 */
function zbx_unsetcookie($name){
	zbx_setcookie($name, null, -99999);
	unset($_COOKIE[$name]);
}

/* function:
 *     zbx_flush_post_cookies
 *
 * description:
 *     set posted cookies.
 *
 * author: Eugene Grigorjev
 */
function zbx_flush_post_cookies($unset=false){
	global $ZBX_PAGE_COOKIES;

	if(isset($ZBX_PAGE_COOKIES)){
		foreach($ZBX_PAGE_COOKIES as $cookie){
			if($unset)
				zbx_unsetcookie($cookie[0]);
			else
				zbx_setcookie($cookie[0], $cookie[1], $cookie[2]);
		}
		unset($ZBX_PAGE_COOKIES);
	}
}

/* function:
 *      zbx_set_post_cookie
 *
 * description:
 *      set cookies after authorisation.
 *      require calling 'zbx_flush_post_cookies' function
 *	Called from:
 *         a) in 'include/page_header.php'
 *         b) from 'redirect()'
 *
 * author: Eugene Grigorjev
 */
function zbx_set_post_cookie($name, $value, $time=null){
	global $ZBX_PAGE_COOKIES;

	$ZBX_PAGE_COOKIES[] = array($name, $value, isset($time)?$time:0);
}

/************ END COOKIES ************/

/************* DATE *************/
/* function:
 *      zbx_date2str
 *
 * description:
 *      Convert timestamp to string representation. Retun 'Never' if 0.
 *
 * author: Alexei Vladishev
 */
function zbx_date2str($format, $timestamp){
	return ($timestamp==0)?S_NEVER:date($format,$timestamp);
}

/* function:
 *      zbx_date2age
 *
 * description:
 *      Calculate and convert timestamp to string representation.
 *
 * author: Aly
 */
function zbx_date2age($start_date,$end_date=0,$utime = false){

	if(!$utime){
		$start_date=date('U',$start_date);
		if($end_date)
			$end_date=date('U',$end_date);
		else
			$end_date = time();
	}

	$time = abs($end_date-$start_date);
//SDI($start_date.' - '.$end_date.' = '.$time);

	$years = (int) ($time / (365*86400));
	$time -= $years*365*86400;

	//$months = (int ) ($time / (30*86400));
	//$time -= $months*30*86400;

	$weeks = (int ) ($time / (7*86400));
	$time -= $weeks*7*86400;

	$days = (int) ($time / 86400);
	$time -= $days*86400;

	$hours = (int) ($time / 3600);
	$time -= $hours*3600;

	$minutes = (int) ($time / 60);
	$time -= $minutes*60;

	if($time >= 1){
		$seconds = round($time,2);
		$ms = 0;
	}
	else{
		$seconds = 0;
		$ms = round($time,3) * 1000;
	}

	$str =  (($years)?$years.'y ':'').
//			(($months)?$months.'m ':'').
			(($weeks)?$weeks.'w ':'').
			(($days)?$days.'d ':'').
			(($hours && !$years)?$hours.'h ':'').
			(($minutes && !$years && !$weeks)?$minutes.'m ':'').
			((!$years && !$weeks && !$days && (!$ms || $seconds))?$seconds.'s ':'').
			(($ms && !$years && !$weeks && !$days && !$hours)?$ms.'ms':'');
return $str;
}

function getmicrotime(){
	list($usec, $sec) = explode(" ",microtime());
	return ((float)$usec + (float)$sec);
}

/************* END DATE *************/


/************* SORT *************/
function natksort(&$array) {
	$keys = array_keys($array);
	natcasesort($keys);

	$new_array = array();

	foreach ($keys as $k) {
		$new_array[$k] = $array[$k];
	}

	$array = $new_array;
	return true;
}

function asort_by_key(&$array, $key){
	if(!is_array($array)) {
		error('Incorrect type of asort_by_key');
		return array();
	}

	$key = htmlspecialchars($key);
	uasort($array, create_function('$a,$b', 'return $a[\''.$key.'\'] - $b[\''.$key.'\'];'));
return $array;
}


/* function:
 *      zbx_rksort
 *
 * description:
 *      Recursively sort an array by key
 *
 * author: Eugene Grigorjev
 */
function zbx_rksort(&$array, $flags=NULL){
	if(is_array($array)){
		foreach($array as $id => $data)
			zbx_rksort($array[$id]);

		ksort($array,$flags);
	}
	return $array;
}

/************* END SORT *************/

/************* ZBX MISC *************/

// accepts parametr as integer either
function zbx_ctype_digit($x){
	return preg_match('/^\\d+$/',$x);
}

function zbx_numeric($value){
	if(is_array($value)) return false;
	if(zbx_empty($value)) return false;

	$value = strval($value);

return preg_match('/^[-|+]?\\d+$/',$value);
}

function zbx_empty($value){
	if(is_null($value)) return true;
	if(is_array($value) && empty($value)) return true;
	if(is_string($value) && ($value === '')) return true;
return false;
}

function zbx_strlen(&$str){
	if(!$strlen = strlen($str)) return $strlen;

	$reallen = 0;
	$fbin= 1 << 7;
	$sbin= 1 << 6;

// check first byte for 11xxxxxx or 0xxxxxxx
	for($i=0; $i < $strlen; $i++){
		if(((ord($str[$i]) & $fbin) && (ord($str[$i]) & $sbin)) || !(ord($str[$i]) & $fbin)) $reallen++;
	}
return $reallen;
}

function zbx_strstr($haystack,$needle){
	$pos = strpos($haystack,$needle);
	if($pos !== FALSE){
		$pos = substr($haystack,$pos);
	}
return $pos;
}

function zbx_stristr($haystack,$needle){
	$haystack_B = strtoupper($haystack);
	$needle = strtoupper($needle);

	$pos = strpos($haystack_B,$needle);
	if($pos !== FALSE){
		$pos = substr($haystack,$pos);
	}
return $pos;
}

function zbx_substring($haystack, $start, $end=null){
	if($end < $start) return '';

	$len = zbx_strlen($haystack);
	if(is_null($end))
		$result = substr($haystack, $start);
	else
		$result = substr($haystack, $start, ($end - $start));

return $result;
}

function uint_in_array($needle,$haystack){
	foreach($haystack as $id => $value)
		if(bccomp($needle,$value) == 0) return true;
return false;
}

function str_in_array($needle,$haystack,$strict=false){
	if(is_array($needle)){
		return in_array($needle,$haystack,$strict);
	}
	else if($strict){
		foreach($haystack as $id => $value)
			if($needle === $value) return true;
	}
	else{
		foreach($haystack as $id => $value)
			if(strcmp($needle,$value) == 0) return true;
	}
return false;
}

function zbx_nl2br(&$str){
	$str_res = array();
	$str_arr = explode("\n",$str);
	foreach($str_arr as $id => $str_line){
		array_push($str_res,$str_line,BR());
	}
return $str_res;
}

function zbx_value2array(&$values){
	if(!is_array($values) && !is_null($values)){
		$tmp = array();
		$tmp[$values] = $values;
		$values = $tmp;
	}
}
/************* END ZBX MISC *************/

?>
