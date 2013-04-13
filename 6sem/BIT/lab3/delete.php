<?php
error_reporting(E_ERROR);
$link = mysql_connect('127.0.0.1', 'root', 'pwd')
    or die('Не удалось соединиться: ' . mysql_error());
mysql_select_db('php_lab3') or die('Не удалось выбрать базу данных');

$query = "DELETE FROM test_table WHERE Id = {$_POST['id']}";
$result = mysql_query($query) or die('Запрос не удался: ' . mysql_error());
header('Location: /lab3');

mysql_free_result($result);

mysql_close($link);
?>
