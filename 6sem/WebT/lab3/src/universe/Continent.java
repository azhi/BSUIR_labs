package universe;

public class Continent {

	public Continent(String name, double percent) throws IllegalArgumentException {
		this.setName(name);
		this.setPercent(percent);
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public double getPercent() {
		return percent;
	}
	
	public void setPercent(double percent) throws IllegalArgumentException {
		if (percent < 10.0)
			throw new IllegalArgumentException("{" + this.name + "} It's not continent, it's an island");
		this.percent = percent;
	}
	
	@Override
	public String toString() {
		return "Continent [Name=" + getName() + " Percent=" + getPercent() + "]";
	}

	private String name;
	private double percent;
}
