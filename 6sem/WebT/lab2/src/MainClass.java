import ishop.Good;
import ishop.Shop;
import ishop.humans.Admin;
import ishop.humans.BadClient;
import ishop.humans.Client;
import ishop.humans.GoodClient;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import exams.Faculty;
import exams.humans.Enrollee;
import exams.humans.GoodTeacher;
import exams.humans.StrictTeacher;
import exams.humans.Teacher;


public class MainClass {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		if ( "--exams".equals(args[0]) ) {
			Faculty faculty = new Faculty(5);

			faculty.addTeacher(new Teacher(1, "Teacher"));
			faculty.addTeacher(new StrictTeacher(2, "StrictTeacher"));
			faculty.addTeacher(new GoodTeacher(3, "GoodTeacher"));

			try {
				faculty.registerEnrollee(new Enrollee(1, "Petrov P.P"));
				faculty.registerEnrollee(new Enrollee(2, "Ivanov I.I"));
				faculty.registerEnrollee(new Enrollee(3, "Sidorov S.S"));
				faculty.registerEnrollee(new Enrollee(4, "J. Smith"));
				faculty.registerEnrollee(new Enrollee(5, "C. Black"));
			} catch (Exception e) {
				e.printStackTrace();
			}

			faculty.endRegister();

			faculty.doExams();

			List<Enrollee> bestEnrollees = faculty.pickBestEnrollees(); 
			System.out.println("Picked enrollees:");
			for (Enrollee enrollee : bestEnrollees) {
				System.out.println(enrollee);
			}
		}
		else if ( "--shop".equals(args[0]) ) {
			Random random = new Random();
			Shop shop = new Shop(1, "ADMIN");
			Admin admin = shop.getAdmin();
			
			admin.addGood(new Good(1, "CPU", 1000));
			admin.addGood(new Good(2, "MB", 700));
			admin.addGood(new Good(3, "VideoCard", 1300));
			admin.addGood(new Good(4, "Memory", 500));
			admin.addGood(new Good(5, "HDD", 700));
			
			List<Client> clients = new ArrayList<Client>();
			clients.add(new Client(1, "Ivanov I.I.", shop));
			clients.add(new Client(2, "Petrov P.P.", shop));
			clients.add(new BadClient(3, "Sidorov S.S.", shop));
			clients.add(new GoodClient(4, "J. Smith", shop));
			
			for (Client client : clients) {
				int count = random.nextInt(5);
				for ( int i = 0; i < count; ++i )
					client.purchaseGood(shop.getGoods().get(random.nextInt(shop.getGoods().size())));
				System.out.println();
			}
		}
	}
}
