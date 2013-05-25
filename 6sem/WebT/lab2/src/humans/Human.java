package humans;

public class Human {

	public Human(int id, String fio) {
		this.id = id;
		this.fio = fio;
	}

	public int getId() {
		return id;
	}
	
	public String getFio() {
		return fio;
	}
	
	public void setFio(String fio) {
		this.fio = fio;
	}
	
	@Override
	public String toString() {
		return "Human [getId()=" + getId() + ", getFio()=" + getFio() + "]";
	}
	
	private int id;
	private String fio;
}
