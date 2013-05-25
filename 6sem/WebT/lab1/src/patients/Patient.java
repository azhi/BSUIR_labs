package patients;

public class Patient {

	public Patient(long id, String lastName, String firstName,
			String middleName, String address, String phoneNo,
			long medicalCardNo, String diagnosis) {
		this.id = id;
		this.lastName = lastName;
		this.firstName = firstName;
		this.middleName = middleName;
		this.address = address;
		this.phoneNo = phoneNo;
		this.medicalCardNo = medicalCardNo;
		this.diagnosis = diagnosis;
	}

	public long getId() {
		return id;
	}
	
	public String getLastName() {
		return lastName;
	}
	public void setLastName(String lastName) {
		this.lastName = lastName;
	}
	
	public String getFirstName() {
		return firstName;
	}
	
	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}
	public String getMiddleName() {
		return middleName;
	}
	
	public void setMiddleName(String middleName) {
		this.middleName = middleName;
	}
	
	public String getAddress() {
		return address;
	}
	
	public void setAddress(String address) {
		this.address = address;
	}
	
	public String getPhoneNo() {
		return phoneNo;
	}
	
	public void setPhoneNo(String phoneNo) {
		this.phoneNo = phoneNo;
	}
	
	public long getMedicalCardNo() {
		return medicalCardNo;
	}
	
	public void setMedicalCardNo(long medicalCardNo) {
		this.medicalCardNo = medicalCardNo;
	}
	
	public String getDiagnosis() {
		return diagnosis;
	}
	
	public void setDiagnosis(String diagnosis) {
		this.diagnosis = diagnosis;
	}
	
	@Override
	public String toString() {
		return "Patient [id=" + id + ", lastName=" + lastName + ", firstName="
				+ firstName + ", middleName=" + middleName + ", address="
				+ address + ", phoneNo=" + phoneNo + ", medicalCardNo="
				+ medicalCardNo + ", diagnosis=" + diagnosis + "]";
	}

	
	private long id;
	private String lastName;
	private String firstName;
	private String middleName;
	private String address;
	private String phoneNo;
	private long medicalCardNo;
	private String diagnosis;
}
