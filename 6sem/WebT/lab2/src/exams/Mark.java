package exams;

import exams.humans.Teacher;

public class Mark {

	public Mark(Exam exam, Teacher teacher, int value) {
		this.exam = exam;
		this.teacher = teacher;
		this.value = value;
	}

	public int getValue() {
		return value;
	}
	
	public void setValue(int value) {
		this.value = value;
	}
	
	public Exam getExam() {
		return exam;
	}
	
	public Teacher getTeacher() {
		return teacher;
	}
	
	private Teacher teacher;
	private Exam exam;
	private int value;
}
