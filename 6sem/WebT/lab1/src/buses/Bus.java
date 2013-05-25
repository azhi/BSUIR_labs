package buses;

public class Bus {
	public Bus(long id, String driver, long busNo, long routeNo, String model,
			int startYear, long mileage) {
		this.id = id;
		this.driver = driver;
		this.busNo = busNo;
		this.routeNo = routeNo;
		this.model = model;
		this.startYear = startYear;
		this.mileage = mileage;
	}
	
	public long getId() {
		return id;
	}
	
	public String getDriver() {
		return driver;
	}
	
	public void setDriver(String driver) {
		this.driver = driver;
	}
	
	public long getBusNo() {
		return busNo;
	}
	
	public void setBusNo(long busNo) {
		this.busNo = busNo;
	}
	
	public long getRouteNo() {
		return routeNo;
	}
	
	public void setRouteNo(long routeNo) {
		this.routeNo = routeNo;
	}
	
	public String getModel() {
		return model;
	}
	
	public void setModel(String model) {
		this.model = model;
	}
	
	public int getStartYear() {
		return startYear;
	}
	
	public void setStartYear(int startYear) {
		this.startYear = startYear;
	}
	
	public long getMileage() {
		return mileage;
	}
	
	public void setMileage(long mileage) {
		this.mileage = mileage;
	}
	
	@Override
	public String toString() {
		return "Bus [id=" + id + ", driver=" + driver + ", busNo=" + busNo
				+ ", routeNo=" + routeNo + ", model=" + model + ", startYear="
				+ startYear + ", mileage=" + mileage + "]";
	}


	private long id;
	private String driver;
	private long busNo;
	private long routeNo;
	private String model;
	private int startYear;
	private long mileage;
}
