package shared.bo;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.apache.log4j.Logger;

public class Konto {
	private static Logger logger = Logger.getLogger(Konto.class);

	private long Id = 0;
	private String Blz = null;
	private String Kontonummer = null;
	private String Waehrung = null;
	private String Kontoname = null;
	private String Beschreibung = null;
	
//	public Konto(String kontonummer) {
//		Kontonummer = kontonummer;
//	}
	public String getKontonummer() {
		return Kontonummer;
	}
	public void setKontonummer(String kontonummer) {
		Kontonummer = kontonummer;
	}
	public String getWaehrung() {
		return Waehrung;
	}
	public void setWaehrung(String waehrung) {
		Waehrung = waehrung;
	}
	public long getId() {
		return Id;
	}
	public String getBlz() {
		return Blz;
	}
	public void setBlz(String blz) {
		Blz = blz;
	}
	public String getKontoname() {
		return Kontoname;
	}
	public void setKontoname(String kontoname) {
		Kontoname = kontoname;
	}
	public String getBeschreibung() {
		return Beschreibung;
	}
	public void setBeschreibung(String beschreibung) {
		Beschreibung = beschreibung;
	}
	public void setId(long id) {
		Id = id;
	}
	
	public boolean loadWithAccountNumber(Connection iConn) {
		boolean r = false;

		PreparedStatement stmt;
		try {
			stmt = iConn.prepareStatement(
					"select id,blz,ktonr,curcode from konten where ktonr=?"
			);
			stmt.setString(1, Kontonummer);
			stmt.execute();
			ResultSet rs = stmt.getResultSet();
			if (rs.next()) {
				Id = rs.getLong(1);
				Waehrung = rs.getString(4);
				r = true;
			}
			stmt.close();
		} catch (SQLException e) {
			logger.error(e);
		}
		
		return r;
	}
	
	
}
