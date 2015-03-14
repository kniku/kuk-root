/**
 * 
 */
package clserv.model.tablespec;

/**
 * @author kuk
 *
 */
public class ImplColumnSpec implements I_ColumnSpec {

	private String mName = null;
	private ColumnType mType;
	private int mLength;
	private boolean mPrimaryKey;
	private boolean mNull;
	private String [] mStringConstr;
	
	public ImplColumnSpec(String iName, ColumnType iType, int iLength, String [] iStringConstr, boolean iPrimaryKey, boolean iNull) {
		mName = iName;
		mType = iType;
		mLength = iLength > 0 ? iLength : 0;
		mStringConstr = iStringConstr;
		mPrimaryKey = iPrimaryKey;
		mNull = mPrimaryKey ? false : iNull;
	}
	
	@Override
	public int getLength() {
		return mLength;
	}

	@Override
	public String getName(String iTablePrefix) {
		return (iTablePrefix == null ? "" : iTablePrefix + ".") + mName;
	}

	@Override
	public ColumnType getType() {
		return mType;
	}

	@Override
	public boolean isNull() {
		return mNull;
	}

	@Override
	public boolean isPrimaryKey() {
		return mPrimaryKey;
	}

	@Override
	public String[] getStringConstraints() {
		return mStringConstr;
	}

}
