package clserv.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.net.URL;
import java.sql.Connection;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Properties;

import org.apache.log4j.Logger;

import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.model.dao.DaoES_PRELOGS;
import clserv.shared.Database;
import clserv.shared.GlobalConstants;
import clserv.shared.Helper;
import clserv.shared.I_Employee;
import clserv.shared.I_EmployeeManager;


/**
 * Der Applikationskontext
 * SINGLETON
 * @author kuk
 *
 */
public final class AppContext implements I_EmployeeManager {
	private java.util.Properties mProperties = null;
	private static Logger logger = Logger.getLogger(AppContext.class);
	private long cntConnections = 0;
	private int cntEmergencyLogs = 0;
	private HashMap<clserv.shared.Constants.RequestType, Integer> RequestStatistik = new HashMap<clserv.shared.Constants.RequestType, Integer>();
	private int maxThreadsAllTime = 0;
	private long appstart = 0;
	private HashMap<String, Mitarbeiter> logins = new HashMap<String, Mitarbeiter>();
	private clserv.shared.Database mDatabase = null;
	private long mLastLogEntry = 0;	// timestamp letzter IN/OUT Eintrag
	
	private String mServerName = null;
	private int mPort = 0;
	private int mPortDeprecated = 0;
	private int mTimeoutClient = 10;	// timout fuer blocked i/o in Sekunden
	private EmergencyLogger emergency_es_logger = EmergencyLogger.getInstance();
	private int mDBMaxPoolSize = 0;
	private static AppContext mTheAppContext = null;
	private int mTimeoutWatcher = 0;	// timeout fuer prelog watching (0=no watcher)
	private PrelogWatcher mWatcher = null;
	private UserClientIndex mUserClientIndex = null;
	private int mServerMode = 0;
	private DBListener mDBListener = null;
	private String mDataDir = null;
	
	public void emergency_handle(DaoES_LOGS iEntry, String iMessage) {
		synchronized (this) {
			emergency_es_logger.handle(iEntry, iMessage);
		}
		cntEmergencyLogs++;
	}
	
	public void notifyAboutEmergencyImport(int iImportsOk) {
		cntEmergencyLogs -= iImportsOk;
		if (cntEmergencyLogs < 0) cntEmergencyLogs = 0;
	}
	
	/**
	 * Erh�ht den entsprechenden Request Counter um 1
	 * @param iRequest Der zu erh�hende Request
	 * @see AppContext.RequestType f�r m�glich Request Types...
	 */
	public void notifyAboutRequest(clserv.shared.Constants.RequestType iType) {
		if (RequestStatistik.containsKey(iType) == false) {
			RequestStatistik.put(iType, 1);
		} else {
			Integer i = RequestStatistik.get(iType);
			i++;
			RequestStatistik.put(iType, i);
		}
		if (iType != clserv.shared.Constants.RequestType.ALL_OF_THEM) {
			notifyAboutRequest(clserv.shared.Constants.RequestType.ALL_OF_THEM);
		}
	}
	
	/**
	 * Liefert den Request Count fuer angegebenen Request Type
	 * @param iType Der gewuenschte Request Type
	 * @return Anzahl der Requests dieses Types
	 */
	public Integer getRequestCount(clserv.shared.Constants.RequestType iType) {
		Integer r = 0;
		if (RequestStatistik.containsKey(iType)) {
			r = RequestStatistik.get(iType);
		}
		return r;
	}
	
	/**
	 * Aktualisiert den Zeitstempel des letzten IN/OUT Requests
	 * @param iEntryTime Zeitstempel des neuen Eintrags oder 0 (=>aktuelle Zeit)
	 * @see getLastLogEntry
	 */
	public void notifyAboutLogEntry(long iEntryTime) {
		if (iEntryTime > 0) {
			mLastLogEntry = iEntryTime;
		} else {
			mLastLogEntry = Helper.getCurrentTimeMillis();
		}
	}
	
	/**
	 * @return Liefert Zeitstempel des letzten IN/OUT Requests
	 * @see notifyAboutLogEntry
	 */
	public synchronized long getLastLogEntry() {
		return mLastLogEntry;
	}
	
	public static AppContext getInstance() {
		if (mTheAppContext == null) {
			mTheAppContext = new AppContext();
		}
		return mTheAppContext;
	}
	
	public static FileInputStream getPropertyFile() throws FileNotFoundException {
		FileInputStream r = null;
		logger.debug("Suche property file...");
//		URL u = ClassLoader.getSystemClassLoader().getResource(GlobalConstants.CLServ_PROPERTY_FILE);
		URL u = logger.getClass().getClassLoader().getResource("clserv.properties");
		if (u != null) {
			logger.info(u.getPath());
			r = new FileInputStream(u.getPath());
		}
/*		try {
			logger.debug(GlobalConstants.CLServ_PROPERTY_FILE+"...");
			r = new FileInputStream(GlobalConstants.CLServ_PROPERTY_FILE);
		} catch (FileNotFoundException e) {
			logger.debug("bin/"+GlobalConstants.CLServ_PROPERTY_FILE+"...");
			try {
				r = new FileInputStream("bin/"+GlobalConstants.CLServ_PROPERTY_FILE);
			} catch (FileNotFoundException e2) {
				logger.debug("server/bin/"+GlobalConstants.CLServ_PROPERTY_FILE+"...");
				r = new FileInputStream("server/bin/"+GlobalConstants.CLServ_PROPERTY_FILE);
			}
		}
		logger.debug("Gefunden.");		
*/		return r;
	}
	
	private AppContext() {
		appstart = Helper.getCurrentTimeMillis();
		mProperties = new java.util.Properties();
		try {
			FileInputStream f = getPropertyFile();
			mProperties.load(f);
			f.close();

			mServerName = mProperties.getProperty("ServerName", GlobalConstants.CLServ_ServerName);
			mPort = Integer.parseInt(mProperties.getProperty("Port", GlobalConstants.CLServ_Port));
			mPortDeprecated = Integer.parseInt(mProperties.getProperty("PortDeprecated", GlobalConstants.CLServ_Port_deprecated));
			mDBMaxPoolSize  = Integer.parseInt(mProperties.getProperty("DBMaxPoolSize", GlobalConstants.CLServ_DBMaxPoolSize));
			mTimeoutClient = Integer.parseInt(mProperties.getProperty("TimeoutClient", GlobalConstants.CLServ_TimeoutClient));
			mTimeoutWatcher = Integer.parseInt(mProperties.getProperty("TimeoutWatcher", GlobalConstants.CLServ_TimeoutWatcher));
			mServerMode = Integer.parseInt(mProperties.getProperty("ServerMode", "0"));
			mDataDir = mProperties.getProperty("DataDir", "data");
			logger.info("========== clserv Startup ==========");
			logger.info("Server Name="+mServerName);
			logger.info("Version="+clserv.server.getVersionString());
			logger.info("Port="+mPort);
			logger.info("PortDeprecated="+mPortDeprecated);
			logger.info("DataDir="+mDataDir);
			logger.info("Database=("+
					mProperties.getProperty("DB_Driver", GlobalConstants.QMSDB_Default_Driver)+
					") "+
					mProperties.getProperty("DB_User", GlobalConstants.QMSDB_Default_User)+
					"@"+
					mProperties.getProperty("DB_Connection", GlobalConstants.QMSDB_Default_Connect));
			logger.info("TimeoutClient="+mTimeoutClient);
			logger.info("TimeoutWatcher="+mTimeoutWatcher);
			logger.info("ServerMode="+mServerMode);
			logger.info("======================================");
		} catch (Exception e) {
			logger.fatal("property file nicht gefunden oder nicht lesbar: "+GlobalConstants.CLServ_PROPERTY_FILE, e);
			System.exit(-1);
		}
		String DBConn = mProperties.getProperty("DB_Connection", GlobalConstants.QMSDB_Default_Connect);
		DBConn = DBConn.replace("${DataDir}", mDataDir);
		mDatabase = clserv.shared.Database.createInstance(mProperties.getProperty("DB_Driver", GlobalConstants.QMSDB_Default_Driver),
				DBConn,
				mProperties.getProperty("DB_User", GlobalConstants.QMSDB_Default_User),
				mProperties.getProperty("DB_Password", GlobalConstants.QMSDB_Default_Pass),
				mDBMaxPoolSize);
		try {
			logger.info("teste Datenbankverbindung...");
			Connection test = mDatabase.getConnection();
			Database.checkOrCreateTables(test);
			mDatabase.releaseConnection(test);
			logger.info("Datenbankverbindung Ok.");
		} catch (Exception e) {
			logger.fatal("Datenbankverbindung fehlgeschlagen: "+GlobalConstants.CLServ_PROPERTY_FILE, e);
			System.exit(-1);
		}
		
		// erzeuge Verzeichnisstruktur
		File f = new File(mDataDir);
		logger.info("DataDirFull="+f.getAbsolutePath());
		if (f.exists() == false) {
			f.mkdirs();
		}
		f = new File(mDataDir+"/clients");
		if (f.exists() == false) {
			f.mkdir();
		}
		
		startePrelogWatcher(mProperties);
		starteDBListener();
		mUserClientIndex = new UserClientIndex(mDataDir+"/clients");
	}

	/**
	 * 'Herunterfahren' des AppContexts (nur bei Prozessende sinnvoll)
	 * @param iReturn Return - Wert für den exit Aufruf...
	 */
	public void shutdown() {
		stopPrelogWatcher();
		stopDBListener();
		try {
			mDatabase.shutdown();
		} catch (Exception e) {
			logger.error(null, e);
		}
	}
	
	/**
	 * Fuer interne Statistik: Erhoeht Anzahl der Client Verbindungen
	 * @see getCntConnections
	 */
	public void addConnection()  {
		cntConnections += 1;
	}
	
	/**
	 * Fuer interne Statistik: Erhoeht die Anzahl der max. Threads, wenn
	 * iNumberOfThreads > dem aktuellem Maximimum
	 * @param iNumberOfThreads Anzahl der aktuellen Threads
	 * @see getMaxThreadsAllTime
	 */
	public void informAboutRunningThreads(int iNumberOfThreads) {
		if (iNumberOfThreads > maxThreadsAllTime) {
			maxThreadsAllTime = iNumberOfThreads;
		}
	}

	/**
	 * Traegt ein Mitarbeiterobjekt in die globale Mitarbeiterliste ein - oder
	 * modifiert ein bestehendens
	 * @param iMitarbeiter Der einzufuegende bzw. zu modifizierende Mitarbeiter
	 * @see findeMitarbeiter(String iLogin)
	 */
	public void einfuegenOderErsetzenMitarbeiter(Mitarbeiter iMitarbeiter) {
		synchronized (this) {
			logins.remove(iMitarbeiter.getLogin());
			logins.put(iMitarbeiter.getLogin(), iMitarbeiter);
		}
	}
	
	/**
	 * Verwirft und aktualisiert anschlieszend die globale Mitarbeiterliste aus der Datenbank neu
	 */
	public void reset_mitarbeiter() {
		try {
			synchronized (this) {
				logins = mDatabase.reloadMitarbeiter(null, logins);
			}
		} catch (Exception e) {
			logger.error(null, e);
		}
	}
	public void reset_all() {
		mLastLogEntry = 0;
		try {
			synchronized (this) {
				reset_mitarbeiter();
				clserv.shared.Database.reloadLogs(null, logins);
				PluginManager.getInstance().reloadPlugins();
			}
		} catch (Exception e) {
			logger.error(null, e);
		}
	}
	
	/**
	 * Fügt einen neuen Log-Eintrag für den angegebenen Mitarbeiter in der globalen Mitarbeiterliste ein
	 * @param iLogin Der gesuchte Mitarbeiter (Login)
	 * @param iEntry Der neue Log-Eintrag
	 * @return Eintrag eingefuegt? (true/false)
	 */
	public boolean addLogFuerMitarbeiter(String iLogin, DaoES_LOGS iEntry) {
		Mitarbeiter mitarbeiter = null;
		boolean r = false;
		synchronized (this) {
			mitarbeiter = (Mitarbeiter )findeMitarbeiterMitLogin(iLogin);
			if (mitarbeiter != null) {
				if (mitarbeiter.getLogCount() > 0) {
					DaoES_LOGS es = mitarbeiter.getLog(-1);	// letzten Eintrag holen
					Calendar cal_entry = Calendar.getInstance();
					Calendar cal_es = Calendar.getInstance();
					cal_entry.setTime(iEntry.getEntry_ts());
					cal_es.setTime(es.getEntry_ts());
					if (cal_entry.get(Calendar.DAY_OF_MONTH) != cal_es.get(Calendar.DAY_OF_MONTH)) {
						logger.debug("Logliste fuer Mitarbeiter [" + iLogin + "] wird zurueckgesetzt (erster Eintrag fuer diesen Tag)");
						mitarbeiter.resetLogs();
					} else if (	cal_entry.get(Calendar.HOUR_OF_DAY) == cal_es.get(Calendar.HOUR_OF_DAY) &&
								cal_entry.get(Calendar.MINUTE) == cal_es.get(Calendar.MINUTE) &&
								cal_entry.get(Calendar.SECOND) == cal_es.get(Calendar.SECOND)) {
						logger.debug("Eintrag mit gleichem Zeitstempel bereits vorhanden! Ignoriere neuen Eintrag.");
						return r;
					}
				}
				mitarbeiter.addLog(iEntry);
				r = true;
			}
		}
		return r;
	}

	/**
	 * F�gt einen neuen Log-Eintrag f�r den angegebenen Mitarbeiter in der globalen Mitarbeiterliste ein
	 * @param iLogin Der gesuchte Mitarbeiter (Login)
	 * @param iEntry Der neue Log-Eintrag
	 * @return Eintrag eingefuegt? (true/false)
	 */
	public boolean addPreLogFuerMitarbeiter(String iLogin, DaoES_PRELOGS iEntry) {
		Mitarbeiter mitarbeiter = null;
		boolean r = false;
		synchronized (this) {
			mitarbeiter = (Mitarbeiter )findeMitarbeiterMitLogin(iLogin);
			if (mitarbeiter != null) {
				r = true;
				mitarbeiter.addPreLog(iEntry);
			}
		}
		return r;
	}

	/*
	 * Getter
	 * ==============================================================
	 */
		
	public long getCntConnections() {
		return cntConnections;
	}
	
	public long getCntEmergencyLogs() {
		return cntEmergencyLogs;
	}

	public int getMaxThreadsAllTime() {
		return maxThreadsAllTime;
	}

	public long getUptime() {
		return Helper.getCurrentTimeMillis() - appstart;
	}

	public int getMPort() {
		return mPort;
	}

	public int getMPortDeprecated() {
		return mPortDeprecated;
	}

	public String getMServerName() {
		return mServerName;
	}
	
	public int getTimeoutClient() {
		return mTimeoutClient;
	}
	public int getTimeoutWatcher() {
		return mTimeoutWatcher;
	}
	public int getServerMode() {
		return mServerMode;
	}
	public String getDataDir() {
		return mDataDir;
	}
	/**
	 * Liefert einen Iterator fuer die globale Mitarbeiterliste
	 * Achtung: Nur innerhalb eines 'synchronized(AppContext)' - Blocks verwenden!
	 * @return Iterator der globalen Mitarbeiterliste
	 */
	public Iterator<Mitarbeiter> getMitarbeiterIterator() {
		return logins == null ? null : logins.values().iterator();
	}

	public int getDBMaxPoolSize() {
		return mDBMaxPoolSize;
	}
	
	public clserv.shared.Database getDatabase() {
		return mDatabase;
	}

	public void cleanup_old_logs(String iLogin) {
		synchronized (this) {
			if (iLogin == null) {
				Iterator<Mitarbeiter> maIter = getMitarbeiterIterator();
				while (maIter != null && maIter.hasNext()) {
					maIter.next().cleanup_old_logs();
				}
			} else {
				Mitarbeiter ma = (Mitarbeiter )findeMitarbeiterMitLogin(iLogin);
				if (ma != null) {
					ma.cleanup_old_logs();
				}
			}
		}
	}

	// ==========================================
	// Implementierung von IMitarbeiterVerwaltung
	// ==========================================
	
	@Override
	public I_Employee findeMitarbeiterMitLogin(String iLogin) {
		Mitarbeiter r = null;
		synchronized (this) {
			r = (Mitarbeiter )logins.get(iLogin);
		}
		if (r != null) {
			if (logger.isDebugEnabled()) logger.debug("Mitarbeitersuche: ["+iLogin+"] -> "+r.toString());
		} else {
			logger.warn("Mitarbeitersuche: ["+iLogin+"] -> nicht gefunden!");
		}
		return r;
	}

	@Override
	public I_Employee findeMitarbeiterMitNamen(String iVorname, String iNachname) {
		Mitarbeiter ma, r = null;
		synchronized (this) {
			Iterator<Mitarbeiter> iter = getMitarbeiterIterator();
			while (iter.hasNext()) {
				ma = iter.next();
				if (ma.getName().equals(iNachname) && ma.getFirstName().equals(iVorname)) {
					r = ma;
					break;
				}
			}
		}
		if (r != null) {
			if (logger.isDebugEnabled()) logger.debug("Mitarbeitersuche: ["+iVorname+","+iNachname+"] -> "+r.toString());
		} else {
			logger.warn("Mitarbeitersuche: ["+iVorname+","+iNachname+"] -> nicht gefunden!");			
		}
		return r;
	}

	private void startePrelogWatcher(Properties iProperties) {
		if (mWatcher == null) {
			mWatcher = new PrelogWatcher(mTimeoutWatcher, iProperties);
			mWatcher.start();
		}
	}

	private void stopPrelogWatcher() {
		if (mWatcher != null) {
			mWatcher.interrupt();
		}
	}

	private void starteDBListener() {
		if (getServerMode() > 0 && mDBListener == null) {
			mDBListener = new DBListener();
			mDBListener.start();
		}
	}
	
	private void stopDBListener() {
	if (mDBListener != null) {
		mDBListener.interrupt();
	}
}
	
	/* === Start: User Client register fuer notifier === */
	public void registerUserClientEntry(String iLogin, String iHost, String iPort) {
		synchronized (mUserClientIndex) {
			mUserClientIndex.insertUserClient(iLogin, iHost, iPort, true);			
		}
	}
	public void removeUserClientEntry(String iLogin, String iHost, String iPort) {
		synchronized (mUserClientIndex) {
			mUserClientIndex.removeUserClient(iLogin, iHost, iPort);
		}
	}
	public ArrayList<String> getClientUserList() {
		ArrayList<String> r = new ArrayList<String>();
		synchronized (mUserClientIndex) {
			Iterator<String> I = mUserClientIndex.getClientUserIter();
			if (I != null) {
				while (I.hasNext()) {
					r.add(I.next());
				}
			}
		}
		return r;
	}
	public ArrayList<String> getClientEntryList(String iLogin) {
		ArrayList<String> r = new ArrayList<String>();
		synchronized (mUserClientIndex) {
			Iterator<String> I = mUserClientIndex.getClientEntryIter(iLogin);
			if (I != null) {
				while (I.hasNext()) {
					r.add(I.next());
				}
			}
		}
		return r;
	}
	public int getCntUserClients() {
		synchronized (mUserClientIndex) {
			return mUserClientIndex.getClientEntryCount();
		}
	}
	public void checkAllClientEntries(String [] iUserFilter, ArrayList<String> oUsersOnline, ArrayList<String> oUsersOffline) {
		mUserClientIndex.checkAllClientEntries(iUserFilter, oUsersOnline, oUsersOffline);
	}
	/* === Ende: User Client register fuer notifier === */

}
