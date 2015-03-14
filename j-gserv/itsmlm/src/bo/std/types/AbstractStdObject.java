package bo.std.types;

public abstract class AbstractStdObject {
	private AbstractStdType mType;
	protected AbstractStdObject(AbstractStdType mType) {
		this.mType = mType;
	}
	public AbstractStdType getType() {
		return mType;
	}
}
