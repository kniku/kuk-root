package clserv.clienthandler;

import java.io.IOException;
import java.sql.Timestamp;
import java.util.Iterator;

import org.apache.log4j.Logger;

import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.tools.AppContext;


public abstract class AClientHandler_pze_base extends AClientHandler implements IStatemachine {
	private final Logger logger = Logger.getLogger(this.getClass());

	protected class Parameter {
		Timestamp mLastRequest = null; 
	}

	public void handle(String iCommand) throws IOException {
		
		Parameter myParams = (Parameter )getParameter(iCommand);
		
		if (handle_START(iCommand)) {
			DaoES_LOGS Log = null;
			AppContext ac = AppContext.getInstance();
			int count = 0;
			boolean firstOfMitarbeiter;
			synchronized (ac) {
				if (myParams == null || myParams.mLastRequest == null) {		
					Iterator<Mitarbeiter> maIter = ac.getMitarbeiterIterator();
					while (maIter != null && maIter.hasNext()) {
						Mitarbeiter ma = maIter.next();
						Iterator<DaoES_LOGS> esIter = ma.getLogIterator();
						firstOfMitarbeiter = true;
						while (esIter != null && esIter.hasNext()) {
							Log = esIter.next();
//							if (logger.isTraceEnabled()) {
//								logger.trace("Eintrag=[" + Log.toString() + "]");						
//							}
							count++;
							if (firstOfMitarbeiter) {
								if (handle_ENTRYSTART(iCommand, Log) == false) {
									break;
								}
								firstOfMitarbeiter = false;
							}
							if (handle_ENTRY(iCommand, Log) == false) {
								break;
							}								
						}
						if (firstOfMitarbeiter == false) {
							handle_ENTRYFINISH(iCommand, Log);
						}
					}
				} else {
					Iterator<Mitarbeiter> maIter = ac.getMitarbeiterIterator();
					while (maIter != null && maIter.hasNext()) {
						Mitarbeiter ma = maIter.next();
						
						int lastLog = ma.getLogCount() - 1;
						if (lastLog < 0) continue;
						
						int index = lastLog;
//						if (logger.isTraceEnabled()) {
//							logger.trace("Log Optimizer: Suche in Logliste ("+lastLog+") f�r ["+ma.getLogin()+"] nach Eintr�gen vor ["+DateFormat.getDateTimeInstance().format(myParams.mLastRequest)+"]");
//						}
						
						firstOfMitarbeiter = true;
						
						while (index >= 0) {
							Log = ma.getLog(index);
//							if (logger.isTraceEnabled()) {
//								logger.trace("Log Optimizer: Pr�fe Eintrag "+index+": ["+Log.toString()+"]");
//							}
							if (Log.getEntry_ts().before(myParams.mLastRequest)) {
								index++;
//								if (logger.isTraceEnabled()) {
//									logger.trace("Log Optimizer: Ab hier sind die Eintr�ge veraltet - werden �bersprungen");
//								}
								break;
							}
//							if (logger.isTraceEnabled()) {
//								logger.trace("Log Optimizer: Eintrag "+index+" ist aktueller als RequestTime");
//							}
							index--;
						}
						if (index < 0) index = 0;
//						if (logger.isTraceEnabled()) {
//							logger.trace("Log Optimizer: Einträge die gesendet werden müssen: ["+((index < lastLog) ? lastLog - index + 1: 0)+"]");
//						}
						while (index <= lastLog) {
							Log = ma.getLog(index);
//							if (logger.isTraceEnabled()) {
//								logger.trace("Sende Eintrag [" + Log.toString() + "]");						
//							}
							count++;
							if (firstOfMitarbeiter) {
								if (handle_ENTRYSTART(iCommand, Log) == false) {
									break;
								}
								firstOfMitarbeiter = false;
							}
							if (handle_ENTRY(iCommand, Log) == false) {
								break;
							}
							index++;
						}

						if (firstOfMitarbeiter == false) {
							handle_ENTRYFINISH(iCommand, Log);
						}
						
//						if (logger.isTraceEnabled()) {
//							logger.trace("Log Optimizer: Beendet ["+ma.getLogin()+"]");
//						}
					}
				}
			} // --synchronized
			handle_FINISH(iCommand);
			logger.info("PZE Request fertig. Neue Einträge übertragen: "+count);
		}
	}
}
