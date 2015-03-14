package clserv.tools;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

import org.apache.log4j.Logger;


public class Listener extends Thread {
	private final Logger logger = Logger.getLogger(this.getClass());
	private int mPort = 0;
	@SuppressWarnings("unchecked")
	private Class mHandlerClass = null;

	@SuppressWarnings("unchecked")
	public Listener(Class iHandlerClass, int iPort) {
		mHandlerClass = iHandlerClass;
		mPort = iPort;
	}
	
	public void run() {
		logger.info("Neuer Listener wurde gestartet (Connectionhandler="+mHandlerClass.getName()+")");
		logger.info("Erzeuge Server Socket auf Port " + mPort);
		try {
			ServerSocket s = new ServerSocket(mPort);
			do {
				logger.debug("Warte auf Verbindungsaufnahme (port:"+mPort+")...");
				Socket connection = s.accept();
				AppContext.getInstance().addConnection();
				logger.info("Neue Verbindung von "+connection.getRemoteSocketAddress().toString());
				try {
					AConnectionHandler h = (AConnectionHandler )mHandlerClass.newInstance();
					h.setSocket(connection);
					h.start();
				} catch(java.lang.IllegalAccessException e) {
					logger.error(null, e);
				} catch(java.lang.InstantiationException e) {
					logger.error(null, e);
				}
			} while (true);
		} catch (IOException e) {
			logger.error(null, e);
		}
	}	
}
