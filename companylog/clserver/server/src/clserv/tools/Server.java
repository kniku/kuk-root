package clserv.tools;

import java.util.ArrayList;

import org.apache.log4j.Logger;

final class shutdown_thread extends Thread {
	private final Logger logger = Logger.getLogger(this.getClass());

	@Override
	public void run() {
		logger.info("SHUTDOWN HOOK started...");
		AppContext ac = AppContext.getInstance();
		ac.shutdown();
		logger.info("SHUTDOWN HOOK finished.");
	}
	
}

public class Server {
	private final Logger logger = Logger.getLogger(this.getClass());
	private boolean isRunning = false;
	private ArrayList<Listener> theListeners = new ArrayList<Listener>();
	
	public void start() {
		if (!isRunning) {
			
			logger.info("starting server...");
			
			AppContext ac = AppContext.getInstance();

			Runtime.getRuntime().addShutdownHook(new shutdown_thread());
			ac.reset_all();

			Listener L1 = new Listener(ConnectionHandler_main.class, ac.getMPort());
			if (L1 != null) {
				theListeners.add(L1);
				L1.start();
			}
			
			if (ac.getMPortDeprecated() > 0) {
				Listener L2 = new Listener(ConnectionHandler_deprecated.class, ac.getMPortDeprecated());
				if (L2 != null) {
					theListeners.add(L2);
					L2.start();
				}
			}
			
			logger.info("server STARTED.");
			isRunning = true;
		}
	}
	
	public void stop() {
		if (isRunning) {

			logger.info("stopping server...");

			for (Listener aListener : theListeners) {
				if (aListener != null) aListener. interrupt();
			}
			theListeners.clear();
			logger.info("server STOPPED.");
			isRunning = false;
		}
	}
/*
	public void runAll() {
		AppContext ac = AppContext.getInstance();

		ac.reset_all();

		if (ac.getMPortDeprecated() > 0) {
			Listener L2 = new Listener(ConnectionHandler_deprecated.class, ac.getMPortDeprecated());
			if (L2 != null) {
				L2.start();
			}
		}
		
		logger.info("Erzeuge main Server Socket auf Port " + ac.getMPort());
		try {
			ServerSocket s = new ServerSocket(ac.getMPort());
			do {
				logger.debug("Warte auf Verbindungsaufnahme (port:"+ac.getMPort()+")...");
				Socket connection = s.accept();
				ac.addConnection();
				logger.info("Neue Verbindung von "+connection.getRemoteSocketAddress().toString());
				ConnectionHandler_main h = new ConnectionHandler_main(connection);
				h.start();
			} while (true);
		} catch (IOException e) {
			logger.error(null, e);
		}
	}
	*/
	
}
