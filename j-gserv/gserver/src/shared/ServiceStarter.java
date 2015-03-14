package shared;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

final class shutdown_thread extends Thread {
	private final Log log = LogFactory.getLog(this.getClass());

	@Override
	public void run() {
		log.info("SHUTDOWN HOOK started...");
		// ...
		log.info("SHUTDOWN HOOK finished.");
	}
	
}

public class ServiceStarter {
	
	private final Log log = LogFactory.getLog(this.getClass());
	private boolean isRunning = false;
	private Listener theListener = null;
	
	public void start() {
		if (!isRunning) {
			
			log.info("starting listener...");
			
			Runtime.getRuntime().addShutdownHook(new shutdown_thread());

			theListener = new Listener(1234);
			if (theListener != null) {
				theListener.start();
			}
			
			log.info("listeners STARTED.");
			isRunning = true;
		}
	}
	
	public void stop() {
		if (isRunning) {
			log.info("stopping listener...");
			if (theListener != null) theListener.interrupt();
			log.info("listeners STOPPED.");
			isRunning = false;
		}
	}

}
