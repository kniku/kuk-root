package bo.std.types;

public abstract class AbstractStdTypeStructure extends AbstractStdType {
	protected AbstractStdTypeStructure(String iName) {
		super(iName);
	}
	public abstract int getLastMemberIndex();
	public abstract String getMemberLabelByIndex(int iIndex);		
}
