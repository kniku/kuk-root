package shared;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class Listener extends Thread {
	private final Log log = LogFactory.getLog(this.getClass());
	private int mPort = 0;

	public Listener(int iPort) {
		mPort = iPort;
	}

	@Override
	public void run() {
		log.info("Neuer Listener wurde gestartet");
		log.info("Erzeuge Server Socket auf Port " + mPort);
		try {
			ServerSocket s = new ServerSocket(mPort);
			do {
				log.debug("Warte auf Verbindungsaufnahme (port:"+mPort+")...");
				Socket connection = s.accept();
//				AppContext.getInstance().addConnection();
				log.info("Neue Verbindung von " + connection.getRemoteSocketAddress().toString());
				ConnectionHandler h = new ConnectionHandler(connection);
				h.start();
//				try {
//					AConnectionHandler h = (AConnectionHandler )mHandlerClass.newInstance();
//					h.setSocket(connection);
//					h.start();
//				} catch(java.lang.IllegalAccessException e) {
//					logger.error(null, e);
//				} catch(java.lang.InstantiationException e) {
//					logger.error(null, e);
//				}
			} while (true);
		} catch (IOException e) {
			log.error(null, e);
		}
	}

}
