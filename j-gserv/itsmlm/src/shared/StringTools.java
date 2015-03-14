package shared;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Formatter;

public final class StringTools {

	public final static String getMD5Hash(String input) {
		String r = null;
		StringBuffer stringBuffer = new StringBuffer(255);
		MessageDigest md5;
		try {
			md5 = MessageDigest.getInstance("MD5");
			md5.update(input.getBytes());
			Formatter f = new Formatter(stringBuffer);
			for (byte b : md5.digest()) {
				f.format("%02x", b);
			}
			r = stringBuffer.toString().toUpperCase();
		} catch (NoSuchAlgorithmException e) {
			// just catch it...
		}
		return r;
	}
	
	public final static boolean isEmptyStrg(String iString) {
		boolean r = false;
		if (iString == null || iString.isEmpty()) r = true;
		return r;
	}
}
