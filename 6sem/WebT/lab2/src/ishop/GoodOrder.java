package ishop;

import ishop.humans.Client;

public class GoodOrder {

	public GoodOrder(Good good, Client client) {
		this.good = good;
		this.client = client;
	}
	
	public Good getGood() {
		return good;
	}
	
	public Client getClient() {
		return client;
	}
	
	@Override
	public String toString() {
		return "GoodOrder [getGood()=" + getGood() + ", getClient()="
				+ getClient() + "]";
	}
	
	private Good good;
	private Client client;
}
