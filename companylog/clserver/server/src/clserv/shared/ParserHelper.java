/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package clserv.shared;

import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;

/**
 *
 * @author kuk
 */
public class ParserHelper {
	public static final String Timestamp2String(Timestamp iTS) {
		String r = null;
		// TODO: implement Timestamp2String
		return r;
	}
	
	public static final Timestamp String2Timestamp(String iStr) {
		Timestamp r = null;
		if (iStr != null && iStr.length() == 14) {
			GregorianCalendar cal = new GregorianCalendar();
			cal.set(Calendar.YEAR, Integer.parseInt(iStr.substring(0, 4)));
			cal.set(Calendar.MONTH, Integer.parseInt(iStr.substring(4, 6)) - 1);
			cal.set(Calendar.DAY_OF_MONTH, Integer.parseInt(iStr.substring(6, 8)));
			cal.set(Calendar.HOUR_OF_DAY, Integer.parseInt(iStr.substring(8, 10)));
			cal.set(Calendar.MINUTE, Integer.parseInt(iStr.substring(10, 12)));
			cal.set(Calendar.SECOND, Integer.parseInt(iStr.substring(12, 14)));
			cal.set(Calendar.MILLISECOND, 0);
			r = new Timestamp(cal.getTimeInMillis());
		}
		return r;
	}
}
