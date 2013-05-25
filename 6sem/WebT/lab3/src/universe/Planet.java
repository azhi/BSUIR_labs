package universe;

import java.util.ArrayList;
import java.util.List;

public class Planet {

	public Planet(String name) {
		this.setName(name);
		continents = new ArrayList<Continent>();
	}

	public void addContinent(Continent continent) throws NotEnoughSpaceException {
		double owned_land = 0.0;
		for (Continent con : continents)
			owned_land += con.getPercent();
		owned_land += continent.getPercent();
		if (owned_land > 50.0)
			throw new NotEnoughSpaceException("{" + continent.getName() + "} This planet is overloaded with continents, go found another one!");
		continents.add(continent);
	}
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		if (name.length() > 8)
			throw new IllegalArgumentException("{" + name + "} Your planet's name is too long, you are not welcome!");
		this.name = name;
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Planet [name=").append(getName());
		builder.append(", continents_count=").append(continents.size()).append("\n");
		for (Continent continent : continents) {
			builder.append("  ").append(continent).append("\n");
		}
		builder.append("]");
		return builder.toString();
	}
	
	private String name;
	private List<Continent> continents;
}
