package exams.humans;

import exams.Exam;
import exams.Mark;

public class GoodTeacher extends Teacher {

	public GoodTeacher(int id, String fio) {
		super(id, fio);
	}

	@Override
	public void evaluateEnrolleeByExam(Enrollee enrollee, Exam exam) {
		Mark mark = new Mark(exam, this, 4 + random.nextInt(6));
		enrollee.addMark(mark);
	}
}