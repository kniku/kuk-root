package clserv.model.tables;

import clserv.model.tablespec.I_ColumnSpec;
import clserv.model.tablespec.I_IndexSpec;
import clserv.model.tablespec.I_TableSpec;
import clserv.model.tablespec.ImplColumnSpec;

public class ES_LOGS implements I_TableSpec {
	
	public static ImplColumnSpec ColumnLOGIN = clserv.model.tables.Columns.LOGIN;
	public static ImplColumnSpec ColumnENTRY_TYPE = clserv.model.tables.Columns.ENTRY_TYPE;
	public static ImplColumnSpec ColumnINFO = clserv.model.tables.Columns.INFO;
	public static ImplColumnSpec ColumnENTRY_TS = clserv.model.tables.Columns.ENTRY_TS;
	public static ImplColumnSpec ColumnORIGIN = clserv.model.tables.Columns.ORIGIN;
	public static ImplColumnSpec ColumnSOCKET_CLIENT = clserv.model.tables.Columns.SOCKET_CLIENT;
	
	private static final ImplColumnSpec[] theColumns =  {
		ColumnLOGIN,
		ColumnENTRY_TYPE,
		ColumnINFO,
		ColumnENTRY_TS,
		ColumnORIGIN,
		ColumnSOCKET_CLIENT
	};
	
	private static ES_LOGS mInstance = null;

	private ES_LOGS() {}
	public static I_TableSpec getInstance() {
		if (mInstance == null) mInstance = new ES_LOGS();
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
