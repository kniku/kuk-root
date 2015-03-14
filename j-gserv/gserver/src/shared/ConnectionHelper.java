package shared;

import java.io.IOException;
import java.net.Socket;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public final class ConnectionHelper {
	private static final Log log = LogFactory.getLog(ClientHandlerFactory.class);
	
	public static void sendMessageToClientImmediately(Socket iSocket, String iMessage) {
		try {
			if (log.isTraceEnabled()) log.trace("to client: ["+iMessage+"]");
			iSocket.getOutputStream().write(iMessage.getBytes("ISO-8859-1"));
			//mSocket.getOutputStream().flush();
		} catch (IOException e) {
			log.error(null, e);
		}
	}

	private static String envelopClientMessage(String iType, String iMessage) {
		if (iType == null) return null;
		if (iMessage == null) {
			return "<gsm t=\"" + iType + "\"/>";
		}
		return "<gsm t=\"" + iType + "\">" + iMessage + "</gsm>";
	}
	
	public static String createResponse_error(String iMessage) {
		return envelopClientMessage("error", iMessage);
	}
	public static String createResponse_ok(String iMessage) {
		return envelopClientMessage("ok", iMessage);
	}
	
}
