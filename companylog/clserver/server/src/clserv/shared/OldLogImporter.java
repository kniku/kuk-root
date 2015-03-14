package clserv.shared;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;

import org.apache.log4j.Logger;

import clserv.clienthandler.ClientHandlerFactory;
import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.model.tables.Columns;
import clserv.shared.I_Employee;
import clserv.shared.I_EmployeeManager;


public final class OldLogImporter {
	private static Logger logger = Logger.getLogger(OldLogImporter.class);
	private static final int LINELEN_OLD = 56;
	private static final int LINELEN_NEW = 102;
	private static BufferedWriter mErrorStream = null;
	
	private static void handle_errline(String iFilename, String iLine, String iDescr) throws IOException {
		if (mErrorStream == null) {
			mErrorStream = new BufferedWriter(new FileWriter(iFilename, true));
			mErrorStream.write("#\n# import errors ("+new Timestamp(System.currentTimeMillis()).toString()+")\n#\n");
		}
		if (iDescr != null) {
			mErrorStream.write("# ERROR: "+iDescr+"\n");
		}
		mErrorStream.write(iLine+"\n");
	}

	public static void importiere(String iFilename, String iErrorFilename, Connection mQMSDB, boolean iCommit, I_EmployeeManager iMAVerwaltung, OldLogImporterResult ioResult) {
		ioResult.reset();
		logger.info("Starte Import von "+iFilename);
		BufferedReader InputStream = null;
		Timestamp ts = new Timestamp(0);
		PreparedStatement stmt = null;
//		int line_count = 0;
		try {
			stmt = mQMSDB.prepareStatement(
			    	"insert into "+clserv.model.tables.ES_LOGS.getInstance().getName()+
			    	" ("+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName(null)+
			    	","+clserv.model.tables.ES_LOGS.ColumnENTRY_TYPE.getName(null)+
			    	","+clserv.model.tables.ES_LOGS.ColumnINFO.getName(null)+
			    	","+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName(null)+
			    	","+clserv.model.tables.ES_LOGS.ColumnORIGIN.getName(null)+
			    	") values (?,?,?,?,?)");
			InputStream = new BufferedReader(new FileReader(iFilename));
			DaoES_LOGS entry = new DaoES_LOGS("", new Timestamp(0));
			while (InputStream.ready()) {
				ioResult.file_lines++;
				String line = InputStream.readLine();
				if (line.startsWith("#") || line.trim().isEmpty()) {
					// comment or empty line
					continue;
				}
				if ((line.startsWith("00,0000004702,") && line.length() >= LINELEN_NEW) || (line.startsWith("00,") && line.length() >= LINELEN_OLD)) {
//ALT:00,0000004702,06:35:31,20-11-02,IN,00,Rauch           ,Jorgen         .              
//NEU:00,0000004702,07:52:37,04-06-07,OU,00,Unger           ,Erwin      (529),(eur) bin nur kurz weg       .
					String name, vname, info;
					name = line.substring(38, 54).trim();
					if (line.length() < LINELEN_NEW) {
						vname = line.substring(55);
					} else {
						vname = line.substring(55, 71);						
					}
					int pos;
					if ((pos = vname.lastIndexOf("(")) > 0) {
						vname = vname.substring(0, pos);
					}
					vname = vname.trim();
					I_Employee i_ma = iMAVerwaltung.findeMitarbeiterMitNamen(vname, name);
					if (i_ma == null) {
						i_ma = iMAVerwaltung.findeMitarbeiterMitLogin(name.toLowerCase());
					}
					if (i_ma == null && name.equals("Jahrbacher")) {
						i_ma = iMAVerwaltung.findeMitarbeiterMitNamen(vname, "Kunz");
					}
					if (i_ma == null && name.equals("Zielinksi")) {
						i_ma = iMAVerwaltung.findeMitarbeiterMitNamen(vname, "Zielinski");
					}
					if (i_ma == null && name.equals("Krutzler")) {
						i_ma = iMAVerwaltung.findeMitarbeiterMitLogin("klk");
					}
					if (i_ma == null && name.equals("Draxler")) {
						i_ma = iMAVerwaltung.findeMitarbeiterMitLogin("dre");
					}
					if (i_ma == null && name.equals("mbicha")) {
						i_ma = iMAVerwaltung.findeMitarbeiterMitLogin("mbi");
					}
					Mitarbeiter ma = (Mitarbeiter )i_ma;
					if (i_ma == null && name.length() == 3) {
						ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(name.toLowerCase());
					}
					if (ma != null) {
						try {
							entry.setLogin(ma.getLogin());
							if (line.length() < LINELEN_NEW) {
								info = "";
							} else {
								info = line.substring(72);
								if (info.startsWith("(")) {
									info = info.substring(5);
								}
							}
							entry.setOrigin(Columns.StrConstrORIGIN_Import);
							entry.setInfo(info.trim());
							entry.setEntry_type(line.substring(32, 34).equals("IN") ? clserv.model.tables.Columns.StrConstrENTRY_TYPE_In : clserv.model.tables.Columns.StrConstrENTRY_TYPE_Out);
							ts.setTime(ClientHandlerFactory.createDateFormat_alt().parse(line.substring(14, 31)).getTime());
							entry.setEntry_ts(ts);
							
							if (iCommit) {
								stmt.setString(1, entry.getLogin());
								stmt.setString(2, entry.getEntry_type());
								stmt.setString(3, entry.getInfo());
								stmt.setTimestamp(4, entry.getEntry_ts());
								stmt.setString(5, entry.getOrigin());
								int insert_err_cnt = 0;
								do {
									try {
										stmt.execute();
										mQMSDB.commit();
										if (insert_err_cnt > 0) logger.info("Erneutes Insert OK");
										ioResult.imports_ok++;
										break;
									} catch (SQLException e) {
										mQMSDB.rollback();
										ioResult.imports_dbl++;
										insert_err_cnt++;
										if (insert_err_cnt < 1) {
											logger.info("Duplikat: "+entry.toString()+"; Erhoehe Timestamp um 1 Sekunde...");
											GregorianCalendar cal = new GregorianCalendar();
											cal.setTime(ts);
											cal.add(Calendar.SECOND, 1);
											ts.setTime(cal.getTimeInMillis());
											stmt.setTimestamp(4, ts);
											entry.setEntry_ts(ts);
											logger.info("Versuche erneutes Insert: "+entry.toString());
										} else {
											logger.error(e);
											ioResult.imports_err++;
											handle_errline(iErrorFilename, line, "duplicate");
										}
									}
								} while (insert_err_cnt > 0 && insert_err_cnt < 1);
							}
						} catch (Exception e) {
							ioResult.imports_err++;
							logger.error(e);
							handle_errline(iErrorFilename, line, "db error");
							if (iCommit) {
								break;
							}
						}
					} else {
						ioResult.imports_err++;
						logger.error(iFilename + ": unbekannter Mitarbeiter ("+name+","+vname+")!");
						handle_errline(iErrorFilename, line, "unknown employee: ("+name+","+vname+")");
					}
				} else {
					ioResult.imports_err++;
					logger.info(iFilename + ": ungueltige Zeile ("+ioResult.file_lines+")!");
					handle_errline(iErrorFilename, line, "line invalid ("+ioResult.file_lines+")");
				}
			}
		} catch (Exception e) {
			ioResult.imports_err++;
			logger.error(e);
		}
		try {
			if (iCommit) {
				mQMSDB.commit();
			} else {
				mQMSDB.rollback();
			}
			if (stmt != null) {
				stmt.close();
			}
		} catch (Exception e) {
			logger.error(e);			
		}
		try {
			if (InputStream != null) InputStream.close();
		} catch (IOException e) {
			logger.error(e);
		}
		logger.info(iFilename + ": Zeilen="+ioResult.file_lines);
		try {
			if (mErrorStream != null) {
				mErrorStream.close();
				mErrorStream = null;
			}
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
