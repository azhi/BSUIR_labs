package exams.humans;

import exams.Exam;
import exams.Mark;

public class StrictTeacher extends Teacher {

	public StrictTeacher(int id, String fio) {
		super(id, fio);
	}

	@Override
	public void evaluateEnrolleeByExam(Enrollee enrollee, Exam exam) {
		Mark mark = new Mark(exam, this, random.nextInt(7));
		enrollee.addMark(mark);
	}
}
