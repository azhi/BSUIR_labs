package square;

public class Point {

	public Point(int x, int y) {
		this.x = x;
		this.y = y;
	}

	public int getX() {
		return x;
	}
	
	public void setX(int x) {
		this.x = x;
	}
	
	public int getY() {
		return y;
	}
	
	public void setY(int y) {
		this.y = y;
	}
	
	private int x;
	private int y;
	
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Point [getX()=").append(getX()).append(", getY()=")
				.append(getY()).append("]");
		return builder.toString();
	}

}
