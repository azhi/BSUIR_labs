import java.util.NoSuchElementException;

import matrix.DispersedMatrix;
import numbers.NumberCollection;

public class MainClass {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if ( "--numbers".equals(args[0]) ) {
			NumberCollection nc = new NumberCollection();
			nc.addNumber(5.5);
			nc.addNumber(10.0);
			nc.addNumber(1.1);
			nc.addNumber(67.9);
			nc.addNumber(15.2);
			nc.addNumber(19.7);
			
			try {
				nc.removeNumber(10.0);
			} catch (NoSuchElementException e) {
				System.err.println(e.getMessage());
			}
			
			double res1 = nc.getNearest(4.0);
			double res2 = nc.getNearest(20.0);
			System.out.printf("Nearest for 4.0: %.2f\n", res1);
			System.out.printf("Nearest for 20.0: %.2f\n", res2);
		}
		else if ( "--matrix".equals(args[0]) ) {
			DispersedMatrix m1 = new DispersedMatrix(args[1]);
			DispersedMatrix m2 = new DispersedMatrix(args[2]);
			System.out.println("m1:");
			System.out.println(m1);
			System.out.println("m2:");
			System.out.println(m2);
			
			DispersedMatrix sum = m1.add(m2);
			DispersedMatrix mul = m1.multiply(m2);
			
			System.out.println("sum:");
			System.out.println(sum);
			System.out.println("mul:");
			System.out.println(mul);
		}
	}
}
