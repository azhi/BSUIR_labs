<script>
  function formSubmit(id) {
    document.forms[0].id.value = id;
    document.forms[0].submit();
  }
</script>

<?php
error_reporting(E_ERROR);
$link = mysql_connect('127.0.0.1', 'root', 'pwd')
    or die('Не удалось соединиться: ' . mysql_error());
mysql_select_db('php_lab3') or die('Не удалось выбрать базу данных');

echo "<form action=delete.php method=POST>\n";
echo "<input type=hidden name=id value=-1>\n";
echo "</form>";

$query = 'SELECT * FROM test_table';
$result = mysql_query($query) or die('Запрос не удался: ' . mysql_error());

echo "<table>\n";
echo "<thead>\n";
echo "\t<th>Id</th>\n";
echo "\t<th>Name</th>\n";
echo "\t<th>Adress</th>\n";
echo "\t<th>Actions</th>\n";
echo "</thead>\n";
echo "<tbody>\n";
while ($line = mysql_fetch_array($result, MYSQL_ASSOC)) {
    echo "\t<tr>\n";
    foreach ($line as $col_value) {
        echo "\t\t<td>$col_value</td>\n";
    }
    echo "<td>\n";
    echo "\t<a href=\"update.php?id={$line['id']}\">update</a>\n";
    echo "&nbsp;";
    echo "\t<a href=\"javascript:formSubmit({$line['id']});\">delete</a>\n";
    echo "</td>\n";
    echo "\t</tr>\n";
}
echo "</table>\n";

echo "\t<a href=\"create.php\">Create new</a>\n";

mysql_free_result($result);

mysql_close($link);
?>
