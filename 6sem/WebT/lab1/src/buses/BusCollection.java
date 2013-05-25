package buses;

import java.util.ArrayList;
import java.util.List;


public class BusCollection {
	public BusCollection() {
		buses = new ArrayList<Bus>();
	}
	
	public void addBus(String driver, long busNo, long routeNo, String model,
			int startYear, long mileage) {
		Bus bus = new Bus(id, driver, busNo, routeNo, model, startYear, mileage);
		buses.add(bus);
		id++;
	}
	
	public List<Bus> findByRouteNo(long routeNo) {
		List<Bus> res = new ArrayList<Bus>();
		for (Bus bus : buses) {
			if ( bus.getRouteNo() == routeNo )
				res.add(bus);
		}
		return res;
	}
	
	public List<Bus> findOlderThan(long age, long currentYear) {
		List<Bus> res = new ArrayList<Bus>();
		for (Bus bus : buses) {
			if ( currentYear - bus.getStartYear() >= age )
				res.add(bus);
		}
		return res;
	}
	
	public List<Bus> findGreaterMileage(long mileage) {
		List<Bus> res = new ArrayList<Bus>();
		for (Bus bus : buses) {
			if ( bus.getMileage() >= mileage )
				res.add(bus);
		}
		return res;
	}
	
	static public void printList(List<Bus> list) {
		for (Bus bus : list) {
			System.out.println(bus.toString());
		}
	}
	
	private List<Bus> buses;
	static long id = 0;
}
