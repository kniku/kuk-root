package clserv;
import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import clserv.model.Mitarbeiter;
import clserv.shared.Database;
import clserv.shared.GlobalConstants;
import clserv.shared.I_EmployeeManager;
import clserv.shared.OldLogImporter;
import clserv.shared.OldLogImporterResult;
import clserv.tools.AppContext;



public class importer implements I_EmployeeManager {
	private static final String Version = "1.0";
	private static HashMap<String, Mitarbeiter> logins = new HashMap<String, Mitarbeiter>();
	private static ArrayList<String> mFiles = new ArrayList<String>();
	private static int cnt_lines = 0;
	private static int cnt_errors = 0;
	private static int cnt_imports = 0;
	private static int cnt_duplicates = 0;
	private static boolean mCommit = false;
	private static String mck3dir = null;
	private static OldLogImporterResult mImpResult = null;
	private static Database mDatabase = null;
	
	public static void main(String[] args) {
		
		importer Importer = new importer();
		
		if (args == null || args.length < 1) {
			show_usage();
		}
		parse_args(args);
		read_properties();
		reloadMitarbeiter();
		System.out.println("===========================================");
		try {
			Connection mQMSDB = mDatabase.getConnection();
			Iterator<String> i = mFiles.iterator();
			while (i.hasNext()) {
				Importer.do_import(i.next(), mQMSDB);
			}
			mDatabase.releaseConnection(mQMSDB);
			System.out.println("===========================================");
			System.out.println("Importergebnis: Files: "+mFiles.size()+", Verarbeitete Zeilen: "+cnt_lines+
					", Imports: "+cnt_imports+", Duplicates:" + cnt_duplicates + ", Errors: "+cnt_errors);
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (SQLException e) {
			e.printStackTrace();
		}
		shutdown();
	}

	private static void shutdown() {
			try {
				if (mDatabase != null) mDatabase.shutdown();
			} catch (SQLException e) {
				e.printStackTrace();
			}
			System.out.println("Programm beendet.");
	}

	private void do_import(String iFilename, Connection mQMSDB) {
		if (mImpResult == null) mImpResult = new OldLogImporterResult();
		OldLogImporter.importiere(iFilename, mck3dir+"/import_errors.log", mQMSDB, mCommit, this, mImpResult);
		cnt_lines += mImpResult.file_lines;
		cnt_duplicates += mImpResult.imports_dbl;
		cnt_errors += mImpResult.imports_err;
		cnt_imports += mImpResult.imports_ok;
		System.out.println(iFilename + ": Zeilen=" + mImpResult.file_lines);
	}

	private static void parse_args(String[] args) {
		for (int i = 0; i < args.length ; i++) {
			if (args[i].startsWith("commit=")) {
				if (args[i].substring(7).equals("yes")) {
					mCommit = true;
				}
			}
			if (args[i].startsWith("ck3dir=")) {
				FileFilter fileFilter = new FileFilter() {
			        public boolean accept(File file) {
			            return file.isFile();
			        }
			    };
			    mck3dir = args[i].substring(7);
				File f = new File(mck3dir);
				File [] x = f.listFiles(fileFilter);
				for (int ii = 0; ii < x.length; ii++) {
					String fn = x[ii].getName().toLowerCase();
					if (fn.endsWith(".pze") || fn.endsWith(".dat")) {
						mFiles.add(x[ii].getPath());
					}
				}
			}
		}
		System.out.println("Inputfiles: "+mFiles.size());
		if (mCommit) {
			System.out.println("Commit=True");
		} else {
			System.out.println("Commit=False");
		}
	}


	private static void read_properties() {
		java.util.Properties mProperties = new java.util.Properties();
		try {
			System.out.println("Lese clserv property file: "+GlobalConstants.CLServ_PROPERTY_FILE);
			FileInputStream f = AppContext.getPropertyFile();
			mProperties.load(f);
			f.close();

			System.out.println("Verbindung mit QMS Datenbank ("+mProperties.getProperty("DB_Connection", GlobalConstants.QMSDB_Default_Connect)+"/"+mProperties.getProperty("DB_User", GlobalConstants.QMSDB_Default_User)+")...");
			Class.forName(mProperties.getProperty("DB_Driver", GlobalConstants.QMSDB_Default_Driver));
			mDatabase = clserv.shared.Database.createInstance(mProperties.getProperty("DB_Driver", GlobalConstants.QMSDB_Default_Driver),
					mProperties.getProperty("DB_Connection", GlobalConstants.QMSDB_Default_Connect),
					mProperties.getProperty("DB_User", GlobalConstants.QMSDB_Default_User),
					mProperties.getProperty("DB_Password", GlobalConstants.QMSDB_Default_Pass),
					5);
			System.out.println("Check der Tabelle ES_LOGS oder Versuch, sie zu erzeugen...");
			Connection mQMSDB = mDatabase.getConnection();
			Database.checkOrCreateTables(mQMSDB);
			mDatabase.releaseConnection(mQMSDB);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			show_usage();
		} catch (IOException e) {
			e.printStackTrace();
			show_usage();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			show_usage();
		} catch (SQLException e) {
			e.printStackTrace();
			show_usage();
		} catch (Exception e) {
			e.printStackTrace();
			show_usage();
		}
	}

	private static void show_usage() {
		System.out.println("EcoImporter, v" + Version);
		System.out.println("Aufruf: EcoImporter [ck3dir=PZEDIR] [commit=NO/yes]");
		System.exit(0);
	}

	private static void reloadMitarbeiter() {
		try {
			Connection mQMSDB = mDatabase.getConnection();
			logins = mDatabase.reloadMitarbeiterWithConnection(mQMSDB, logins);
			mDatabase.releaseConnection(mQMSDB);
			System.out.println("Mitarbeiter geladen: "+logins.size());
		} catch (SQLException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	// ==========================================
	// Implementierung von IMitarbeiterVerwaltung
	// ==========================================

	@Override
	public Mitarbeiter findeMitarbeiterMitLogin(String iLogin) {
		return logins.get(iLogin);
	}

	@Override
	public Mitarbeiter findeMitarbeiterMitNamen(String iVorname, String iNachname) {
		Iterator<Mitarbeiter> iter = logins.values().iterator();
		Mitarbeiter ma, r = null;
		while (iter.hasNext()) {
			ma = iter.next();
			if (ma.getName().equals(iNachname) && ma.getFirstName().equals(iVorname)) {
				r = ma;
				break;
			}
		}
		return r;
	}
}
