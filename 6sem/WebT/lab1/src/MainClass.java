import java.util.List;

import buses.Bus;
import buses.BusCollection;

import patients.Patient;
import patients.PatientCollection;


public class MainClass {

	/**
	 * @param args
	 */

	public static void main(String[] args) {
		if ( "--patients-db".equals(args[0]) ) {
			PatientCollection pc = new PatientCollection();
			pc.addPatient("Petrov", "Petr", "Petrovich", "ul. Ulica, dom. 77, kv. 89", "+375292154785", 178452145, "volchanka");
			pc.addPatient("Ivanov", "Ivan", "Ivanovich", "ul. Neulica, dom. 22, kv. 11", "+375295123543", 158452145, "prostuda");
			pc.addPatient("Sidorov", "Sidr", "Sidorovich", "ul. Somulica, dom. 143, kv. 177", "+375174574512", 221478563, "volchanka");

			if ( "--diagnosis".equals(args[1]) ) {
				List<Patient> res = pc.findByDiagnosis(args[2]);
				System.out.println("Patients with " + args[2] + ":");
				PatientCollection.printList(res);
			}
			else if ( "--medical-no".equals(args[1]) ) {
				List<Patient> res = pc.findByMedicalCardNo(Long.parseLong(args[2]), Long.parseLong(args[3]));
				System.out.println("Patients with medical card no between " + args[2] + " and " + args[3] + ":");
				PatientCollection.printList(res);
			}
		}
		else if ( "--buses-db".equals(args[0]) ) {
			BusCollection bc = new BusCollection();
			bc.addBus("Petrov P.P.", 1, 5, "Ikarus", 1990, 100500);
			bc.addBus("Ivanov I.I.", 2, 5, "Ikarus", 1993, 10000);
			bc.addBus("Sidorov S.S.", 3, 10, "Ikarus", 1989, 50000);

			if ( "--route-no".equals(args[1]) ) {
				List<Bus> res = bc.findByRouteNo(Long.parseLong(args[2]));
				System.out.println("Buses with route no " + args[2] + ":");
				BusCollection.printList(res);
			}
			else if ( "--age".equals(args[1]) ) {
				List<Bus> res = bc.findOlderThan(Integer.parseInt(args[2]), 2013);
				System.out.println("Buses with older than " + args[2] + " years:");
				BusCollection.printList(res);
			}
			else if ( "--mileage".equals(args[1]) ) {
				List<Bus> res = bc.findGreaterMileage(Long.parseLong(args[2]));
				System.out.println("Buses with mileage greater than " + args[2] + ":");
				BusCollection.printList(res);
			}
		}

	}
}

