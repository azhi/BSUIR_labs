package patients;

import java.util.ArrayList;
import java.util.List;

public class PatientCollection {
	
	public PatientCollection() {
		patients = new ArrayList<Patient>(); 
	}

	public void addPatient(String lastName, String firstName,
			String middleName, String address, String phoneNo,
			long medicalCardNo, String diagnosis) {
		Patient patient = new Patient(id, lastName, firstName,
				middleName, address, phoneNo, medicalCardNo,
				diagnosis);
		patients.add(patient);
		id++;
	}
	
	public List<Patient> findByDiagnosis(String diagnosis) {
		List<Patient> res = new ArrayList<Patient>();
		for (Patient patient : patients) {
			if ( patient.getDiagnosis().equals(diagnosis) )
				res.add(patient);
		}
		return res;
	}
	
	public List<Patient> findByMedicalCardNo(long lowerBound, long upperBound) {
		List<Patient> res = new ArrayList<Patient>();
		for (Patient patient : patients) {
			if ( (patient.getMedicalCardNo() >= lowerBound) && 
					(patient.getMedicalCardNo() <= upperBound))
				res.add(patient);
		}
		return res;
	}
	
	static public void printList(List<Patient> list) {
		for (Patient patient : list) {
			System.out.println(patient.toString());
		}
	}

	private List<Patient> patients;
	static long id = 0;
}
