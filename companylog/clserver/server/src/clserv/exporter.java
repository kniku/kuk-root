package clserv;

import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.HashMap;

import clserv.clienthandler.ClientHandlerFactory;
import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.model.ddl.DDLTools;
import clserv.model.tables.ES_LOGS;
import clserv.shared.Database;
import clserv.shared.GlobalConstants;
import clserv.tools.AppContext;


public class exporter {
	private static final String Version = "1.0";
	private static String mYear = null;
	private static String mMonth = null;
	private static HashMap<String, Mitarbeiter> logins = new HashMap<String, Mitarbeiter>();
	private static Database mDatabase = null;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if (args == null || args.length < 1) {
			show_usage();
		}
		parse_args(args);
		read_properties();
		try {
			Connection mQMSDB = mDatabase.getConnection();		
			reloadMitarbeiter(mQMSDB);
			System.out.println("===========================================");
			do_export(mQMSDB);
			mDatabase.releaseConnection(mQMSDB);
			shutdown();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SQLException e) {
			e.printStackTrace();
		}			
	}

	private static void do_export(Connection mQMSDB) {
		BufferedWriter w = null;
		GregorianCalendar cal = new GregorianCalendar();
		if (mYear != null) cal.set(Calendar.YEAR, Integer.parseInt(mYear));
		else mYear = Integer.toString(cal.get(Calendar.YEAR));
		if (mMonth != null) cal.set(Calendar.MONTH, Integer.parseInt(mMonth) - 1);
		else mMonth = Integer.toString(cal.get(Calendar.MONTH) + 1);
		if (mMonth.length() < 2) mMonth = "0" + mMonth;
		cal.set(Calendar.DAY_OF_MONTH, 1);
		cal.set(Calendar.HOUR_OF_DAY, 0);
		cal.set(Calendar.MINUTE, 0);
		cal.set(Calendar.SECOND, 0);
		cal.set(Calendar.MILLISECOND, 0);		
		Timestamp ts_untergrenze = new Timestamp(cal.getTimeInMillis());
		cal.set(Calendar.DAY_OF_MONTH, cal.getActualMaximum(Calendar.DAY_OF_MONTH));
		cal.set(Calendar.HOUR_OF_DAY, 23);
		cal.set(Calendar.MINUTE, 59);
		cal.set(Calendar.SECOND, 59);
		cal.set(Calendar.MILLISECOND, 999);
		Timestamp ts_obergrenze = new Timestamp(cal.getTimeInMillis());
		try {
			w = new BufferedWriter(new FileWriter(mYear+mMonth+"_export.pze", false));
			PreparedStatement StmtPrep = mQMSDB.prepareStatement(
					"select "+DDLTools.DDL_HELPER_create_columnlist(ES_LOGS.getInstance(), null)+
//					"select "+ES_LOGS.ColumnLOGIN.getName(null)+","+ES_LOGS.ColumnENTRY_TYPE.getName(null)+","+ES_LOGS.ColumnINFO.getName(null)+","+ES_LOGS.ColumnENTRY_TS.getName(null)+
					" from "+ES_LOGS.getInstance().getName()+
					" where "+ES_LOGS.ColumnENTRY_TS.getName(null)+"<=? and "+ES_LOGS.ColumnENTRY_TS.getName(null)+">=?"+
					" order by "+ES_LOGS.ColumnENTRY_TS.getName(null)
			);
			StmtPrep.setTimestamp(1, ts_obergrenze);
			StmtPrep.setTimestamp(2, ts_untergrenze);
			System.out.println("Bereich:");
			System.out.println("Von=[" + ts_untergrenze.toString() + "]");
			System.out.println("Bis=[" + ts_obergrenze.toString() + "]");
			StmtPrep.execute();
			ResultSet rs = StmtPrep.getResultSet();
			int count = 0;
			for (; rs.next() ;count++) {
				Mitarbeiter ma = logins.get(rs.getString(ES_LOGS.ColumnLOGIN.getName(null)));
				if (ma == null) {
					System.out.println("WARNING: Unbekannter Login: [" + rs.getString(ES_LOGS.ColumnLOGIN.getName(null)) + "]! Mitarbeiter nicht gefunden. Erzeuge anonymen Mitarbeiter...");
					ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(rs.getString(ES_LOGS.ColumnLOGIN.getName(null)));
				}
				if (ma != null) {
					DaoES_LOGS aLog = new DaoES_LOGS(rs.getString(ES_LOGS.ColumnLOGIN.getName(null)), rs.getTimestamp(ES_LOGS.ColumnENTRY_TS.getName(null)));
					aLog.setEntry_type(rs.getString(ES_LOGS.ColumnENTRY_TYPE.getName(null)));
					aLog.setInfo(rs.getString(ES_LOGS.ColumnINFO.getName(null)));
					String s = ClientHandlerFactory.createResponse_pzeData_alt_mit_Mitarbeiter(aLog, ma);
					w.write(s);
				}			
			}
			rs.close();
			StmtPrep.close();
			mQMSDB.rollback();
			w.close();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(-1);
		}
	}

	private static void parse_args(String[] args) {
		for (int i = 0; i < args.length ; i++) {
			if (args[i].startsWith("year=")) {
			    mYear = args[i].substring(5);
			}
			if (args[i].startsWith("month=")) {
			    mMonth = args[i].substring(6);
			}
		}
		if (mYear != null) System.out.println("Year: "+mYear);			
		if (mMonth != null) System.out.println("Month: "+mMonth);			
	}
	
	private static void read_properties() {
		java.util.Properties mProperties = new java.util.Properties();
		try {
			System.out.println("Lese clserv property file: "+GlobalConstants.CLServ_PROPERTY_FILE);
			FileInputStream f = AppContext.getPropertyFile();
			mProperties.load(f);
			f.close();
			
			System.out.println("Verbindung mit QMS Datenbank ("+mProperties.getProperty("DB_Connection", GlobalConstants.QMSDB_Default_Connect)+"/"+mProperties.getProperty("DB_User", GlobalConstants.QMSDB_Default_User)+")...");
			Class.forName(mProperties.getProperty("DB_Driver", GlobalConstants.QMSDB_Default_Driver));
			mDatabase = clserv.shared.Database.createInstance(mProperties.getProperty("DB_Driver", GlobalConstants.QMSDB_Default_Driver),
					mProperties.getProperty("DB_Connection", GlobalConstants.QMSDB_Default_Connect),
					mProperties.getProperty("DB_User", GlobalConstants.QMSDB_Default_User),
					mProperties.getProperty("DB_Password", GlobalConstants.QMSDB_Default_Pass),
					5);
			System.out.println("Check der Tabelle ES_LOGS oder Versuch, sie zu erzeugen...");
			Connection mQMSDB = mDatabase.getConnection();
			Database.checkOrCreateTables(mQMSDB);
			mDatabase.releaseConnection(mQMSDB);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			show_usage();
		} catch (IOException e) {
			e.printStackTrace();
			show_usage();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			show_usage();
		} catch (SQLException e) {
			e.printStackTrace();
			show_usage();
		} catch (Exception e) {
			e.printStackTrace();
			show_usage();
		}
	}
	
	private static void shutdown() {
		try {
			if (mDatabase != null) mDatabase.shutdown();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		System.out.println("Programm beendet.");
	}
	private static void show_usage() {
		System.out.println("EcoExporter, v" + Version);
		System.out.println("Aufruf: EcoExporter [year=YEAR] [month=MONTH(1-12)]");
		System.out.println("Beispiel: EcoExporter year=2008 month=03");
		System.exit(0);
	}

	private static void reloadMitarbeiter(Connection mQMSDB) {
		try {
			logins = mDatabase.reloadMitarbeiterWithConnection(mQMSDB, logins);
			System.out.println("Mitarbeiter geladen: "+logins.size());
		} catch (SQLException e) {
			e.printStackTrace();
		}
	}	
}
