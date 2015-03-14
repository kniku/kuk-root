package clserv.model.tablespec;

public class ImplIndexSpec implements I_IndexSpec {

	private String mName;
	private String [] mColumns;
	
	public ImplIndexSpec(String iName, String [] iColumns) {
		mName = iName;
		mColumns = iColumns;
	}
	@Override
	public String[] getColumns() {
		return mColumns;
	}

	@Override
	public String getName() {
		return mName;
	}

}
