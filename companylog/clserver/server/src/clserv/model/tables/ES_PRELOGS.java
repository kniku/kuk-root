package clserv.model.tables;

import clserv.model.tablespec.I_ColumnSpec;
import clserv.model.tablespec.I_IndexSpec;
import clserv.model.tablespec.I_TableSpec;
import clserv.model.tablespec.ImplColumnSpec;

public class ES_PRELOGS implements I_TableSpec {
	
	public static ImplColumnSpec ColumnLOGIN = clserv.model.tables.Columns.LOGIN;
	public static ImplColumnSpec ColumnENTRY_TYPE = clserv.model.tables.Columns.ENTRY_TYPE;
	public static ImplColumnSpec ColumnINFO = clserv.model.tables.Columns.INFO;
	public static ImplColumnSpec ColumnENTRY_TS = clserv.model.tables.Columns.ENTRY_TS;
	public static ImplColumnSpec ColumnSOCKET_CLIENT = clserv.model.tables.Columns.SOCKET_CLIENT;
	
	private static final ImplColumnSpec[] theColumns =  {
		ColumnLOGIN,
		ColumnENTRY_TYPE,
		ColumnINFO,
		ColumnENTRY_TS,
		ColumnSOCKET_CLIENT
	};
	
//	private static final ImplIndexSpec[] theIndexes =  {
//		new ImplIndexSpec("index_1", new String [] {"LOGIN", "ENTRY_TS"})
//	};
	private static ES_PRELOGS mInstance = null;
	private ES_PRELOGS() {}
	public static I_TableSpec getInstance() {
		if (mInstance == null) mInstance = new ES_PRELOGS();
		return mInstance;
	}
	
	@Override
	public I_ColumnSpec [] getColumns() {
		return theColumns; 
	}
	@Override
	public String getName() {
		return this.getClass().getSimpleName();
	}
	@Override
	public I_IndexSpec[] getIndexes() {
		return null;//theIndexes;
	}	
}
