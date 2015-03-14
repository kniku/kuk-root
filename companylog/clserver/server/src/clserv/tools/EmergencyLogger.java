package clserv.tools;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import org.apache.log4j.Logger;

import clserv.clienthandler.ClientHandlerFactory;
import clserv.model.dao.DaoES_LOGS;


public final class EmergencyLogger {
	
	private static final String EMERG_LOG_DIRECTORY = "log";
	private static final String EMERG_LOG_FILENAME = "emergency.pze";
	
	private final Logger logger = Logger.getLogger(this.getClass());

	// Singleton ...
	private static EmergencyLogger singleInstance = null;
	private EmergencyLogger() {}
	public static EmergencyLogger getInstance() {
		if (singleInstance == null) {
			singleInstance = new EmergencyLogger();		
		}
		return singleInstance;
	}
	// ...Singleton

	public static String getEmergencyLogDirectoryName() {
		return EMERG_LOG_DIRECTORY;
	}
	
	public static String getEmergencyLogFileName() {
		return EMERG_LOG_FILENAME;
	}
	
	public void handle(DaoES_LOGS iEntry, String iMessage) {
		FileWriter writer = null;
		try {
			writer = new FileWriter(EMERG_LOG_DIRECTORY+"/"+EMERG_LOG_FILENAME, true);
		} catch (IOException e) {
			File f = new File(EMERG_LOG_DIRECTORY);
			f.mkdir();
		}
		try {
			logger.error("Schreibe emergency log Eintrag -> " + EMERG_LOG_DIRECTORY+"/"+EMERG_LOG_FILENAME);
			if (writer == null) writer = new FileWriter(EMERG_LOG_DIRECTORY+"/"+EMERG_LOG_FILENAME, true);
			if (iMessage != null) {
				writer.write("# " + iMessage + "\n");
			}
			iMessage = ClientHandlerFactory.createResponse_pzeData_alt(iEntry);
			writer.write(iMessage);
			writer.close();
		} catch (IOException e) {
			logger.fatal("I/O Error beim Schreiben des Emergency Logs!", e);
		}
	}
}
