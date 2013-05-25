package exams;

import java.util.List;

import exams.humans.Enrollee;
import exams.humans.Teacher;

public class Exam {

	public Exam(Teacher teacher) {
		this.teacher = teacher;
	}
	
	public void doExam(List<Enrollee> enrollees) {
		for ( Enrollee enrollee : enrollees ) {
			teacher.evaluateEnrolleeByExam(enrollee, this);
		}
	}

	private Teacher teacher;
}
