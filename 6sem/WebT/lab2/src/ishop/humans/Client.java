package ishop.humans;

import java.util.Random;

import ishop.Good;
import ishop.GoodOrder;
import ishop.Selling;
import ishop.Shop;
import humans.Human;

public class Client extends Human {

	public Client(int id, String fio, Shop shop) {
		super(id, fio);
		this.shop = shop;
	}
	
	public void purchaseGood(Good good) {
		GoodOrder goodOrder = new GoodOrder(good, this);
		Selling selling;
		try {
			selling = shop.registerGoodOrder(goodOrder);
			if ( payOrNotToPay() )
				selling.setPaid();
			shop.registerSelling(selling);
			if ( selling.isSuccessfull() )
				System.out.println(selling.getGoodOrder().getGood() + " sold successfully!");
			else
				System.out.println(selling.getGoodOrder().getGood() + " is not sold!");
		} catch (Exception e) {
			System.err.println(this + " is bad client, he got banned!");
		}
	}
	
	protected boolean payOrNotToPay() {
		return random.nextBoolean();
	}
	
	@Override
	public String toString() {
		return "Client [getId()=" + getId() + ", getFio()=" + getFio() + "]";
	}
	
	private Shop shop;
	private Random random = new Random();
}
