package clienthandler;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.nio.ByteBuffer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import shared.ConnectionHelper;
import shared.I_ClientHandler;
import shared.Version;

public final class ClientHandler_telnet implements I_ClientHandler {
	private final Log log = LogFactory.getLog(this.getClass());

	@Override
	public void handle(Socket iSocket, String iCmd) {
		try {
			iSocket.setSoTimeout(60 * 1000);  // timeout for blocking reads
		} catch (SocketException e) {
			// just catch it...
		}
		ConnectionHelper.sendMessageToClientImmediately(iSocket, "\r[gserv] " + Version.getAppVersionString() + ", type \"help\" if you need some... ;-)");
		do {
			ConnectionHelper.sendMessageToClientImmediately(iSocket, "\n\n\r>>");
			String cmd = getClientCommand(iSocket);
			if (cmd.equals("exit")) {
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "[gserv] bye...");
				break;
			} else if (cmd.equals("shutdown")) {
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "[gserv] i'm going down now...");
				System.exit(0);
			} else if (cmd.equals("help")) {
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "[gserv] valid commands are:\n\r");
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "        exit ....... leave session\n\r");
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "        shutdown ... shutdown gserv!\n\r");
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "        help ....... this help");
			} else {
				ConnectionHelper.sendMessageToClientImmediately(iSocket, "[gserv] ?");				
			}
		} while (true);
	}

	private String getClientCommand(Socket iSocket) {
		String r = null;
		ByteBuffer buffer = ByteBuffer.allocate(100);
		ByteBuffer cmdbuffer = ByteBuffer.allocate(100);
		byte [] bb = buffer.array();
		try {
			int cnt;
			while (r == null && (cnt = iSocket.getInputStream().read(bb)) >= 0) {
				if (cmdbuffer.remaining() < cnt) {
					throw new IOException("input buffer size exceeded!");
				}
				for (int i = 0; i < cnt; i++) {
					if (bb[i] == '\n' || bb[i] == '\r') {
						r = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");
						break;
					} else {
						cmdbuffer.put(bb[i]);
					}
				}
			}
		} catch (IOException e) {
			log.error(null, e);
		}
		if (log.isTraceEnabled()) log.trace("received: [" + r + "]");
		return r;
	}
}
