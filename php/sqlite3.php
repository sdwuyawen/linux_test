<?php
/*
 * sqlite3.php
 * 
 * Copyright 2015 Paul <wu_bao@163.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
header("Content-Type: text/html;charset=utf-8");   

try {
	$dbh = new PDO('sqlite:game.db'); 
	if ($dbh){ 
		$dbh->beginTransaction(); 
		$sth = $dbh->prepare('select * from game_list where type>16'); 
		$sth->execute();
		$sth->setFetchMode(PDO::FETCH_ASSOC);
		$result = $sth->fetchAll();
		$json = json_encode($result, JSON_UNESCAPED_UNICODE);
		echo $json;
		//$fp_res = fopen('/var/www/vgameslist', "w");
		//if ($fp_res != 0) {
			//echo "write data to game_list";
			//fwrite($fp_res, $json);
			//fclose($fp_res);
		//} else {
			//echo "open game_list fail";
		//}
	}else{ 
		echo 'connect bad'; 
	}
} catch (PDOException $e) {
	echo 'Connection failed: '.$e->getMessage();
}
?>
