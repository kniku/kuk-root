package clserv.clienthandler;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Plugin;
import clserv.shared.I_XMLHandler;
import clserv.shared.XMLParser;
import clserv.tools.AppContext;
import clserv.tools.PluginManager;


class Parameter implements I_XMLHandler {
	private static Logger logger = Logger.getLogger(NewClientHandlerFactory.class);
	String mRequestType = null;
	@Override

	public void startElement(String uri, String localName, String name,
			Attributes atts) throws SAXException {
		if (localName.equals("esm")) {
			mRequestType = atts.getValue("t");
		}
	}
	
	void parseCommand(String iCommand) {
		try {
			XMLParser xmlparser = new XMLParser(this);
			xmlparser.parseString(iCommand);
		} catch (org.xml.sax.SAXParseException spe) {
			spe.printStackTrace();
			StringBuffer sb = new StringBuffer( spe.toString() );
			sb.append("\n Line number: " + spe.getLineNumber());
			sb.append("\nColumn number: " + spe.getColumnNumber() );
			sb.append("\n Public ID: " + spe.getPublicId() );
			sb.append("\n System ID: " + spe.getSystemId() + "\n");
			logger.info(sb.toString());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}

public final class NewClientHandlerFactory {
	private static Logger logger = Logger.getLogger(NewClientHandlerFactory.class);
	
	public static I_ClientHandler getHandler(I_ConnectionHandler iConnHandler, String iCommand) {
		I_ClientHandler r = null;
		
		if (iCommand != null) {
			AppContext ac = AppContext.getInstance();
			Parameter params = new Parameter();
			params.parseCommand(iCommand);
			
			if (params.mRequestType == null) return null;
			
			if (params.mRequestType.equals("info")) {
				r = new NewClientHandler_info();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.INFO);
				logger.info("handler=[info]");
			} else if (params.mRequestType.equals("help")) {
				r = new NewClientHandler_help();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.HELP);
				logger.info("handler=[help]");
			} else if (params.mRequestType.equals("rehash")) {
				r = new NewClientHandler_rehash();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.ALL_OF_THEM);
				logger.info("handler=[rehash]");
			} else if (params.mRequestType.equals("sql")) {
				r = new NewClientHandler_sql();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.SQL);
				logger.info("handler=[rehash]");
			} else {
				I_Plugin pi = PluginManager.getInstance().getInstanceOfPlugin(params.mRequestType);
				if (pi != null) {
					r = new NewClientHandler_plugins(pi);
					ac.notifyAboutRequest(clserv.shared.Constants.RequestType.ALL_OF_THEM);
					logger.info("handler=[plugin:"+params.mRequestType+",v="+pi.getVersion()+"]");
				}
			} 
		}
		return r;
	}
}
