package bitools.xml2db;

import java.io.File;
import java.io.IOException;

import org.apache.log4j.Logger;

import bitools.database.ConnectionHelper;
import bitools.shared.AppContext;

public class xml2db {
	private static Logger logger = Logger.getLogger(xml2db.class);

	/**
	 * @param args
	 * @throws IOException 
	 */
	public static void main(String[] args) throws Exception {
		
		File f = new File("log4j.properties");
		if (f.exists()) {
			org.apache.log4j.PropertyConfigurator.configure(logger.getClass().getClassLoader().getResource(f.getName()));
		}
		
		logger.info("startup: xml2db, v0.1...");
		
		// Anwendung initialisieren
		AppContext ac = AppContext.getInstance("xml2db");
		// kommandozeilenparameter uebernehmen
		for (int i = 0; i < args.length; i++) {
			String param = args[i];
			if (param.startsWith("-")) {
				String val = "1";
				if (i + 1 < args.length && !args[i + 1].startsWith("-")) {
					val = args[++i];
				}
				ac.overruleProperty(param.substring(1), val);
			}
		}
		
		if (ac.getProperty("h") != null || ac.getProperty("help") != null) {
			System.out.print("Aufruf: xml2db [-DB_User user] [-DB_Url url] [-DB_Password pwd] [-DB_Driver driver] [-commit] [-help]");
			System.exit(0);	
		}
		
//		logger.debug(ac.getProperty("DB_User"));
//		logger.debug(ac.getProperty("commit"));
		
		// Datenbankverbindung checken
		if (ConnectionHelper.checkDBConnection(ac) == false) {
			System.exit(-1);
		}
		
		KontoinfoParser parser = new KontoinfoParser(new KontoinfoDBWriter());

		f = new File(ac.getProperty("DataDir"));
		logger.debug("DatDir:"+f.getAbsolutePath());
		if (!f.exists()) {
			logger.error("DataDir: existiert nicht!");
			System.exit(-1);
		}
		File backupdir = new File(f.getAbsolutePath()+"/Archive");
		logger.debug("ArchiveDir:"+backupdir.getAbsolutePath());
		if (!backupdir.exists()) {
			logger.debug("ArchiveDir: existiert nicht, wird angelegt...");
			backupdir.mkdir();
		}
		logger.debug("COMMIT=" + (ac.getProperty("commit") == null ? "0" : ac.getProperty("commit")));
		logger.debug("Verabeite Auszugsdateien...");
		String fl[] = f.list();
		if (fl != null) {
			for (String fn : fl) {
				File aFile = new File(f.getAbsolutePath()+"/"+fn);
				if (aFile.exists() && aFile.isFile() && aFile.canRead()) {
					logger.debug("Bearbeite Inputdatei:"+fn);
					parser.parseKontoinfoDatei(aFile.getAbsolutePath());
					if (ac.getProperty("commit") != null && ac.getProperty("commit").equals("1")) {
						logger.debug("Archiviere Inputdatei:"+fn);
						File backupfile = new File(backupdir.getAbsolutePath()+"/"+fn);
						if (!aFile.renameTo(backupfile)) {
							logger.error("Archivierung fehlgeschlagen! Beende Import.");
							System.exit(-1);
						}
					}
				}
			}
		}
		logger.debug("Fertig.");
	}
	

}
