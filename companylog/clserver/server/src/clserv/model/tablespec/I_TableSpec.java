package clserv.model.tablespec;


public interface I_TableSpec {
	public String getName();
	public I_ColumnSpec[] getColumns();
	public I_IndexSpec[] getIndexes();
}
