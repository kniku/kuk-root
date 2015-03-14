package clserv.model.tablespec;

public interface I_ColumnSpec {

	public enum ColumnType {STRING, TIMESTAMP, NUMERIC}
	
	public String getName(String iTablePrefix);
	public ColumnType getType();
	public int getLength();
//	public int getScale();
	public boolean isNull();
	public boolean isPrimaryKey();
	public String [] getStringConstraints();
}
