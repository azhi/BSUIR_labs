<?php
error_reporting(E_ERROR);
$link = mysql_connect('127.0.0.1', 'root', 'pwd')
    or die('Не удалось соединиться: ' . mysql_error());
mysql_select_db('php_lab3') or die('Не удалось выбрать базу данных');

$id = isset($_POST['id']) ? $_POST['id'] : "";
$name = isset($_POST['name']) ? $_POST['name'] : "";
$adress = isset($_POST['adress']) ? $_POST['adress'] : "";

if (isset($_POST['id']) && isset($_POST['name']) && isset($_POST['adress'])) {
  $query = "INSERT INTO test_table (id, name, adress) VALUES ({$id}, '{$name}', '{$adress}')";
  $result = mysql_query($query) or die('Запрос не удался: ' . mysql_error());
  if ($result)
    header('Location: /lab3');
  else
    echo "<p style=\"color: red;\"> Something went wrong, fix your values </p>";
}

echo "<form action=\"create.php\" method=\"POST\">";
echo "\t<label> Id: <input type=\"text\" name=\"id\" value=\"{$id}\" /></label> <br />";
echo "\t<label> Name: <input type=\"text\" name=\"name\" value=\"{$name}\" /></label> <br />";
echo "\t<label> Address: <input type=\"text\" name=\"adress\" value=\"{$adress}\" /></label> <br />";
echo "\t<input type=\"submit\" value=\"Submit\" name=\"Submit\" />";
echo "</form>\n";

mysql_free_result($result);

mysql_close($link);
?>
