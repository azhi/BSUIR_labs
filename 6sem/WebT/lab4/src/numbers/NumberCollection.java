package numbers;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.NoSuchElementException;

public class NumberCollection {
	
	public NumberCollection() {
		numbers = new ArrayList<Double>();
	}
	
	public void addNumber(double val) {
		int index = Collections.binarySearch(numbers, val);	
		if (index < 0)
			index = ~index;	
		numbers.add(index, val);
	}
	
	public void removeNumber(double val) throws NoSuchElementException {
		int index = Collections.binarySearch(numbers, val);	
		if (index < 0)
			throw new NoSuchElementException("Can't find number " + val + " in the list");
		numbers.remove(index);
	}
	
	public double getNearest(double val) {
		int index = Collections.binarySearch(numbers, val);	
		if (index < 0)
			index = ~index;	
		
		if (index == 0)
			return numbers.get(index);
		
		if ( Math.abs(numbers.get(index) - val) < Math.abs(numbers.get(index - 1) - val) )
			return numbers.get(index);
		else
			return numbers.get(index - 1);
	}

	private List<Double> numbers;
}
