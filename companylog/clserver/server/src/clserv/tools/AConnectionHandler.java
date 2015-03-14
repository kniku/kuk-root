package clserv.tools;

import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;

import org.apache.log4j.Logger;

import clserv.shared.I_ConnectionHandler;



public abstract class AConnectionHandler extends Thread implements I_ConnectionHandler {
	private final Logger logger = Logger.getLogger(this.getClass());
	protected Socket mSocket = null;
	
	public AConnectionHandler() {
	}

	public AConnectionHandler(Socket iConn) {
		mSocket = iConn;
	}

	public void run() {
		logger.debug("Neuer Thread ["+getName()+"] wurde gestartet (Handler="+getHandlerName()+")");
		AppContext.getInstance().informAboutRunningThreads(activeCount());
		if (mSocket != null) {
			try {
				mSocket.setSoTimeout(AppContext.getInstance().getTimeoutClient() * 1000);	// timeout for blocking reads...
				run_handler();
			} catch (Exception e) {
				if (e instanceof java.net.SocketTimeoutException) {
					logger.debug("client connection: timeout!");
				} else logger.error(null, e);
			} finally {
				try {
					mSocket.close();
					logger.debug("Verbindung geschlossen");
				} catch (IOException e) {
					// just catch it...
				}
			}
		}
		logger.debug("Thread ["+getName()+"] wurde beendet");
	}
	
	public abstract void run_handler() throws Exception;
	public abstract String getHandlerName();

	/**
	 * Sendet die uebergebene Nachricht an den Client
	 * @param iMessage Die Nachricht fuer den Client
	 */
	public void sendMessageToClientImmediately(String iMessage) {
		try {
			if (logger.isTraceEnabled()) logger.trace("to client: ["+iMessage+"]");
			mSocket.getOutputStream().write(iMessage.getBytes("ISO-8859-1"));
			//mSocket.getOutputStream().flush();
		} catch (IOException e) {
			logger.error(null, e);
		}
	}

	/**
	 * Holt ueber die aktuelle Verbindung die naechste Message des Clients
	 * @return Empfangene Message
	 */
/*	public String getMessageFromClient() {
		String r = null;
		ByteBuffer buffer = ByteBuffer.allocate(2000);
		ByteBuffer cmdbuffer = ByteBuffer.allocate(2000);
		byte [] bb = buffer.array();
		try {
			while (mSocket.getInputStream().read(bb) >= 0) {
				if (cmdbuffer.remaining() > bb.length) {
					throw new IOException("input buffer size exceeded!");
				}
				cmdbuffer.put(buffer);
			}
			r = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");				
		} catch (IOException e) {
			logger.error(null, e);
		}
		return r;
	}
*/	
	/**
	 * Holt ueber die aktuelle Verbindung das naechste Kommando des Clients
	 * @return Empfangenes Kommando
	 */
	public String getClientCommand() {
		String r = null;
		ByteBuffer cmdbuffer = ByteBuffer.allocate(2000);
		ByteBuffer buffer = ByteBuffer.allocate(2000);
		int i, cnt;
		boolean cmdHasStarted = false, cmdHasFinished = false;
		byte [] bb = buffer.array();
		try {
			while (cmdHasFinished == false && (cnt = mSocket.getInputStream().read(bb)) >= 0) {
				for (i = 0; i < cnt; i++) {
					if (cmdHasStarted && bb[i] == '>') {
						cmdbuffer.put(bb[i]);
						r = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");
						cmdHasFinished = true;
						break;
					} else if (bb[i] == '<') {
						cmdbuffer.clear();
						cmdbuffer.put(bb[i]);
						cmdHasStarted = true;
					} else {
						cmdbuffer.put(bb[i]);
					}
				}
				if (cnt > 0 && cmdHasStarted == false && cmdHasFinished == false) {
					// old tool support
					r = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");
					// protokollumsetzung f�r jene handler, die ohne r�ckmeldung
					// an den client auskommen. Für die anderen muss es spezielle
					// handler geben...
					if (r.startsWith("es3.5:")) {
/*						if (r.substring(6, 10).equals("ENT:")) {
							// [es3.5:ENT:kuk                           :I:K:test                          ]
							//  012345678901234567890123456789012345678901234567890123456789012345678901234
							if (r.substring(40, 42).equals(":I")) {
								r = "in:"+r.substring(10, 13);
							} else {
								r = "out:"+r.substring(10, 13);
							}
						}
*/
					} else r = null;
					break;
				}
			}
		} catch (IOException e) {
			logger.error(e.toString(), e);
		}
		if (r == null) {
			logger.error("invalid command");
		} else {
			logger.debug("command received: ["+r+"]");
		}
		return r;
	}

	public Socket getSocket() {
		return mSocket;
	}
	public void setSocket(Socket socket) {
		mSocket = socket;
	}

}
