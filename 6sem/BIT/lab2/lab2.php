<?php error_reporting(E_ERROR) ?>
<form action="lab2.php" method="POST">
  <label> Текст: <textarea rows="3" cols="100" name="text"><?=isset($_POST['text']) ? $_POST['text'] : ""?></textarea></label> <br />
  <input type="submit" value="Submit" name="Submit" />
</form>
<p>
  <?php echo color_numbers(isset($_POST['text']) ? $_POST['text'] : "") ?>
</p>

<?php
  function color_numbers($text)
  {
    $text = preg_replace_callback("/(?<=\A|\s)\d+\.\d+(?=\s|\Z)/", "color_frac_number", $text);
    $text = preg_replace_callback("/(?<=\A|\s)\d+(?=\s|\Z)/", "color_integer_number", $text);
    return $text;
  }

  function color_integer_number($params)
  {
    return "<font color=\"blue\">".trim($params[0])."</font>";
  }

  function color_frac_number($params)
  {
    return "<font color=\"red\">".round($params[0], 1)."</font>";
  }
?>
