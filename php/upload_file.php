<?php
/*
 * upload_file.php
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
  echo $_POST["submit"]. "<br />";
  echo count($_FILES) . "<br />";
  if (!empty($_FILES["file"]))
  {
    if ($_FILES["file"]["error"] > 0) {
	  echo "Return Code: " . $_FILES["file"]["error"] . "<br />";
    } else {
      echo "Upload: " . $_FILES["file"]["name"] . "<br />";
      echo "Type: " . $_FILES["file"]["type"] . "<br />";
      echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
      echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br />";

      if (file_exists("upload/" . $_FILES["file"]["name"])) {
        echo $_FILES["file"]["name"] . " already exists. ";
      } else {
        move_uploaded_file($_FILES["file"]["tmp_name"], "upload/" . $_FILES["file"]["name"]);
        echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
      }
    }
  } 
  if (!empty($_FILES["image"]))
  {
    if ($_FILES["image"]["error"] > 0) {
	  echo "Return Code: " . $_FILES["image"]["error"] . "<br />";
    } else {
      echo "Upload: " . $_FILES["image"]["name"] . "<br />";
      echo "Type: " . $_FILES["image"]["type"] . "<br />";
      echo "Size: " . ($_FILES["image"]["size"] / 1024) . " Kb<br />";
      echo "Temp file: " . $_FILES["image"]["tmp_name"] . "<br />";

      if (file_exists("upload/" . $_FILES["image"]["name"])) {
        echo $_FILES["image"]["name"] . " already exists. ";
      } else {
        move_uploaded_file($_FILES["image"]["tmp_name"], "upload/" . $_FILES["image"]["name"]);
        echo "Stored in: " . "upload/" . $_FILES["image"]["name"];
      }
    }
  }   
?>
