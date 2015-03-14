package clserv.shared;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.HashMap;

import org.apache.log4j.Logger;

import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.model.dao.DaoES_PRELOGS;
import clserv.model.ddl.DDLTools;
import clserv.model.ddl.DDLTools.DDL_DATABASE_DIALECT;
import clserv.model.tables.ES_LOGS;
import clserv.model.tables.ES_PRELOGS;
import clserv.model.tables.Schema;
import clserv.model.tablespec.I_TableSpec;
import clserv.tools.AppContext;


public final class Database {
	private static Logger logger = Logger.getLogger(Database.class);
	private static ConnectionPool mConnPool= null;

	// Singleton ...
	private static Database singleInstance = null;	
	private Database(String iDriver, String iConnect, String iUser, String iPass, int iMaxPoolSize) {
		mConnPool = new ConnectionPool(iDriver, iConnect, iUser, iPass, iMaxPoolSize);
		logger.info("Verbindung mit QMS Datenbank: Driver="+iDriver+", Connect="+iConnect+", User="+iUser);
	}
	public static Database createInstance(String iDriver, String iConnect, String iUser, String iPass, int iMaxPoolSize) {
		if (singleInstance == null) {
			singleInstance = new Database(iDriver, iConnect, iUser, iPass, iMaxPoolSize);
		}
		return singleInstance;
	}
	public static Database getInstance() {
		return singleInstance;
	}
	// ...Singleton
	
	/**
	 * Prueft, ob die Datenbank aktuell zu Verfuegung steht.
	 * @return Verfuegbar -> true, sonst false.
	 */
	public boolean isAvailable() {
		boolean r = false;
		PreparedStatement s = null;
		Connection c = null;
		try {
			c = getConnection();
			s = c.prepareStatement("select login from es_logs where login='XXX' and entry_ts is null");
			s.execute();
			r = true;
		} catch (Exception e) {
			logger.error(e);
			mConnPool.closeAllConnections();
			r = false;
		} finally {
			try {
				releaseConnection(c);
				if (s != null) {
					s.close();
				}
			} catch (SQLException e) {
				logger.error(e);
			}
		}
		return r;
	}
	
	public HashMap<String, Mitarbeiter> reloadMitarbeiterWithConnection(Connection iConnection, HashMap<String, Mitarbeiter> iHash) throws SQLException {
		if (iHash == null) {
			iHash = new HashMap<String, Mitarbeiter>();
		} else {
			iHash.clear();
		}
		Statement s = iConnection.createStatement();
		s.execute(
			"SELECT QMPERNNAME,QMPERVNAME,QMPERTELDW,QMPERTELPR,QMPERTELFA,QMPERTELP2," + 
	 		"QMPERTHYPR,QMPERKUBEZ,QMPERGESCH,QMPERTHYFA,QMPERPGRPR,QMPERAKTIV,QMPERFPOSN FROM QMDB10"
		);
//		s.execute("select QMPERKUBEZ,QMPERAKTIV,QMPERVNAME,QMPERNNAME,QMPERTELDW,QMPERGESCH from qmdb10");
		ResultSet rs = s.getResultSet();
		int count = 0, aktivcount = 0;
		for (; rs.next() ;count++) {
			Mitarbeiter mitarbeiter = new Mitarbeiter(rs.getString("QMPERKUBEZ").toLowerCase());
			mitarbeiter.setVname(rs.getString("QMPERVNAME"));
			mitarbeiter.setName(rs.getString("QMPERNNAME"));
			mitarbeiter.setDurchwahl(rs.getString("QMPERTELDW"));
			mitarbeiter.setAutonummer(rs.getString("QMPERPGRPR"));
			mitarbeiter.setHandyfirma(rs.getString("QMPERTHYFA"));
			mitarbeiter.setHandyprivat(rs.getString("QMPERTHYPR"));
			mitarbeiter.setTelfirma(rs.getString("QMPERTELFA"));
			mitarbeiter.setTelprivat(rs.getString("QMPERTELPR"));
			mitarbeiter.setTelprivat2(rs.getString("QMPERTELP2"));
			mitarbeiter.setPosition(rs.getString("QMPERFPOSN"));
			if (rs.getString("QMPERAKTIV").equals("J")) {
				mitarbeiter.setAktiv(true);
				aktivcount++;
			} else {
				mitarbeiter.setAktiv(false);
			}
			if (rs.getString("QMPERGESCH").equals("W")) {
				mitarbeiter.setGender(Mitarbeiter.Gender.FEMALE);
			} else {
				mitarbeiter.setGender(Mitarbeiter.Gender.MALE);
			}
			iHash.put(mitarbeiter.getLogin(), mitarbeiter);
		}
		rs.close();
		s.close();
		return iHash;
	}
	
	public HashMap<String, Mitarbeiter> reloadMitarbeiter(Connection iConnection, HashMap<String, Mitarbeiter> iHash) {
		boolean localTransaction = false;
		try {
			if (iConnection == null) {
				iConnection = getConnection();
				localTransaction = true;
			}
			iHash = reloadMitarbeiterWithConnection(iConnection, iHash);
			if (localTransaction) {
				iConnection.rollback();
			}
			logger.info("Mitarbeiter geladen: " + iHash.size());
		} catch (Exception e) {
			logger.error(null, e);
		} finally {
			try {
				if (localTransaction) {
					releaseConnection(iConnection);
				}
			} catch (Exception e) {
				logger.error(null, e);
			}
		}
		return iHash;
	}

	public static void checkOrCreateTables(Connection iConnection) throws Exception
	{
		DDL_DATABASE_DIALECT dbms = DDLTools.DDL_identify_database(iConnection);
		if (logger.isDebugEnabled()) logger.debug("pruefe Datenbankschema...");
		for (I_TableSpec  iTable: Schema.getSchema()) {
			if (logger.isDebugEnabled()) logger.debug("pruefe " + iTable.getName() + "...");
			Statement s = iConnection.createStatement();
			try {
				s.execute("select count(*) from "+iTable.getName());
			} catch (SQLException e) {
				if (logger.isDebugEnabled()) logger.debug(iTable.getName() + " failed.");
				if (logger.isDebugEnabled()) logger.debug("erzeuge " + iTable.getName() + "...");
				iConnection.commit();
				s.close();
				ArrayList<String> STA = DDLTools.DDL_STMT_create_table(dbms, iTable);
				for (String iStmt : STA) {
					if (logger.isDebugEnabled()) logger.debug("STMT: [" + iStmt + "]");
					s = iConnection.createStatement();
					try {
						s.execute(iStmt);
					} catch (SQLException e2) {
						logger.fatal(null, e2);
						throw e2;
					}
					s.close();
					iConnection.commit();					
				}
			} finally {
				if (logger.isDebugEnabled()) logger.debug(iTable.getName() + " ok.");
				s.close();
				iConnection.commit();
			}
		}
	}
	
	public static void insertES_LOGS(Connection iConnection, DaoES_LOGS iEntry) throws SQLException
	{
		PreparedStatement stmt = iConnection.prepareStatement(
		    	"insert into "+clserv.model.tables.ES_LOGS.getInstance().getName()+
		    	" ("+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName(null)+","+
		    	clserv.model.tables.ES_LOGS.ColumnENTRY_TYPE.getName(null)+","+
		    	clserv.model.tables.ES_LOGS.ColumnINFO.getName(null)+","+
		    	clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName(null)+","+
		    	clserv.model.tables.ES_LOGS.ColumnORIGIN.getName(null)+","+
		    	clserv.model.tables.ES_LOGS.ColumnSOCKET_CLIENT.getName(null)+
		    	") values (?,?,?,?,?,?)");
		stmt.setString(1, iEntry.getLogin());
		stmt.setString(2, iEntry.getEntry_type());
		stmt.setString(3, iEntry.getInfo());
		stmt.setTimestamp(4, iEntry.getEntry_ts());
		stmt.setString(5, iEntry.getOrigin());
		stmt.setString(6, iEntry.getSocket_client());
		if (logger.isDebugEnabled()) logger.debug("Datenbank insert: "+iEntry.toString());
		stmt.execute();		
		stmt.close();
	}

	public static void insertES_PRELOGS(Connection iConnection, DaoES_PRELOGS iEntry) throws SQLException {
		PreparedStatement stmt = iConnection.prepareStatement(
		    	"insert into "+clserv.model.tables.ES_PRELOGS.getInstance().getName()+
		    	" ("+clserv.model.tables.ES_PRELOGS.ColumnLOGIN.getName(null)+","+
		    	clserv.model.tables.ES_PRELOGS.ColumnENTRY_TYPE.getName(null)+","+
		    	clserv.model.tables.ES_PRELOGS.ColumnINFO.getName(null)+","+
		    	clserv.model.tables.ES_PRELOGS.ColumnENTRY_TS.getName(null)+","+
		    	clserv.model.tables.ES_PRELOGS.ColumnSOCKET_CLIENT.getName(null)+
		    	") values (?,?,?,?,?)");
		stmt.setString(1, iEntry.getLogin());
		stmt.setString(2, iEntry.getEntry_type());
		stmt.setString(3, iEntry.getInfo());
		stmt.setTimestamp(4, iEntry.getEntry_ts());
		stmt.setString(5, iEntry.getSocket_client());
		if (logger.isDebugEnabled()) logger.debug("Datenbank insert: "+iEntry.toString());
		stmt.execute();		
		stmt.close();
	}
	
	public static void deleteES_PRELOGS(Connection iConnection, DaoES_PRELOGS iEntry) throws SQLException {
		PreparedStatement stmt = iConnection.prepareStatement(
		    	"delete from "+clserv.model.tables.ES_PRELOGS.getInstance().getName()+
		    	" where "+clserv.model.tables.ES_PRELOGS.ColumnLOGIN.getName(null)+"=?"+
				" and "+clserv.model.tables.ES_PRELOGS.ColumnENTRY_TS.getName(null)+"=?");
				
		stmt.setString(1, iEntry.getLogin());
		stmt.setTimestamp(2, iEntry.getEntry_ts());
		if (logger.isDebugEnabled()) logger.debug("Datenbank delete: "+iEntry.toString());
		stmt.execute();
		stmt.close();
	}

	public Connection getConnection() throws ClassNotFoundException, SQLException {
		return mConnPool.getConnection();
	}

	public void releaseConnection(Connection iConnection) throws SQLException {
		mConnPool.releaseConnection(iConnection);
	}

	public static void closeAllConnections() {
		mConnPool.closeAllConnections();
	}

	public void shutdown() throws SQLException {
		mConnPool.shutdown();
	}
	
	public static int getActualPoolsize_inUse() {
		return mConnPool.getPoolsize_inUse();
	}
	public static int getActualPoolsize_notInUse() {
		return mConnPool.getPoolsize_notInUse();
	}
	
	public static int getActualPoolsize_allTime() {
		return mConnPool.getPoolsize_allTime();
	}

	public static void reloadLogs(Connection iConnection, HashMap<String, Mitarbeiter> iHash) {
		boolean localTransaction = false;
		if (iHash == null) return;
		GregorianCalendar cal = new GregorianCalendar();
		cal.set(Calendar.HOUR_OF_DAY, 0);
		cal.set(Calendar.MINUTE, 0);
		cal.set(Calendar.SECOND, 0);
		cal.set(Calendar.MILLISECOND, 0);
		Timestamp ts_heute_tagesbegin = new Timestamp(cal.getTimeInMillis());
		cal.set(Calendar.HOUR_OF_DAY, 23);
		cal.set(Calendar.MINUTE, 59);
		cal.set(Calendar.SECOND, 59);
		cal.set(Calendar.MILLISECOND, 999);
		Timestamp ts_heute_tagesende = new Timestamp(cal.getTimeInMillis());
		cal.add(Calendar.MONTH, -1);	// bis max. einen Monat zurueckliegende alte Eintraege dazuladen
		Timestamp ts_unteres_limit = new Timestamp(cal.getTimeInMillis());
		try {
			if (iConnection == null) {
				iConnection = Database.getInstance().getConnection();
				localTransaction = true;
			}
			
			// laden der Logs ...
			
			String Stmt =
				"select " + DDLTools.DDL_HELPER_create_columnlist(ES_LOGS.getInstance(), "el1")+
				" from "+clserv.model.tables.ES_LOGS.getInstance().getName()+" el1"+
				" where "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("el1")+">=? and "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("el1")+"<=?"+
				" union all"+
				" select " + DDLTools.DDL_HELPER_create_columnlist(ES_LOGS.getInstance(), "el2")+
				" from "+clserv.model.tables.ES_LOGS.getInstance().getName()+" el2"+
				" join qmdb10 qm on qm.qmmannummr='01' and "+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName("el2")+"=qm.qmperkubez and qm.qmperaktiv='J'"+
				" where"+
				" "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("el2")+"<? and "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("el2")+">? and"+
				" not exists (select null from "+clserv.model.tables.ES_LOGS.getInstance().getName()+" x"+
					" where "+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName("x")+"="+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName("el2")+" and"+
					" "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("x")+">=? and "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("x")+"<=?)"+
				" and "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("el2")+"=(select max("+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("y")+")"+
					" from "+clserv.model.tables.ES_LOGS.getInstance().getName()+" y where "+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName("y")+"="+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName("el2")+" and"+
					" "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName("y")+"<?)"+
				" order by 4";
			
			PreparedStatement StmtPrep = iConnection.prepareStatement(Stmt);
			StmtPrep.setTimestamp(1, ts_heute_tagesbegin);
			StmtPrep.setTimestamp(2, ts_heute_tagesende);
			StmtPrep.setTimestamp(3, ts_heute_tagesbegin);
			StmtPrep.setTimestamp(4, ts_unteres_limit);
			StmtPrep.setTimestamp(5, ts_heute_tagesbegin);
			StmtPrep.setTimestamp(6, ts_heute_tagesende);
			StmtPrep.setTimestamp(7, ts_heute_tagesbegin);
			
			if (logger.isDebugEnabled()) {
				logger.debug("Select=[" + Stmt + "]");
				logger.debug("Parameter 1=[" + ts_heute_tagesbegin.toString() + "]");
				logger.debug("Parameter 2=[" + ts_heute_tagesende.toString() + "]");
				logger.debug("Parameter 3=[" + ts_heute_tagesbegin.toString() + "]");
				logger.debug("Parameter 4=[" + ts_unteres_limit.toString() + "]");
				logger.debug("Parameter 5=[" + ts_heute_tagesbegin.toString() + "]");
				logger.debug("Parameter 6=[" + ts_heute_tagesende.toString() + "]");
				logger.debug("Parameter 7=[" + ts_heute_tagesbegin.toString() + "]");
			}
			
			StmtPrep.execute();
			
			ResultSet rs = StmtPrep.getResultSet();
			int count = 0;
			Mitarbeiter ma;
			DaoES_LOGS aLog = null;
			String aLogin;
			AppContext ac = AppContext.getInstance();
			for (; rs.next() ;count++) {
				ma = null;
				aLogin = rs.getString(clserv.model.tables.ES_LOGS.ColumnLOGIN.getName(null));
				if (iHash.containsKey(aLogin)) {
					ma = iHash.get(aLogin);
				}
				if (ma == null) {
					logger.warn("Unbekannter Login: [" + aLogin + "]! Mitarbeiter nicht gefunden. Erzeuge anonymen Mitarbeiter...");
					ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(aLogin);
					ac.einfuegenOderErsetzenMitarbeiter(ma);
				}
				if (ma != null) {
					aLog = new DaoES_LOGS(aLogin, rs.getTimestamp(clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName(null)));
					aLog.setEntry_type(rs.getString(clserv.model.tables.ES_LOGS.ColumnENTRY_TYPE.getName(null)));
					aLog.setInfo(rs.getString(clserv.model.tables.ES_LOGS.ColumnINFO.getName(null)));
					if (logger.isTraceEnabled()) {
						logger.trace("Log Eintrag=[" + aLog.toString() + "]");						
					}
					ma.addLog(aLog);
				} else {
					logger.error("Unbekannter Login: [" + aLogin + "]! Mitarbeiter wurde nicht gefunden und konnte auch nicht anonym angelegt werden!");
				}
			}
			if (aLog != null) {
				ac.notifyAboutLogEntry(aLog.getEntry_ts().getTime());
			} else {
				ac.notifyAboutLogEntry(0);				
			}
			if (logger.isDebugEnabled()) {
				logger.debug("Anzahl geladener Log Eintraege=[" + count + "]");
			}
			rs.close();
			StmtPrep.close();
			
			// laden der Logs fertig.

			// laden der PreLogs...
			
			Stmt = "select "+DDLTools.DDL_HELPER_create_columnlist(ES_PRELOGS.getInstance(), null)+
					" from "+ES_PRELOGS.getInstance().getName()+
					" order by "+clserv.model.tables.ES_PRELOGS.ColumnENTRY_TS.getName(null);
			
			StmtPrep = iConnection.prepareStatement(Stmt);
			StmtPrep.execute();
			rs = StmtPrep.getResultSet();
			count = 0;
			DaoES_PRELOGS aPreLog = null;
			for (; rs.next() ;count++) {
				aLogin = rs.getString(clserv.model.tables.ES_PRELOGS.ColumnLOGIN.getName(null));
				ma = iHash.get(aLogin);
				if (ma == null) {
					logger.warn("Unbekannter Login: [" + aLogin + "]! Mitarbeiter nicht gefunden. Erzeuge anonymen Mitarbeiter...");
					ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(aLogin);
					ac.einfuegenOderErsetzenMitarbeiter(ma);
				}
				if (ma != null) {
					aPreLog = new DaoES_PRELOGS(aLogin, rs.getTimestamp(clserv.model.tables.ES_PRELOGS.ColumnENTRY_TS.getName(null)));
					aPreLog.setEntry_type(rs.getString(clserv.model.tables.ES_PRELOGS.ColumnENTRY_TYPE.getName(null)));
					aPreLog.setInfo(rs.getString(clserv.model.tables.ES_PRELOGS.ColumnINFO.getName(null)));
					if (logger.isTraceEnabled()) {
						logger.trace("PreLog Eintrag=[" + aPreLog.toString() + "]");						
					}
					ma.addPreLog(aPreLog);
				} else {
					logger.error("Unbekannter Login: [" + aLogin + "]! Mitarbeiter wurde nicht gefunden und konnte auch nicht anonym angelegt werden!");
				}
			}
			if (logger.isDebugEnabled()) {
				logger.debug("Anzahl geladener PreLog Eintraege=[" + count + "]");
			}
			rs.close();
			StmtPrep.close();

			// PreLogs fertig.
			
			if (localTransaction) {
				iConnection.rollback();
			}
		} catch (Exception e) {
			logger.error(null, e);
		} finally {
			try {
				if (localTransaction) {
					Database.getInstance().releaseConnection(iConnection);
				}			
			} catch (Exception e) {
				logger.error(null, e);
			}
		}
	}
}
