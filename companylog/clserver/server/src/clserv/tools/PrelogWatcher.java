/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package clserv.tools;
import java.sql.Connection;
import java.sql.Timestamp;
import java.util.Iterator;
import java.util.Properties;
import org.apache.log4j.Logger;
import clserv.clienthandler.AClientHandler_es_logger;
import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.model.dao.DaoES_PRELOGS;
import clserv.model.tables.Columns;
import clserv.shared.Helper;

/**
 *
 * @author kuk
 */
public class PrelogWatcher extends Thread {
	private static final int DEFAULT_CLIENTCHECK_TIMEOUT = 0;
	private static final int DEFAULT_PRELOG_TIMEOUT = 0;
	private final Logger logger = Logger.getLogger(this.getClass());
	private int mTimeout = 0;
	private int mTimeoutClientCheck = 0;
	private int mTimeoutClientCheckRemaining = 0;
	private int mTimeoutPrelog = 0;
	private int mTimeoutPrelogRemaining = 0;
	
	public PrelogWatcher(int timeoutPrelogWatcher, Properties iProperties) {
		mTimeout = timeoutPrelogWatcher;
		mTimeoutClientCheckRemaining = mTimeoutClientCheck = Integer.parseInt(iProperties.getProperty("TimeoutClientCheckWatcher", Integer.toString(DEFAULT_CLIENTCHECK_TIMEOUT)));
		mTimeoutPrelogRemaining = mTimeoutPrelog = Integer.parseInt(iProperties.getProperty("TimeoutPrelogWatcher", Integer.toString(DEFAULT_PRELOG_TIMEOUT)));
	}

	@Override
	public void run() {
		logger.debug("Neuer Thread (Watcher,id="+getId()+", timeout="+mTimeout+") gestartet...");
		if (mTimeout <= 0) {
			logger.debug("Kein Timeout! Watcher wird beendet.");
			return;
		}
		try {
			do {
				sleep(mTimeout * 1000);
				if (logger.isTraceEnabled()) logger.trace("Watcher,id="+getId()+" awaken...");

				AppContext ac = AppContext.getInstance();
				Timestamp now = Helper.getCurrentTime();
				
				if (mTimeoutClientCheck > 0) {
					mTimeoutClientCheckRemaining -= mTimeout;
					if (mTimeoutClientCheckRemaining <= 0) {
						mTimeoutClientCheckRemaining = mTimeoutClientCheck;
						if (logger.isTraceEnabled()) logger.trace("Watcher: processing clientcheck...");
						ac.checkAllClientEntries(null, null, null);
					}
				}
				
				if (mTimeoutPrelog > 0) {
					mTimeoutPrelogRemaining -= mTimeout;
					if (mTimeoutPrelogRemaining <= 0) {
						mTimeoutPrelogRemaining = mTimeoutPrelog;
						if (logger.isTraceEnabled()) logger.trace("Watcher: processing prelog check...");
						synchronized (ac) {
							Iterator<Mitarbeiter> iter = ac.getMitarbeiterIterator();
							while (iter != null && iter.hasNext()) {
								Mitarbeiter aMitarbeiter = iter.next();
								for (int idx = 0; ; ) {
									DaoES_PRELOGS aPrelog = aMitarbeiter.getPreLog(idx);
									if (aPrelog != null) {
										if (aPrelog.getEntry_ts().before(now)) {
											// Normalen Eintrag erzeugen
											DaoES_LOGS entry = new DaoES_LOGS(aPrelog.getLogin(), aPrelog.getEntry_ts());
											entry.setEntry_type(aPrelog.getEntry_type());
											entry.setInfo(aPrelog.getInfo());
											entry.setSocket_client(aPrelog.getSocket_client());
											entry.setOrigin(Columns.StrConstrORIGIN_Prelog);
											// Eintrag anlegen, aber nur, wenn > letztem "echten" Log Eintrag
											DaoES_LOGS lastLog = aMitarbeiter.getLog(-1);
											if (lastLog == null || lastLog.getEntry_ts().before(entry.getEntry_ts())) {
												AClientHandler_es_logger.handleNewEntry(ac, entry, logger, true);
											}
											// prelog aus prelog liste entfernen...
											aMitarbeiter.removePreLog(idx);
											// prelog aus Datenbank entfernen
											Connection c = null;
											try {
												c = ac.getDatabase().getConnection();
												clserv.shared.Database.deleteES_PRELOGS(c, aPrelog);
												c.commit();
											} catch (Exception ex) {
												if (c != null) {
													try {
														c.rollback();
													} catch (Exception e1) {
														// just catch it ...
													}
												}
												// Moeglicherweise ist die Datenbankverbindung abgerissen - sicherheitshalber
												// werden alle offenen Connections geschlossen...
												logger.error("PreLog Eintrag konnte nicht gelöscht werden!", ex);
												clserv.shared.Database.closeAllConnections();
											}
										} else {
											idx++;	// naechstes prelog checken...
										}
									} else break;
								}
							}
						}
					}
				}
				if (logger.isTraceEnabled()) logger.trace("Watcher,id="+getId()+" going to sleep...");
			} while (true);
		} catch (InterruptedException ex) {
			logger.info("Watcher interrupted!");
		}
		logger.debug("Thread (Watcher) wird beendet...");
	}
	
}
