package universe;

public class NotEnoughSpaceException extends Exception {

    /**
	 * 
	 */
	private static final long serialVersionUID = 8482388327423774693L;

	public NotEnoughSpaceException() {}

    public NotEnoughSpaceException(String message)
    {
       super(message);
    }
}
