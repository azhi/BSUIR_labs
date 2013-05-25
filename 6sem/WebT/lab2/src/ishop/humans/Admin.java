package ishop.humans;

import ishop.Good;
import ishop.GoodOrder;
import ishop.Selling;
import ishop.Shop;
import humans.Human;

public class Admin extends Human {

	public Admin(int id, String fio, Shop shop) {
		super(id, fio);
		this.shop = shop;
	}
	
	public Selling registerGoodOrder(GoodOrder goodOrder) {
		return new Selling(goodOrder);
	}
	
	public void registerSelling(Selling selling) {
		if ( !selling.isPaid() ) {
			shop.addClientToBlackList(selling.getGoodOrder().getClient());
			selling.setClosed(false);
		}
		else {
			selling.setClosed(true);
		}
		shop.addSelling(selling);
	}
	
	public void addGood(Good good) {
		shop.addGood(good);
	}

	@Override
	public String toString() {
		return "Admin [getId()=" + getId() + ", getFio()=" + getFio() + "]";
	}
	
	private Shop shop;
}
