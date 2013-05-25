import square.Point;
import square.Segment;
import square.Square;
import universe.Continent;
import universe.NotEnoughSpaceException;
import universe.Planet;


public class MainClass {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		if ( "--universe".equals(args[0]) ) {
			Planet planet1 = null;
			@SuppressWarnings("unused")
			Planet planet2 = null;
			try {
				planet1 = new Planet("Earth");
				planet2 = new Planet("Laomedeia");
			} catch (IllegalArgumentException e) {
				System.err.println(e.getMessage());
			}
			
			
			try {
				Continent continent = new Continent("Great Britain", 1);
				planet1.addContinent(continent);
			} catch (IllegalArgumentException e) {
				System.err.println(e.getMessage());
			}
			
			try {
				Continent continent = new Continent("Europe", 20);
				planet1.addContinent(continent);
				continent = new Continent("Africa", 10);
				planet1.addContinent(continent);
				continent = new Continent("America", 10);
				planet1.addContinent(continent);
				continent = new Continent("Australia", 15);
				planet1.addContinent(continent);
			} catch (NotEnoughSpaceException e) {
				System.err.println(e.getMessage());
			}

			System.out.println(planet1);
		}
		else if ( "--square".equals(args[0]) ) {
			Segment[] segments = new Segment[4];
			Point p1 = new Point(0, 0);
			Point p2 = new Point(0, 5);
			Point p3 = new Point(5, 0);
			Point p4 = new Point(5, 5);
			segments[0] = new Segment(p1, p2, Segment.CL_BLACK);
			segments[1] = new Segment(p2, p3, Segment.CL_BLUE);
			segments[2] = new Segment(p3, p4, Segment.CL_RED);
			segments[3] = new Segment(p4, p1, Segment.CL_WHITE);
			Square square = new Square(segments);
			
			try {
				square.flip(90);
				square.flip(45);
			} catch (IllegalArgumentException e) {
				System.err.println(e.getMessage());
			}
			
			try {
				square.changeColor(0, Segment.CL_WHITE);
				square.changeColor(5, Segment.CL_WHITE);
			} catch (IllegalArgumentException e) {
				System.err.println(e.getMessage());
			}
			
			System.out.println(square);
		}
	}
}
