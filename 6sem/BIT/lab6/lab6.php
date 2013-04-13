<?php
echo 'Информация о вашем браузере и ОС: '.$_SERVER['HTTP_USER_AGENT'].'<br />';
echo 'Вы зашли с ip-адреса: '.$_SERVER['REMOTE_ADDR'].'<br />';

if (preg_match("/chrome/i", $_SERVER['HTTP_USER_AGENT']))
  $browser = "chrome";
else if (preg_match("/firefox/i", $_SERVER['HTTP_USER_AGENT']))
  $browser = "firefox";
else if (preg_match("/safari/i", $_SERVER['HTTP_USER_AGENT']))
  $browser = "safari";
else if (preg_match("/ie/i", $_SERVER['HTTP_USER_AGENT']))
  $browser = "ie";
else if (preg_match("/internet\s*explorer/i", $_SERVER['HTTP_USER_AGENT']))
  $browser = "ie";
$ip = $_SERVER['REMOTE_ADDR'];

$link = mysql_connect('127.0.0.1', 'root', 'pwd')
    or die('Не удалось соединиться: ' . mysql_error());
mysql_select_db('php_lab3') or die('Не удалось выбрать базу данных');

if (strlen($browser)) {
  $query = "INSERT INTO clients_info (IP, browser) VALUES ('{$ip}', '{$browser}')";
  $result = mysql_query($query);
  echo "Your browser {$browser} added!";
}

$browsers = ['chrome', 'firefox', 'safari', 'ie'];
$results = [];
foreach ($browsers as $browser) {
  $query = "SELECT COUNT(*) FROM clients_info WHERE browser = '{$browser}'";
  $result = mysql_query($query) or die('Запрос не удался: ' . mysql_error());
  $line = mysql_fetch_array($result, MYSQL_ASSOC);
  if ($line) {
    $results[$browser] = $line['COUNT(*)'];
  }
}

echo "<table>\n";
echo "<thead>\n";
echo "\t<th>Browser</th>\n";
echo "\t<th>Count</th>\n";
echo "</thead>\n";
echo "<tbody>\n";
foreach ($browsers as $browser) {
    echo "\t<tr>\n";
    echo "\t\t<td>{$browser}</td>\n";
    echo "\t\t<td>{$results[$browser]}</td>\n";
    echo "\t</tr>\n";
}
echo "</table>\n";

mysql_free_result($result);

mysql_close($link);
?>
