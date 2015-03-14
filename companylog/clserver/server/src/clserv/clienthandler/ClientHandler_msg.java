package clserv.clienthandler;

import java.io.File;
import java.net.Socket;
import java.sql.Connection;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Iterator;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.shared.Constants;
import clserv.shared.Helper;
import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_XMLHandler;
import clserv.shared.OldLogImporter;
import clserv.shared.OldLogImporterResult;
import clserv.shared.XMLParser;
import clserv.tools.AppContext;
import clserv.tools.EmergencyLogger;


public class ClientHandler_msg extends AClientHandler implements I_XMLHandler {
	private final Logger logger = Logger.getLogger(this.getClass());

	private String mRequestFrom = null;
	private String mRequestTo = null;
	private String mRequestMsg = null;
	
	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	void parseCommand(String iCommand) {
		try {
			XMLParser xmlparser = new XMLParser(this);
			xmlparser.parseString(iCommand);
		} catch (Exception e) {
			e.printStackTrace();
		}		
	}
	
	@Override
	public void startElement(String uri, String localName, String name, Attributes atts) throws SAXException {
		if (localName.equals(Constants.RequestTypeElements.MSG)) {
			mRequestFrom = atts.getValue(Constants.RequestValueAttrs.Login);
			if (mRequestFrom != null) mRequestFrom = mRequestFrom.toLowerCase();
			mRequestTo = atts.getValue(Constants.RequestValueAttrs.To);
			if (mRequestTo != null) mRequestTo = mRequestTo.toLowerCase();
			mRequestMsg = atts.getValue(Constants.RequestValueAttrs.Msg);
		}
	}

	@Override
	public void handle(String iCommand) throws Exception {
		int cnt_empfaenger_erreicht = 0;
		String nicht_erreicht = "";
		
		parseCommand(iCommand);
		if (logger.isDebugEnabled()) {
			logger.debug("msg: parsed parameters: l=["+mRequestFrom+"], to=["+mRequestTo+"], msg=["+mRequestMsg+"]");
		}
		
		if (mRequestFrom != null && mRequestTo != null && mRequestMsg != null) {
			AppContext ac = clserv.tools.AppContext.getInstance();
			String [] to_users;
			if (mRequestTo.equalsIgnoreCase("server")) {
				if (mRequestMsg.startsWith("clientcheck")) {
					String [] lLogins = null;
					if (mRequestMsg.indexOf(':') > 0) {
						lLogins = mRequestMsg.split(":");
					}
					ArrayList<String> lOn = new ArrayList<String>();
					ArrayList<String> lOff = new ArrayList<String>();
					ac.checkAllClientEntries(lLogins, lOn, lOff);
					for (String string : lOn) {
						sendServerMessageToUser(mRequestFrom, "ONLINE: "+string, false);
					}
					for (String string : lOff) {
						sendServerMessageToUser(mRequestFrom, "OFFLINE: "+string, false);
					}
					sendServerMessageToUser(mRequestFrom, "finished [clientcheck]", true);
				} else if (mRequestMsg.equals("status")) {
					String Message = ClientHandlerFactory.createResponse_Statusmeldung(ac).replaceAll("\\r", "");
					String [] yy = Message.split("\n");
					for (int i = yy.length - 2; i >= 0; i--) {
						sendServerMessageToUser(mRequestFrom, yy[i], false);	
					}
					sendServerMessageToUser(mRequestFrom, "finished [serverstatus]", true);
				} else if (mRequestMsg.equals("rehash")) {
					ac.reset_all();
					sendServerMessageToUser(mRequestFrom, "finished [rehash]", true);
				} else if (mRequestMsg.equals("elimport")) {
					if (ac.getDatabase().isAvailable() == false) {
						sendServerMessageToUser(mRequestFrom, "ERROR: no database connection!", true);
					} else {
						SimpleDateFormat sdf = new SimpleDateFormat("_yy-MM-dd,HH-mm-ss");
						File f = new File(EmergencyLogger.getEmergencyLogDirectoryName()+"/"+EmergencyLogger.getEmergencyLogFileName());
						File fnew = new File(EmergencyLogger.getEmergencyLogDirectoryName()+"/"+EmergencyLogger.getEmergencyLogFileName()+sdf.format(Helper.getCurrentTime()));
//						fnew.delete();
						OldLogImporterResult mImpResult = new OldLogImporterResult();
						if (f.exists() && f.canRead() && f.renameTo(fnew)) {
							Connection qmdb;
							qmdb = ac.getDatabase().getConnection();
							OldLogImporter.importiere(fnew.getAbsolutePath(), EmergencyLogger.getEmergencyLogDirectoryName()+"/man_import_errors.log", qmdb, true, ac, mImpResult);
							ac.getDatabase().releaseConnection(qmdb);
//							fnew.delete();
							if (mImpResult.imports_err > 0) {
								sendServerMessageToUser(mRequestFrom, "ERRORS during import: "+mImpResult.imports_err+"! see "+EmergencyLogger.getEmergencyLogDirectoryName()+"/man_import_errors.log", true);
							}
							sendServerMessageToUser(mRequestFrom, "imports: "+mImpResult.imports_ok, false);
							sendServerMessageToUser(mRequestFrom, "duplicates: "+mImpResult.imports_dbl, false);
							ac.notifyAboutEmergencyImport(1000000/*mImpResult.imports_ok*/);
						} else {
							sendServerMessageToUser(mRequestFrom, "file not found/readable: "+f.getAbsolutePath(), false);
						}
					}
					mConnection.sendMessageToClientImmediately("\r\ndone.\r\n\r\n>>");
					sendServerMessageToUser(mRequestFrom, "finished [elimport]", true);
				} else if (mRequestMsg.startsWith("cleanup")) {
						if (mRequestMsg.length() >= 11) {
							mRequestMsg = mRequestMsg.substring(8);
							if (ac.findeMitarbeiterMitLogin(mRequestMsg) == null) {
								sendServerMessageToUser(mRequestFrom, "ERROR: login unknown!", false);				
							}
						} else {
							mRequestMsg = null;
						}
						ac.cleanup_old_logs(mRequestMsg);
						sendServerMessageToUser(mRequestFrom, "finished [cleanup]", true);
				} else if (mRequestMsg.equals("help")) {
					sendServerMessageToUser(mRequestFrom, "cleanup [LOGIN]... remove old logs", false);				
					sendServerMessageToUser(mRequestFrom, "elimport ... emergency log import", false);
					sendServerMessageToUser(mRequestFrom, "rehash ... reload config., employees, logs ...", false);
					sendServerMessageToUser(mRequestFrom, "status ... get server status", false);				
					sendServerMessageToUser(mRequestFrom, "clientcheck[:LOGIN[:LOGIN]] ... client registration check", false);				
					sendServerMessageToUser(mRequestFrom, "valid commands are:", false);
					sendServerMessageToUser(mRequestFrom, "finished [help]", true);
				} else {
					sendServerMessageToUser(mRequestFrom, "unknown command ["+mRequestMsg+"]", true);					
				}
			} else {
				if (mRequestTo.equals("*")) {
					ArrayList<String> l = ac.getClientUserList();
					to_users = new String [l.size()];
					for (int i = 0; i < to_users.length; i++) {
						if (l.get(i).equals(mRequestFrom)) {
							to_users[i] = null;
						} else {
							to_users[i] = l.get(i);
						}
					}
				} else {
					to_users = mRequestTo.split(",");
				}
				for (int i = 0; i < to_users.length; i++) {
					if (to_users[i] != null) {
						String to_user = to_users[i].trim();
						if (sendMessageToUser(to_user, iCommand)) {
							cnt_empfaenger_erreicht += 1;						
						} else {
							if (nicht_erreicht.length() >0 ) nicht_erreicht += ",";
							nicht_erreicht += to_user;
						}
					}
				}
			}
			mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_ok(Integer.toString(cnt_empfaenger_erreicht)));
			if (nicht_erreicht.length() > 0) {
				sendServerMessageToUser(mRequestFrom, "Derzeit nicht erreichbar: ["+nicht_erreicht+"]", true);				
			}
		} else {
			mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_error("no from/to/msg"));			
		}
	}

	private boolean sendServerMessageToUser(String iTo, String iMsg, boolean iPopup) {
		String attrPopup;
		if (iPopup) {
			attrPopup = " popup=\"1\"";
		} else {
			attrPopup = " popup=\"0\"";			
		}
		return sendMessageToUser(iTo, "<msg l=\"Server\" to=\""+iTo+"\" m=\""+iMsg+"\""+attrPopup+"/>");				
	}
	
	private boolean sendMessageToUser(String iTo, String iMsg) {
		boolean r = false;
		ArrayList<String> lReceiver = clserv.tools.AppContext.getInstance().getClientEntryList(iTo);
		for (Iterator<String> iterator = lReceiver.iterator(); iterator.hasNext();) {
			String [] host_port = iterator.next().split(":");
			if (sendMessage(iTo, iMsg, host_port[0], host_port[1]) == false) {
				if (logger.isDebugEnabled()) {
					logger.debug("msg: invalid client ["+iTo+"->"+host_port[0]+":"+host_port[1]+"], canceling registration");
				}
				clserv.tools.AppContext.getInstance().removeUserClientEntry(iTo, host_port[0], host_port[1]);
			} else {
				r = true;
			}
		}
		
		return r;
	}
	
	private boolean sendMessage(String iTo, String iMsg, String iHost, String iPort) {
		boolean r = false;
		try {
			if (logger.isDebugEnabled()) {
				logger.debug("msg: trying client ["+iTo+"->"+iHost+":"+iPort+"]...");
			}
			Socket s = new Socket(iHost, Integer.parseInt(iPort));
			//s.setSoTimeout(10000);	// timeout von 10s
			if (s != null) {
				if (logger.isDebugEnabled()) {
					logger.debug("msg: sending ["+iMsg+"]...");
				}
	//			s.getOutputStream().write(iMsg.getBytes("ISO-8859-1"));							
				s.getOutputStream().write(iMsg.getBytes());							
				s.close();
				r = true;
			}
		} catch (Exception e) {
			// just catch it...
		}
		return r;
	}

}
