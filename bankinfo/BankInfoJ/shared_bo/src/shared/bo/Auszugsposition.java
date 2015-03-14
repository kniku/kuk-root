package shared.bo;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Date;

import org.apache.log4j.Logger;


public class Auszugsposition {
	private static Logger logger = Logger.getLogger(Auszugsposition.class);

	private String Info = null;
	private double Betrag = 0;
	private String Waehrung = null;
	private Date Datum = null;
	private Date Buchungsdatum = null;
	private long KtoId = 0;

	
	public Auszugsposition(long iKontoID) {
		KtoId = iKontoID;
	}
	public String getInfo() {
		return Info;
	}
	public void setInfo(String info) {
		Info = info;
	}
	public double getBetrag() {
		return Betrag;
	}
	public void setBetrag(double betrag) {
		Betrag = betrag;
	}
	public String getWaehrung() {
		return Waehrung;
	}
	public void setWaehrung(String waehrung) {
		Waehrung = waehrung;
	}
	public Date getDatum() {
		return Datum;
	}
	public void setDatum(Date datum) {
		Datum = datum;
	}
	public Date getBuchungsdatum() {
		return Buchungsdatum;
	}
	public void setBuchungsdatum(Date buchungsdatum) {
		Buchungsdatum = buchungsdatum;
	}

	public void reset() {
		Info = null;
		Betrag = 0;
		Waehrung = null;
		Datum = null;
		Buchungsdatum = null;		
	}
	
	public boolean save(Connection iConn) {
		boolean r = false;

		PreparedStatement stmt;
		try {
			stmt = iConn.prepareStatement(
				"insert into positionen (effdt,amt,memo,curcode,posteddt,ktoid) values (?,?,?,?,?,?)"
			);
//			stmt.setString(1, createMD5(this));
			stmt.setDate(1, new java.sql.Date(Datum.getTime()));
			stmt.setDouble(2, Betrag);
			stmt.setString(3, Info);
			stmt.setString(4, Waehrung);
			stmt.setDate(5, new java.sql.Date(Buchungsdatum.getTime()));
			stmt.setLong(6, KtoId);
			stmt.execute();
			r = true;
			stmt.close();
		} catch (SQLException e) {
			logger.error(e);
		}
		
		return r;
	}
	
	/*
	private String createMD5(Auszugsposition iPos) {
		String r = null;
		StringBuffer stringBuffer = new StringBuffer(255);
		MessageDigest md5;
		try {
			md5 = MessageDigest.getInstance("MD5");
			String input = iPos.KtoId + iPos.Betrag + iPos.Info + iPos.Waehrung + iPos.Buchungsdatum + iPos.Datum;
			md5.update(input.getBytes());
			Formatter f = new Formatter(stringBuffer);
			for (byte b : md5.digest()) {
				f.format("%02x", b);
			}
			r = stringBuffer.toString().toUpperCase();
			if (logger.isDebugEnabled()) {
				logger.debug("Pos String: " + input);
				logger.debug("Pos ID: " + r);
			}
		} catch (NoSuchAlgorithmException e) {
			logger.error(e);
		}
		return r;
	}
	*/
}
