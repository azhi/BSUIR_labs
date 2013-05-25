package ishop;

public class Good {

	public Good(int id, String name, long price) {
		this.id = id;
		this.name = name;
		this.price = price;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public long getPrice() {
		return price;
	}
	
	public void setPrice(long price) {
		this.price = price;
	}
	
	public int getId() {
		return id;
	}
	
	@Override
	public String toString() {
		return "Good [getId()=" + getId() + ", getName()=" + getName()
				+ ", getPrice()=" + getPrice() + "]";
	}
	
	private int id;
	private String name;
	private long price;
}
