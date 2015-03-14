package clserv.shared;

import java.sql.Timestamp;

public final class Helper {
	/**
	 * Aktuelle Zeit unter Beruecksichtigung der lokalen Zeitzone
	 * @return Aktuelle Zeit als Anzahl Millisekunden
	 */
	public static final long getCurrentTimeMillis() {
		//return System.currentTimeMillis();
//		return System.currentTimeMillis() + TimeZone.getTimeZone("Europe/Berlin").getRawOffset();
		//System.out.println(new java.util.Date());

		return System.currentTimeMillis();// + TimeZone.getTimeZone("CET").getRawOffset();
//		GregorianCalendar cal = new GregorianCalendar();
//		return cal.getTimeInMillis();
//		return System.currentTimeMillis() + TimeZone.getTimeZone("GMT-01:00").getRawOffset();
//		return new Date().getTime();
	}
	/**
	 * Aktuelle Zeit unter Beruecksichtigung der lokalen Zeitzone
	 * @return Aktuelle Zeit als SQL Timestamp
	 */
	public static final Timestamp getCurrentTime() {
		return new Timestamp(getCurrentTimeMillis());
	}
}
