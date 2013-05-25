package exams.humans;

import humans.Human;

import java.util.ArrayList;
import java.util.List;

import exams.Mark;

public class Enrollee extends Human {

	public Enrollee(int id, String fio) {
		super(id, fio);
		marks = new ArrayList<Mark>();
	}

	public List<Mark> getMarks() {
		return marks;
	}

	public void addMark(Mark mark) {
		marks.add(mark);
	}
	
	public float getAverageMark() {
		if ( cachedAverageMark < 0 ) {
			float res = 0;
			for ( Mark mark : marks ) {
				res += mark.getValue();
			}
			res /= marks.size();
			cachedAverageMark = res;
			return res;
		}
		else
			return cachedAverageMark;
	}

	@Override
	public String toString() {
		return "Enrollee [getId()=" + getId() + ", getFio()=" + getFio()
				+ ", getAverageMark()=" + getAverageMark() + "]";
	} 
	
	private List<Mark> marks;
	private float cachedAverageMark = -1;
}
