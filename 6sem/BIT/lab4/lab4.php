<?php
error_reporting(E_ERROR);
$flash_msg = "";
$nickname = "";
$email = "";
$errors = "";
if (isset($_POST['nickname']) && isset($_POST['email']) &&
    isset($_POST['password']) && isset($_POST['password_confirmation']))
{
  $nickname = $_POST['nickname'];
  $email = $_POST['email'];
  $password = $_POST['password'];
  if (strlen($nickname) < 4)
    $errors = $errors."Nickname should be at least 4 characters long <br />";

  $pattern = "/^[\w+\-.]+@[a-z\d\-.]+\.[a-z]+$/";
  if (!preg_match($pattern, $email))
    $errors = $errors."Your email is not email! <br />";

  if (strlen($password) < 6)
    $errors = $errors."Password should be at least 6 characters long <br />";
  if (strcmp($password, $_POST['password_confirmation']) != 0 )
    $errors = $errors."Password and password confirmation does not match! <br />";

  if (strlen($errors))
    $flash_msg = "Something went wrong:";
  else
    $flash_msg = "Sign up successfull!";
}
include('form.html')
?>
