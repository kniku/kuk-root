package clserv.model.tables;

import clserv.model.tablespec.ImplColumnSpec;
import clserv.model.tablespec.I_ColumnSpec.ColumnType;

public class Columns {
	
	public static ImplColumnSpec LOGIN = new ImplColumnSpec("LOGIN", ColumnType.STRING, 3, null, true, false);
	
	public static String StrConstrENTRY_TYPE_In = "I";
	public static String StrConstrENTRY_TYPE_Out = "O";
	public static ImplColumnSpec ENTRY_TYPE = new ImplColumnSpec("ENTRY_TYPE", ColumnType.STRING, 1, new String [] {StrConstrENTRY_TYPE_In,StrConstrENTRY_TYPE_Out}, false, false);
	
	public static ImplColumnSpec INFO = new ImplColumnSpec("INFO", ColumnType.STRING, 256, null, false, true);
	
	public static ImplColumnSpec ENTRY_TS = new ImplColumnSpec("ENTRY_TS", ColumnType.TIMESTAMP, -1, null, true, false);
	public static String StrConstrORIGIN_Std = "S";
	public static String StrConstrORIGIN_Prelog = "P";
	public static String StrConstrORIGIN_Import = "I";
	public static ImplColumnSpec ORIGIN = new ImplColumnSpec("ORIGIN", ColumnType.STRING, 1, new String [] {StrConstrORIGIN_Std,StrConstrORIGIN_Prelog,StrConstrORIGIN_Import}, false, false);
	public static ImplColumnSpec SOCKET_CLIENT = new ImplColumnSpec("SOCKET_CLIENT", ColumnType.STRING, 256, null, false, true);
}
