package bo.trados;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.Vector;

public class TradosFileImporter {

	static final String TOK_TXT_START = "{^}MLM:";
	static final int TOK_TXT_START_LEN = TOK_TXT_START.length();
	static final String TOK_TXT_END = "{&}";
	static final int TOK_TXT_END_LEN = TOK_TXT_END.length();

	public static Vector<String>loadTexts(String iFilename) throws Exception {
		Vector<String> r = new Vector<String>();
		String newTxt, lastTxt = "";
		
		InputStreamReader in = new InputStreamReader(new FileInputStream(iFilename), "UTF8");
		Reader in2 = new BufferedReader(in);
		char[] ioBuf = new char[65536];
		int cnt;
		while ((cnt = in2.read(ioBuf)) != -1) {
			newTxt = lastTxt + new String(ioBuf, 0, cnt);// (ioBuf);
			int start, end = 0, last = 0;
			while ((start = newTxt.indexOf(TOK_TXT_START, end)) >= 0 && (end = newTxt.indexOf(TOK_TXT_END, start)) >= 0 ) {
				start += TOK_TXT_START_LEN;
//				System.out.println("#"+ newTxt.substring(start, end) +"#");
				r.add(newTxt.substring(start, end));
				last = end + TOK_TXT_END_LEN;
			}
			if (last < newTxt.length()) {
				lastTxt = newTxt.substring(last);
			} else {
				lastTxt = "";
			}
		}
		in.close();
		return r;
	}

}
