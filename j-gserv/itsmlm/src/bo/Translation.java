package bo;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;

import shared.StringTools;

public class Translation implements java.lang.Comparable<Translation> {
	private String version;			// mltext/optional
	private String modul;			// mltext/optional
	private String dialog;			// mltext/optional
	private String dialogdescr;		// mltext/optional
	private String dialogelement;	// mltext/optional
	private String tr_mcode;		// mltrans/optional
	private String tr_look;			// mltrans/optional
	private String tr_app;			// mltrans/optional
	private String tr_modul;		// mltrans/optional
	private String tr_dialog;		// mltrans/optional
	private String tr_isactive;		// mltrans/optional
	private String text;
	private String language;
	private String translation;
	private String md5;
	private boolean Historyable;
	private String updip;
	
	public Translation(String language, String text, String translation, String md5, String iUpdip, boolean iHistoryable) {
		this.language = language;
		this.text = text;
		this.translation = translation;
		setMd5(md5);
		updip = iUpdip == null ? "unknown" : iUpdip;
		Historyable = iHistoryable;
	}

	public String getModul() {
		return modul;
	}

	public void setModul(String modul) {
		this.modul = modul;
	}

	public String getDialog() {
		return dialog;
	}

	public void setDialog(String dialog) {
		this.dialog = dialog;
	}

	public String getDialogdescr() {
		return dialogdescr;
	}

	public void setDialogdescr(String dialogdescr) {
		this.dialogdescr = dialogdescr;
	}

	public String getDialogelement() {
		return dialogelement;
	}

	public void setDialogelement(String dialogelement) {
		this.dialogelement = dialogelement;
	}

	public String getTr_mcode() {
		return tr_mcode;
	}

	public void setTr_mcode(String trMcode) {
		tr_mcode = trMcode;
	}

	public String getTr_look() {
		return tr_look;
	}

	public void setTr_look(String trLook) {
		tr_look = trLook;
	}

	public String getTr_app() {
		return tr_app;
	}

	public void setTr_app(String trApp) {
		tr_app = trApp;
	}

	public String getTr_modul() {
		return tr_modul;
	}

	public void setTr_modul(String trModul) {
		tr_modul = trModul;
	}

	public String getTr_dialog() {
		return tr_dialog;
	}

	public void setTr_dialog(String trDialog) {
		tr_dialog = trDialog;
	}

	public String getTr_isactive() {
		return tr_isactive;
	}

	public void setTr_isactive(String trIsactive) {
		tr_isactive = trIsactive;
	}

	public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getText() {
		return text;
	}

/*	public void setText(String text) {
		this.text = text;
		setMd5(null);
	}
*/
	public String getTranslation() {
		return translation;
	}

	public void setTranslation(String translation) {
		this.translation = translation;
	}

	public String getLanguage() {
		return language;
	}

/*	public void setLanguage(String language) {
		this.language = language;
	}
*/
	public String getMd5() {
		return md5;
	}
	
	public void setMd5(String md5) {
		if (md5 == null || md5.isEmpty()) {
			this.md5 = StringTools.getMD5Hash(this.text);
		} else {
			this.md5 = md5;
		}
	}

	private void writeHistory(Connection iConn, String iUpdip, String iUpdkz) throws SQLException {
		if (Historyable) {
			PreparedStatement stmt = iConn.prepareStatement(
					"INSERT INTO mltrans_history"+
					" (MCODE,LOOK,LANG,APP,MODUL,DIALOG,TEXTKEY,TRANSLATION,ISACTIVE,TEXTMD5,updkz,updip,updts)"+ 
					" VALUES ('*','*',?,'ITSWEB','*','*',?,?,'Y',?,?,?,?)");
			stmt.setString(1, getLanguage());
			stmt.setString(2, getText());
			stmt.setString(3, getTranslation());
			stmt.setString(4, getMd5());
			stmt.setString(5, iUpdkz);
			stmt.setString(6, iUpdip);
			Timestamp ts = new Timestamp(System.currentTimeMillis());
			stmt.setTimestamp(7, ts);
			stmt.execute();
		}		
	}
	
	public void delete(Connection iConn) throws SQLException {
		PreparedStatement stmt = iConn.prepareStatement(
		"delete from mltrans where mcode='*' and look='*' and lang=? and app='ITSWEB' and modul='*' and dialog='*' and textkey=? and textmd5=?");
		stmt.setString(1, getLanguage());
		stmt.setString(2, getText());
		stmt.setString(3, getMd5());
		stmt.execute();
		writeHistory(iConn, updip, "D");
	}
	
	public void save(Connection iConn) throws SQLException {
		PreparedStatement stmt = iConn.prepareStatement(
				"select null from mltrans where mcode='*' and look='*' and lang=? and app='ITSWEB' and modul='*' and dialog='*' and textkey=?");
		stmt.setString(1, getLanguage());
		stmt.setString(2, getText());
		stmt.execute();
		ResultSet rs = stmt.getResultSet();
		boolean found = rs.next();
		
		if (found) {
			stmt = iConn.prepareStatement(
					"update mltrans set translation=?, textmd5=? where mcode='*' and look='*' and lang=? and app='ITSWEB' and modul='*' and dialog='*' and textkey=?");
			stmt.setString(1, getTranslation());
			stmt.setString(2, getMd5());
			stmt.setString(3, getLanguage());
			stmt.setString(4, getText());
			stmt.execute();
			writeHistory(iConn, updip, "C");
		} else {
			stmt = iConn.prepareStatement(
					"INSERT INTO mltrans"+
					" (MCODE,LOOK,LANG,APP,MODUL,DIALOG,TEXTKEY,TRANSLATION,ISACTIVE,TEXTMD5)"+ 
					" VALUES ('*','*',?,'ITSWEB','*','*',?,?,'Y',?)");
			stmt.setString(1, getLanguage());
			stmt.setString(2, getText());
			stmt.setString(3, getTranslation());
			stmt.setString(4, getMd5());
			stmt.execute();
			writeHistory(iConn, updip, "N");
		}
		stmt.close();
	}

	@Override
	public int compareTo(Translation arg0) {
		return this.text.compareTo(arg0.text);
	}
}
