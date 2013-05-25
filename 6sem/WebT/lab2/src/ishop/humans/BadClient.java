package ishop.humans;

import ishop.Shop;

public class BadClient extends Client {

	public BadClient(int id, String fio, Shop shop) {
		super(id, fio, shop);
	}

	protected boolean payOrNotToPay() {
		return false;
	}
}
