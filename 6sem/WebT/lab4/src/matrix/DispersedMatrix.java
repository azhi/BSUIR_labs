package matrix;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class DispersedMatrix {
	
	public DispersedMatrix(DoubleLinkedCycleList elements, int n, int m) {
		this.elements = elements;
		this.n = n;
		this.m = m;
	}
	
	public DispersedMatrix(String filename) {
		elements = new DoubleLinkedCycleList();
		File file = new File(filename);
		Scanner scanner = null;
		try {
			scanner = new Scanner(file);
			n = scanner.nextInt();
			m = scanner.nextInt();
			for (int i = 0; i < n; ++i)
				for (int j = 0; j < m; ++j) {
					double val = scanner.nextDouble();
					if (val != 0.0)
						elements.add(new MatrixElement(val, i, j));
				}
		} catch (Exception e) {
			System.err.println(e.getMessage());
			e.printStackTrace();
		} finally {
			scanner.close();
		}
	}
	
	public int getN() {
		return n;
	}
	
	public int getM() {
		return m;
	}

	public DoubleLinkedCycleList getElements() {
		return elements;
	}

	@SuppressWarnings("unchecked")
	public DispersedMatrix multiply(DispersedMatrix other) {
		if (m != other.getN())
			throw new IllegalArgumentException("Matrix should have match dimensions");
		DoubleLinkedCycleList resElements = new DoubleLinkedCycleList();
		List<MatrixElement>[] rows = this.groupByRows();
		List<MatrixElement>[] columns = other.groupByColumns();
		for (int i = 0; i < n; i++)
			for (int j = 0; j < other.getM(); j++) {
				double value = 0.0;
				for (MatrixElement me : rows[i]) {
					for (MatrixElement ome : columns[j]) {
						if (ome.getX() == me.getY())
							value += me.getValue() * ome.getValue();
					}
				if (value != 0.0)
					resElements.add(new MatrixElement(value, i, j));
				}
			}
		return new DispersedMatrix(resElements, n, other.getM());
	}
	
	public DispersedMatrix add(DispersedMatrix other) throws IllegalArgumentException {
		if (n != other.getN() || m != other.getM())
			throw new IllegalArgumentException("Matrix should have same NxM");
		DoubleLinkedCycleList resElements = new DoubleLinkedCycleList();
		for (MatrixElement me : elements) {
			double value = me.getValue();
			for (MatrixElement ome : other.getElements()) {
				if (ome.getX() == me.getX() &&
					ome.getY() == me.getY()	) {
					value += ome.getValue();
					break;
				}
			}
			resElements.add(new MatrixElement(value, me.getX(), me.getY()));
		}
		
		boolean was = false;
		for (MatrixElement ome : other.getElements()) {
			was = false;
			for (MatrixElement me : elements) {
				if (ome.getX() == me.getX() &&
					ome.getY() == me.getY()	) {
					was = true;
					break;
				}
			}
			if (!was)
				resElements.add(new MatrixElement(ome.getValue(), ome.getX(), ome.getY()));
		}
		return new DispersedMatrix(resElements, n, m);
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public List[] groupByRows() {
		List[] res = new List[n];
		for (int i = 0; i < n; i++) {
			res[i] = new ArrayList<MatrixElement>();
			for (MatrixElement me : elements)
				if (me.getX() == i)
					res[i].add(me);
		}
		return res;
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public List[] groupByColumns() {
		List[] res = new List[m];
		for (int i = 0; i < m; i++) {
			res[i] = new ArrayList<MatrixElement>();
			for (MatrixElement me : elements)
				if (me.getY() == i)
					res[i].add(me);
		}
		return res;
	}
	
	@Override
	public String toString() {
		double[][] resMatrix = new double[n][m];
		for (MatrixElement me : elements)
			resMatrix[me.getX()][me.getY()] = me.getValue();
		StringBuilder res = new StringBuilder();
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j)
				res.append(resMatrix[i][j]).append(' ');
			res.append("\n");
		}
		return res.toString();
	}

	private DoubleLinkedCycleList elements;
	private int n;
	private int m;
}
