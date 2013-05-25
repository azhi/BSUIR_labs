package matrix;

public class MatrixElement {
	
	public MatrixElement(double value, int x, int y) {
		this.value = value;
		this.x = x;
		this.y = y;
	}
	
	public double getValue() {
		return value;
	}
	
	public int getX() {
		return x;
	}
	
	public int getY() {
		return y;
	}

	private double value;
	private int x;
	private int y;
}
