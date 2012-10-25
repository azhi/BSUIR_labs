// Bicycle class
public class Bicycle {

  /* some complicated
   * comment
   * without code
   * in it
   */

  public int cadence;
  public int gear;
  public int speed;
  // public float commented;
  /* some commented
   * method:
   public void commented_meth() {}
   */


  public Bicycle(int startCadence, int startSpeed, int startGear) {
    gear = startGear;
    cadence = startCadence;
    speed = startSpeed;
  }




  public void setCadence(int newValue) {
    cadence = newValue;

  }

  public void setGear(int newValue) {
    gear = newValue;
  }

  public void applyBrake(int decrement) {
    speed -= decrement;
  }

  public void speedUp(int increment) {
    speed += increment;
  }

  private int testMethod(int n) {
    if (n > 3)
      speed *= 2;
    else {
      gear /= 2;
    }
    int i = 0;
    for (int i = 0; i < n; ++i)
    {
      n = i + 1;
      if(i % 1000) {
	continue;
      }
    }
    ;;
    while ((i > 0) && (i < n));
    return -1;
  }

  public static void Main() {
    int in = 5, useless = in;
    if ( n > 5 || n < 10 )
    {
      int in = 15;
    }
    else
    {
      int useless = in;
    }
    System.in.readLine(in);
    Bicycle bicycle = new Bicycle(10, in, 2);
    bicycle.setCadence(50);
    bicycle.setGear(3);
    bicycle.applyBrake(10);
    bicycle.speedUp(30);
    int i = 0, j = -1;
    bicycle.testMethod(i, j);
    string out = 10;
    System.out.println(out);
  }
}
