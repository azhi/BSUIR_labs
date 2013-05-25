package exams.humans;

import humans.Human;

import java.util.Random;

import exams.Exam;
import exams.Mark;

public class Teacher extends Human {

	public Teacher(int id, String fio) {
		super(id, fio);
	}
	
	public void evaluateEnrolleeByExam(Enrollee enrollee, Exam exam) {
		Mark mark = new Mark(exam, this, random.nextInt(10));
		enrollee.addMark(mark);
	}

	@Override
	public String toString() {
		return "Teacher [getId()=" + getId() + ", getFio()=" + getFio() + "]";
	}

	protected Random random = new Random();
}
