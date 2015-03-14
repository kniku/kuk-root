package bo.std.types;

public abstract class AbstractStdObjectStructure extends AbstractStdObject {
	protected AbstractStdObjectStructure(AbstractStdType mType) {
		super(mType);
	}
	public abstract String getValueStringByIndex(int iIndex);
}
