package square;

public class Segment {

	public Segment(Point p1, Point p2, int color) {
		this.p1 = p1;
		this.p2 = p2;
		this.color = color;
	}

	public Point getP1() {
		return p1;
	}
	
	public void setP1(Point p1) {
		this.p1 = p1;
	}
	
	public Point getP2() {
		return p2;
	}
	
	public void setP2(Point p2) {
		this.p2 = p2;
	}
	
	public int getColor() {
		return color;
	}

	public void setColor(int color) {
		this.color = color;
	}

	private Point p1;
	private Point p2;
	private int color;
	
	public final static int CL_BLACK = 0;
	public final static int CL_RED = 1;
	public final static int CL_GREEN = 2;
	public final static int CL_BLUE = 3;
	public final static int CL_WHITE = 4;
	
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Segment [getP1()=").append(getP1())
				.append(", getP2()=").append(getP2()).append(", getColor()=");
		switch (getColor()) {
			case CL_BLACK:
				builder.append("black");
				break;
			case CL_RED:
				builder.append("red");
				break;
			case CL_GREEN:
				builder.append("green");
				break;
			case CL_BLUE:
				builder.append("blue");
				break;
			case CL_WHITE:
				builder.append("white");
				break;
			default:
				builder.append("unknown");
				break;
		}
		builder.append("]");
		return builder.toString();
	}

}
