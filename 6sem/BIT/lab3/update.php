<?php
error_reporting(E_ERROR);
$link = mysql_connect('127.0.0.1', 'root', 'pwd')
    or die('Не удалось соединиться: ' . mysql_error());
mysql_select_db('php_lab3') or die('Не удалось выбрать базу данных');

$id = isset($_POST['id']) ? $_POST['id'] : $_GET['id'];

$query = "SELECT * FROM test_table WHERE id = {$id}";
$result = mysql_query($query) or die('Запрос не удался: ' . mysql_error());
$line = mysql_fetch_array($result, MYSQL_ASSOC);

$name = isset($_POST['name']) ? $_POST['name'] : $line['name'];
$adress = isset($_POST['adress']) ? $_POST['adress'] : $line['adress'];

if (isset($_POST['id']) && isset($_POST['name']) && isset($_POST['adress'])) {
  $query = "UPDATE test_table SET name = '{$name}',
    adress = '{$adress}' WHERE id = {$id}";
  $result = mysql_query($query) or die('Запрос не удался: ' . mysql_error());
  echo "<p style=\"color: green;\"> Succesfully updated </p>";
}

echo "<form action=\"update.php\" method=\"POST\">";
echo "\t<label> Id: <input type=\"text\" name=\"id\" value=\"{$id}\" /></label> <br />";
echo "\t<label> Name: <input type=\"text\" name=\"name\" value=\"{$name}\" /></label> <br />";
echo "\t<label> Address: <input type=\"text\" name=\"adress\" value=\"{$adress}\" /></label> <br />";
echo "\t<input type=\"submit\" value=\"Submit\" name=\"Submit\" />";
echo "</form>\n";

echo "<a href=\"/lab3\"> Back to index </a>";

mysql_free_result($result);

mysql_close($link);
?>
