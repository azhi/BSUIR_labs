package square;

public class Square {

	public Square(Segment[] segments) {
		this.segments = segments;
	}
	
	public void setSide(int x) {
		Point p1 = segments[0].getP1();
		Point p2 = new Point(p1.getX(), p1.getY() + x);
		Point p3 = new Point(p1.getX() + x, p1.getY() + x);
		Point p4 = new Point(p1.getX() + x, p1.getY());
		buildFromPoints(p1, p2, p3, p4);
	}
	
	public void scale(double factor) {
		Point b_p1 = segments[0].getP1();
		Point b_p2 = segments[1].getP1();
		Point b_p3 = segments[2].getP1();
		Point b_p4 = segments[3].getP1();
		
		Point p1 = b_p1;
		
		Point p2 = new Point(b_p2.getX(),
				             (int) Math.round(b_p2.getY() + (b_p2.getY() - b_p1.getY()) * factor));
		
		Point p3 = new Point((int) Math.round(b_p3.getX() + (b_p3.getX() - b_p1.getX()) * factor),
				             (int) Math.round(b_p3.getY() + (b_p3.getY() - b_p1.getY()) * factor));
		
		Point p4 = new Point((int) Math.round(b_p4.getX() + (b_p4.getX() - b_p1.getX()) * factor),
				             b_p4.getY());
		
		buildFromPoints(p1, p2, p3, p4);
	}
	
	public void flip(int angle) throws IllegalArgumentException {
		Point b_p1 = segments[0].getP1();
		Point b_p2 = segments[1].getP1();
		Point b_p3 = segments[2].getP1();
		Point b_p4 = segments[3].getP1();
		
		Point p1 = null;
		Point p2 = null;
		Point p3 = null;
		Point p4 = null;
		
		switch (angle) {
			case 90:
				p1 = b_p4; p2 = b_p1; p3 = b_p2; p4 = b_p3;
				break;
			case 180:
				p1 = b_p3; p2 = b_p4; p3 = b_p1; p4 = b_p2;
				break;
			case -90:
				p1 = b_p2; p2 = b_p3; p3 = b_p4; p4 = b_p1;
				break;
			case 0:
				p1 = b_p2; p2 = b_p2; p3 = b_p3; p4 = b_p4;
				break;
			default:
				throw new IllegalArgumentException("angle should be 90, 180, -90 or 0");
		}
		
		buildFromPoints(p1, p2, p3, p4);
	}
	
	public void changeColor(int index, int color) throws IllegalArgumentException {
		if (color != Segment.CL_BLACK &&
			color != Segment.CL_BLUE &&
			color != Segment.CL_GREEN &&
			color != Segment.CL_RED &&
			color != Segment.CL_WHITE)
			throw new IllegalArgumentException("Unknow color");
		if (index < 0 || index > 3)
			throw new IllegalArgumentException("Index should be in [0, 3]");
		segments[index].setColor(color);
	}
	
	private void buildFromPoints(Point p1, Point p2, Point p3, Point p4) {
		segments[0] = new Segment(p1, p2, segments[0].getColor());
		segments[1] = new Segment(p2, p3, segments[1].getColor());
		segments[2] = new Segment(p3, p4, segments[2].getColor());
		segments[3] = new Segment(p4, p1, segments[3].getColor());
	}
	
	private Segment[] segments;
	
	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("Square [\n");
		for (Segment segment : segments)
			builder.append(segment).append("\n");
		builder.append("]\n");
		return builder.toString();
	}

}
