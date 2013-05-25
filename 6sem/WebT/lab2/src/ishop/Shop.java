package ishop;

import ishop.humans.Admin;
import ishop.humans.Client;

import java.util.ArrayList;
import java.util.List;

public class Shop {

	public Shop(int adminId, String adminFio) {
		admin = new Admin(adminId, adminFio, this);
		
		goods = new ArrayList<Good>();
		sellings = new ArrayList<Selling>();
		blackClientsIds = new ArrayList<Integer>();
	}
	
	private boolean isInBlackList(Client client) {
		for (Integer id : blackClientsIds) {
			if ( id == client.getId() )
				return true;
		}
		return false;
	}
	
	public Admin getAdmin() {
		return admin;
	}

	public List<Good> getGoods() {
		return goods;
	}
	
	public void addGood(Good good) {
		goods.add(good);
	}

	public List<Selling> getSellings() {
		return sellings;
	}
	
	public void addSelling(Selling selling) {
		sellings.add(selling);
	}

	public void addClientToBlackList(Client client) {
		blackClientsIds.add(client.getId());
	}
	
	public Selling registerGoodOrder(GoodOrder goodOrder) throws Exception {
		if ( isInBlackList(goodOrder.getClient()) )
			throw new Exception("Bad client. Screw you!");
		return admin.registerGoodOrder(goodOrder);
	}
	
	public void registerSelling(Selling selling) {
		admin.registerSelling(selling);
	}
	
	private List<Good> goods;
	private List<Selling> sellings;
	private Admin admin;
	private List<Integer> blackClientsIds;

}
