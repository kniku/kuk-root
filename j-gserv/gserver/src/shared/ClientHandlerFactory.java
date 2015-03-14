package shared;

import java.net.Socket;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import clienthandler.ClientHandler_info;
import clienthandler.ClientHandler_telnet;


class Parameter extends SimpleXMLParser {
	private static final Log log = LogFactory.getLog(Parameter.class);
	String mRequestType = null;

	public void startElement(String uri, String localName, String name,
			Attributes atts) throws SAXException {
		if (localName.equals("gcm")) {
			mRequestType = atts.getValue("t");
		}
	}
	
	void parseCommand(String iCommand) {
		try {
			parseString(iCommand);
		} catch (org.xml.sax.SAXParseException spe) {
			spe.printStackTrace();
			StringBuffer sb = new StringBuffer( spe.toString() );
			sb.append("\n Line number: " + spe.getLineNumber());
			sb.append("\nColumn number: " + spe.getColumnNumber() );
			sb.append("\n Public ID: " + spe.getPublicId() );
			sb.append("\n System ID: " + spe.getSystemId() + "\n");
			log.error(sb.toString());
		} catch (Exception e) {
			log.error(null, e);;
		}
	}

}


public final class ClientHandlerFactory {
	private static final Log log = LogFactory.getLog(ClientHandlerFactory.class);
	
	public static I_ClientHandler getHandler(Socket iSocket, String iCommand) {
		I_ClientHandler r = null;
		
		if (iCommand != null) {
			Parameter params = new Parameter();
			params.parseCommand(iCommand);
			log.debug("handler type: [" + params.mRequestType + "]");
			if (params.mRequestType != null) {
				if (params.mRequestType.equals("info")) {
					r = new ClientHandler_info();
				}
			} else {
				r = new ClientHandler_telnet();
			}
		}
		return r;
	}
}
