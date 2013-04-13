<form action="lab1.php" method="POST">
  <label> День рождения: <input type="text" name="day_of_birth" value="<?php echo $_POST['day_of_birth']?>"/> </label> <br />
  <label> Месяц рождения: <input type="text" name="month_of_birth" value="<?php echo $_POST['month_of_birth']?>"/> </label> <br />
  <label> Год рождения: <input type="text" name="year_of_birth" value="<?php echo $_POST['year_of_birth']?>"/> </label> <br />
  <input type="submit" value="Submit" name="Submit" />
</form>
<p>
  <?php echo render_days($_POST['day_of_birth'], $_POST['month_of_birth'], $_POST['year_of_birth']) ?>
</p>

<?php
  function render_days($day, $month, $year)
  {
    if ( $day != "" && $month != "" && $year != "" )
    {
      if ( $day < 0 || $day > 31 )
        return "Wrong format";
      if ( $month < 0 || $month > 12 )
        return "Wrong format";
      if ( $year < 0 || $year > 2100 )
        return "Wrong format";
      $birth = mktime(0,0,0,$month,$day,$year);
      $now = time();
      return floor(($now - $birth)/(60*60*24))." days passed from your birthday";
    }
  }
?>
