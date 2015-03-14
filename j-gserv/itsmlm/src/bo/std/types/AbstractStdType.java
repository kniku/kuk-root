package bo.std.types;


public abstract class AbstractStdType {
	private String mName;
	
	AbstractStdType(String iName) {
		mName = iName;
	}
	
	public String getName() { return mName; };

}
