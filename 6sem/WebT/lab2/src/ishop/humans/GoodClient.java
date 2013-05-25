package ishop.humans;

import ishop.Shop;

public class GoodClient extends Client {

	public GoodClient(int id, String fio, Shop shop) {
		super(id, fio, shop);
	}

	@Override
	protected boolean payOrNotToPay() {
		return true;
	}
}
