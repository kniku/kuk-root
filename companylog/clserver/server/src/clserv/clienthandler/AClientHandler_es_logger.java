package clserv.clienthandler;

import java.sql.Connection;
import java.sql.Timestamp;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.model.dao.DaoES_PRELOGS;
import clserv.model.tables.Columns;
import clserv.shared.Constants;
import clserv.shared.Helper;
import clserv.shared.I_XMLHandler;
import clserv.shared.XMLParser;
import clserv.tools.AppContext;



public abstract class AClientHandler_es_logger extends AClientHandler implements I_XMLHandler {
	private final Logger logger = Logger.getLogger(this.getClass());

	private String mRequestLogin = null;
	private String mRequestInfo = null;
	private String mRequestPrelog = null;
	private Timestamp mRequestPrelogTS = null;
	
	void parseCommand(String iCommand) {
		try {
			XMLParser xmlparser = new XMLParser(this);
			xmlparser.parseString(iCommand);
			if (logger.isDebugEnabled()) {
				logger.debug("parsed values: "+
						"login=["+(mRequestLogin!=null?mRequestLogin:"")+"],"+
						"info=["+(mRequestInfo!=null?mRequestInfo:"")+"],"+
						"pre=["+(mRequestPrelog!=null?"true":"false")+"]");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}		
	}
	
	@Override
	public void startElement(String uri, String localName, String name, Attributes atts) throws SAXException {
		if (localName.equals(Constants.RequestTypeElements.IN) || 
			localName.equals(Constants.RequestTypeElements.OUT)) {
			mRequestLogin = atts.getValue(Constants.RequestValueAttrs.Login);
			if (mRequestLogin != null) mRequestLogin = mRequestLogin.toLowerCase();
			mRequestInfo = atts.getValue(Constants.RequestValueAttrs.Info);
			mRequestPrelog = atts.getValue(Constants.RequestValueAttrs.IsPrelog);
			mRequestPrelogTS = clserv.shared.ParserHelper.String2Timestamp(atts.getValue(Constants.RequestValueAttrs.Timestamp));
		}
	}

	void handle(String iCommand, String iEntryType) {
		// command: <in l="LOGIN" [i=INFO] [pre="1"] [t="20081005121500"]/>
		// command: <out l="LOGIN" [i=INFO] [pre="1"]/>
		boolean ok = false;
		parseCommand(iCommand);
		Timestamp ts = null;
		clserv.tools.AppContext ac = AppContext.getInstance();
		
		if (mRequestLogin != null) {
			Mitarbeiter ma = (Mitarbeiter )ac.findeMitarbeiterMitLogin(mRequestLogin);
			if (ma == null || Mitarbeiter.istAnonymerMitarbeiter(ma)) {
				// Guter Zeitpunkt, um die Mitarbeiterliste zu refreshen.
				// Vieleicht loest sich damit das Problem des unbekannten MA ja auch gleich...
				logger.warn("Mitarbeiter nicht gefunden -> Lade Mitarbeiterliste neu...");
//				ac.reset_mitarbeiter();
				ac.reset_all();
				ma = (Mitarbeiter )ac.findeMitarbeiterMitLogin(mRequestLogin);
			}
			if (ma == null) {
				// create one on the fly...
				ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(mRequestLogin);
				ac.einfuegenOderErsetzenMitarbeiter(ma);
				logger.warn("Anonymen Mitarbeiter erzeugt: "+ma.toString());
			}

			ts = Helper.getCurrentTime();
			
			if (mRequestPrelog == null) {
				if (mRequestPrelogTS != null) ts = mRequestPrelogTS;
				DaoES_LOGS entry = new DaoES_LOGS(ma.getLogin(), ts);
				entry.setEntry_type(iEntryType);
				if (mRequestInfo != null) {
					entry.setInfo(mRequestInfo);
				}
				entry.setOrigin(Columns.StrConstrORIGIN_Std);
				if (mConnection != null) entry.setSocket_client(mConnection.getSocket().getRemoteSocketAddress().toString());
				
				handleNewEntry(ac, entry, logger, (mRequestPrelogTS == null));
				ok = true;
				
			} else if (mRequestPrelogTS != null && ts.before(mRequestPrelogTS) /* keine prelogs in der Vergangenheit */) {
				Connection c = null;
				// create a prelog entry
				DaoES_PRELOGS preentry = new DaoES_PRELOGS(ma.getLogin(), ts);
				preentry.setEntry_type(iEntryType);
				preentry.setEntry_ts(mRequestPrelogTS);
				if (mRequestInfo == null) mRequestInfo = "";
				preentry.setInfo("[P]" + mRequestInfo);
				preentry.setSocket_client(mConnection.getSocket().getRemoteSocketAddress().toString());
				// Den neuen Eintrag gleich mal in die globale Logliste fuer diesen Mitarbeiter...
				if (ac.addPreLogFuerMitarbeiter(preentry.getLogin(), preentry)) {
					// ... und wenn das geklappt hat, den Eintrag in die Datenbank schreiben...
					try {
						c = ac.getDatabase().getConnection();
						clserv.shared.Database.insertES_PRELOGS(c, preentry);
						c.commit();
						if (logger.isTraceEnabled()) logger.trace("Eintrag ["+preentry.toString()+"] erfolgreich in Datenbank abgelegt");
						ac.addPreLogFuerMitarbeiter(preentry.getLogin(), preentry);
						ac.getDatabase().releaseConnection(c);
						ok = true;
					} catch (Exception e) {
						if (c != null) {
							try {
								c.rollback();
							} catch (Exception e1) {
								// just catch it ...
							}
						}
						// Moeglicherweise ist die Datenbankverbindung abgerissen - sicherheitshalber
						// weden alle offenen Connections geschlossen...
						logger.error("Eintrag wurde nicht in der Datenbank protokolliert!", e);
						clserv.shared.Database.closeAllConnections();
					}
				}
			}
		}
		if (mConnection != null) {
			if (ok) {
				mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_ok(null));
			} else {
				mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_error("invalid command!"));
			}
		}
	}

	public static void handleNewEntry(AppContext ac, DaoES_LOGS entry, Logger logger, boolean db_insert) {
		Connection c = null;
		// Den neuen Eintrag gleich mal in die globale Logliste fuer diesen Mitarbeiter...
		if (ac.addLogFuerMitarbeiter(entry.getLogin(), entry)) {
			// ... und wenn das geklappt hat, den Eintrag in die Datenbank oder - im Fehlerfall - ins
			// emergency log schreiben...
			ac.notifyAboutLogEntry(entry.getEntry_ts().getTime());
			if (db_insert == false) return;
			try {
				c = ac.getDatabase().getConnection();
				clserv.shared.Database.insertES_LOGS(c, entry);
				c.commit();
				if (logger.isTraceEnabled()) logger.trace("Eintrag ["+entry.toString()+"] erfolgreich in Datenbank abgelegt");
				ac.getDatabase().releaseConnection(c);
			} catch (Exception e) {
				String message = e.getMessage();
				if (c != null) {
					try {
						c.rollback();
					} catch (Exception e1) {
						// just catch it ...
					}
				}
				// Moeglicherweise ist die Datenbankverbindung abgerissen - sicherheitshalber
				// weden alle offenen Connections geschlossen...
				logger.error("Eintrag wurde nicht in der Datenbank protokolliert!", e);
				clserv.shared.Database.closeAllConnections();
				ac.emergency_handle(entry, message);
			}
		}
	}

}
