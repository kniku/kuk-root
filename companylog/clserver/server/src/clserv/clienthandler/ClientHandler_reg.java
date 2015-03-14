package clserv.clienthandler;

import org.apache.log4j.Logger;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clserv.shared.Constants;
import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_XMLHandler;
import clserv.shared.XMLParser;


public class ClientHandler_reg extends AClientHandler implements I_XMLHandler{
	private final Logger logger = Logger.getLogger(this.getClass());

	private String mRequestLogin = null;
	private String mRequestPort = null;
	
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
		if (localName.equals(Constants.RequestTypeElements.REG) ||
			localName.equals(Constants.RequestTypeElements.UNREG)) {
			mRequestLogin = atts.getValue(Constants.RequestValueAttrs.Login);
			if (mRequestLogin != null) mRequestLogin = mRequestLogin.toLowerCase();
			mRequestPort = atts.getValue(Constants.RequestValueAttrs.Port);
		}
	}
	
	@Override
	public void handle(String iCommand) throws Exception {
		boolean ok = false;
		
		parseCommand(iCommand);
		String iHost = mConnection.getSocket().getInetAddress().getHostAddress();
		if (logger.isTraceEnabled()) {
			logger.trace("reg: parsed parameters: l=["+mRequestLogin+"], port=["+mRequestPort+"]");
			logger.trace("reg: host=["+iHost+"]");
		}
		
		if (mRequestLogin != null && mRequestPort != null) {
			if (iCommand.startsWith("<" + Constants.RequestTypeElements.REG)) {
				clserv.tools.AppContext.getInstance().registerUserClientEntry(mRequestLogin, iHost, mRequestPort);
			} else {
				clserv.tools.AppContext.getInstance().removeUserClientEntry(mRequestLogin, iHost, mRequestPort);
			}
			ok = true;
		}
		if (ok) {
			mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_ok(null));
		} else {
			mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_error("no host or port"));
		}
	}

}
