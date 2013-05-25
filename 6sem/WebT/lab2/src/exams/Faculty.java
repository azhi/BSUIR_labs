package exams;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import exams.humans.Enrollee;
import exams.humans.Teacher;

public class Faculty {

	public Faculty(int examsCount) {
		this.examsCount = examsCount;
		this.teachers = new ArrayList<Teacher>();
		this.enrollees = new ArrayList<Enrollee>();
		this.registerIsOver = false;
	}
	
	public List<Teacher> getTeachers() {
		return teachers;
	}
	
	public void addTeacher(Teacher teacher) {
		teachers.add(teacher);
	}
	
	public List<Enrollee> getEnrollees() {
		return enrollees;
	}
	
	public void registerEnrollee(Enrollee enrollee) throws Exception {
		if ( registerIsOver )
			throw new Exception("Register is over, you can't add enrollee now");
		enrollees.add(enrollee);
	}
	
	public void endRegister() {
		registerIsOver = true;
	}
	
	public void doExams() throws Exception {
		if ( !registerIsOver )
			throw new Exception("Register is not over yet, you can't do exama now");
		for ( int i = 0; i < examsCount; ++i ) {
			Teacher teacher = teachers.get(random.nextInt(teachers.size()));
			Exam exam = new Exam(teacher);
			exam.doExam(enrollees);
		}
	}
	
	public List<Enrollee> pickBestEnrollees() {
		float averageMark = 0;
		for (Enrollee enrollee : enrollees) {
			averageMark += enrollee.getAverageMark();
		}
		averageMark /= enrollees.size();
		
		List<Enrollee> res = new ArrayList<Enrollee>();
		for (Enrollee enrollee : enrollees) {
			if ( enrollee.getAverageMark() >= averageMark )
				res.add(enrollee);
		}
		return res;
	}
	
	private List<Teacher> teachers;
	private List<Enrollee> enrollees;
	private int examsCount;
	private boolean registerIsOver;
	
	private Random random = new Random();
}
