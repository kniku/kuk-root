package shared;

import java.sql.Timestamp;

public final class Helper {
	/**
	 * Aktuelle Zeit unter Beruecksichtigung der lokalen Zeitzone
	 * @return Aktuelle Zeit als Anzahl Millisekunden
	 */
	public static final long getCurrentTimeMillis() {
		return System.currentTimeMillis();// + TimeZone.getTimeZone("CET").getRawOffset();
	}
	/**
	 * Aktuelle Zeit unter Beruecksichtigung der lokalen Zeitzone
	 * @return Aktuelle Zeit als SQL Timestamp
	 */
	public static final Timestamp getCurrentTime() {
		return new Timestamp(getCurrentTimeMillis());
	}
}
