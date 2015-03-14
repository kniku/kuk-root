package clserv.model.ddl;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.ArrayList;

import org.apache.log4j.Logger;

import clserv.model.tablespec.I_ColumnSpec;
import clserv.model.tablespec.I_IndexSpec;
import clserv.model.tablespec.I_TableSpec;
import clserv.model.tablespec.I_ColumnSpec.ColumnType;


public final class DDLTools {
	private static Logger logger = Logger.getLogger(DDLTools.class);

	// unterstuetzte Datenbanken
	public static enum DDL_DATABASE_DIALECT {ORACLE, POSTGRE, UNKNOWN, NOT_CONNECTED}
	
	// default dialekt
	private static DDL_DATABASE_DIALECT DialectDefault = DDL_DATABASE_DIALECT.POSTGRE;
	
	/**
	 * @param iConnection Die Datenbankverbindung
	 * @return Der Datenbanktyp dieser Verbindung
	 */
	public final static DDL_DATABASE_DIALECT DDL_identify_database(Connection iConnection) {
		DDL_DATABASE_DIALECT r = DDL_DATABASE_DIALECT.NOT_CONNECTED;
		String dbname;
		try {
			dbname = iConnection.getMetaData().getDatabaseProductName().toUpperCase();
			if (dbname.contains("POSTGRE")) {
				r = DDL_DATABASE_DIALECT.POSTGRE;
			} else if (dbname.contains("ORACLE")) {
				r = DDL_DATABASE_DIALECT.ORACLE;
			} else {
				r = DDL_DATABASE_DIALECT.POSTGRE;
//				r = DDL_DATABASE_DIALECT.UNKNOWN;
			}
		} catch (SQLException e) {
			logger.fatal(e);
		}
		
		return r;
	}
	
	private final static ArrayList<String> DDL_ORACLE_create_table(I_TableSpec iTableSpec) throws Exception {
		// TODO: StringConstraint support
		String cre = "create table " + iTableSpec.getName() + " (";
		String pk = "";
		for (I_ColumnSpec iColumn : iTableSpec.getColumns()) {
			cre += iColumn.getName(null) + " ";
			if (iColumn.getType() == ColumnType.STRING) {
				cre += "varchar2(" + iColumn.getLength() + ")";
			} else if (iColumn.getType() == ColumnType.NUMERIC) {
				cre += "numeric(" + iColumn.getLength() + ")";				
			} else if (iColumn.getType() == ColumnType.TIMESTAMP) {
				cre += "date";				
			} else {
				throw new Exception("unknown column type");
			}
			if (iColumn.isPrimaryKey()) {
				if (pk.length() > 0) {
					pk += ",";
				}
				pk += iColumn.getName(null);
			}
			cre += ",";
		}
		cre += "primary key (" + pk + "))";
		
		ArrayList<String> r = new ArrayList<String>();
		r.add(cre);
		
		String idx;
		if (iTableSpec.getIndexes() != null) {
			for (I_IndexSpec iIndex : iTableSpec.getIndexes()) {
				idx = "create index " + iIndex.getName() + " on " + iTableSpec.getName() + "(";
				boolean firstColumn = true;
				for (String iColumn : iIndex.getColumns()) {
					if (!firstColumn) idx += ",";
					idx += iColumn;
					firstColumn = false;
				}
				idx += ")";
				r.add(idx);
			}			
		}
		return r;
	};

	private final static ArrayList<String> DDL_POSTGRE_create_table(I_TableSpec iTableSpec) throws Exception {
		// TODO: StringConstraint support
		String cre = "create table " + iTableSpec.getName() + " (";
		String pk = "";
		for (I_ColumnSpec iColumn : iTableSpec.getColumns()) {
			cre += iColumn.getName(null) + " ";
			if (iColumn.getType() == ColumnType.STRING) {
				cre += "varchar(" + iColumn.getLength() + ")";
			} else if (iColumn.getType() == ColumnType.NUMERIC) {
				cre += "numeric(" + iColumn.getLength() + ")";				
			} else if (iColumn.getType() == ColumnType.TIMESTAMP) {
				cre += "timestamp";				
			} else {
				throw new Exception("unknown column type");
			}
			if (iColumn.isPrimaryKey()) {
				if (pk.length() > 0) {
					pk += ",";
				}
				pk += iColumn.getName(null);
			}
			cre += ",";
		}
		cre += "primary key (" + pk + "))";

		ArrayList<String> r = new ArrayList<String>();
		r.add(cre);
		
		String idx;
		if (iTableSpec.getIndexes() != null) {
			for (I_IndexSpec iIndex : iTableSpec.getIndexes()) {
				idx = "create index " + iIndex.getName() + " on " + iTableSpec.getName() + "(";
				boolean firstColumn = true;
				for (String iColumn : iIndex.getColumns()) {
					if (!firstColumn) idx += ",";
					idx += iColumn;
					firstColumn = false;
				}
				idx += ")";
				r.add(idx);
			}			
		}
		return r;
	}

	public static ArrayList<String> DDL_STMT_create_table(DDL_DATABASE_DIALECT iDialect, I_TableSpec iTableSpec) throws Exception {
		if (iDialect == DDL_DATABASE_DIALECT.ORACLE) {
			return DDL_ORACLE_create_table(iTableSpec);
		} else if (iDialect == DDL_DATABASE_DIALECT.POSTGRE) {
			return DDL_POSTGRE_create_table(iTableSpec);
		} else {
			DDL_STMT_create_table(DialectDefault, iTableSpec);
		}
		return null;
	};
	
	public static String DDL_HELPER_create_columnlist(I_TableSpec iTableSpec, String iTablePrefix) {
		String r = "";
		boolean first = true;
		for (I_ColumnSpec iColumn : iTableSpec.getColumns()) {
			if (!first) {
				r += ",";
			}
			r += iColumn.getName(iTablePrefix);
			first = false;
		}		
		return r;
	}
}
