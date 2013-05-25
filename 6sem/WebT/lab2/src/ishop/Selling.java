package ishop;

public class Selling {

	public Selling(GoodOrder goodOrder) {
		this.goodOrder = goodOrder;
		this.closed = false;
		this.successfull = false;
	}
	
	public boolean isClosed() {
		return closed;
	}
	
	public void setClosed(boolean successfull) {
		this.closed = true;
		this.successfull = successfull;
	}
	
	public boolean isSuccessfull() {
		return successfull;
	}
	
	public boolean isPaid() {
		return paid;
	}

	public void setPaid() {
		this.paid = true;
	}
	
	public GoodOrder getGoodOrder() {
		return goodOrder;
	}
	
	private GoodOrder goodOrder;
	private boolean paid;
	private boolean closed;
	private boolean successfull;
}
