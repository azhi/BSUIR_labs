<?php
session_start();
$data = unserialize($_SESSION['data']);
echo "<p> Number: {$data[0]} </p>\n";
echo "<p> Str: {$data[1]} </p>\n";
var_dump($data[2]);
echo "<br /> <a href=\"generator.php\"> Generate new </a>\n";
?>
