<?php
function generateRandomString($length = 10) {
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
    $randomString .= $characters[rand(0, strlen($characters) - 1)];
  }
  return $randomString;
}

$number = rand(0, 100);
$str = "Hello, world!";

$array = [];

$count = rand(3, 10);
for ($i = 0; $i < $count; $i++) {
  $dec = rand(0, 1);
  if ($dec == 0)
    $array[] = rand(0, 10);
  else
    $array[] = generateRandomString(rand(5, 10));
}

$data = [$number, $str, $array];

session_start();
$_SESSION['data'] = serialize($data);

header('Location: receiver.php');
?>
