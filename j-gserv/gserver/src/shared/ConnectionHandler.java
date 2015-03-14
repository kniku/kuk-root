package shared;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.nio.ByteBuffer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public final class ConnectionHandler extends Thread {

	private static final Log log = LogFactory.getLog(ConnectionHandler.class);
	private Socket mSocket = null;
	
	public ConnectionHandler(Socket iConn) {
		mSocket = iConn;
	}

	@Override
	public void run() {
		log.debug("Neuer Thread ["+getName()+"] wurde gestartet");
		if (mSocket != null) {
			try {
				mSocket.setSoTimeout(10 * 1000); // timeout for blocking reads
//				ConnectionHelper.sendMessageToClientImmediately(mSocket, ConnectionHelper.createResponse_ok(null));					
				String cmd = getClientCommand();
				I_ClientHandler h = ClientHandlerFactory.getHandler(mSocket, cmd);
				if (h == null) {
					ConnectionHelper.sendMessageToClientImmediately(mSocket, ConnectionHelper.createResponse_error("unknown/invalid command"));					
				} else {
					h.handle(mSocket, cmd);
				}
			} catch (SocketException e) {
				log.error(null, e);
			} finally {
				try {
					mSocket.close();
					log.debug("Verbindung geschlossen");
				} catch (IOException e) {
					// just catch it...
				}
			}
		}
		log.debug("Thread ["+getName()+"] wurde beendet");
	}

	public String getClientCommand() {
		String r = null;
		ByteBuffer buffer = ByteBuffer.allocate(100);
		ByteBuffer cmdbuffer = ByteBuffer.allocate(100);
		byte [] bb = buffer.array();
		try {
			int cnt;
			boolean cmdHasStarted = false, cmdHasFinished = false;
			while (cmdHasFinished == false && (cnt = mSocket.getInputStream().read(bb)) >= 0) {
				if (cmdbuffer.remaining() < cnt) {
					throw new IOException("input buffer size exceeded!");
				}
				for (int i = 0; i < cnt; i++) {
					if (cmdHasStarted && bb[i] == '>') {
						cmdbuffer.put(bb[i]);
						r = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");
						cmdHasFinished = true;
						break;
					} else if (bb[i] == '<') {
						cmdbuffer.clear();
						cmdbuffer.put(bb[i]);
						cmdHasStarted = true;
					} else if (cmdHasStarted) {
						cmdbuffer.put(bb[i]);
					}
				}
			}
		} catch (IOException e) {
			log.error(null, e);
		}
		if (r == null) {
			ConnectionHelper.sendMessageToClientImmediately(mSocket, ConnectionHelper.createResponse_error("timeout or unknown/invalid message"));
		} else {
			if (log.isTraceEnabled()) log.trace("received: [" + r + "]");
		}
		return r;
	}
	
}
