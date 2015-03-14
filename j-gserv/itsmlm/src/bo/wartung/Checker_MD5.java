package bo.wartung;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import bo.Translation;

public class Checker_MD5 extends Thread {
	private static final Log log = LogFactory.getLog(Checker_MD5.class);
	private long maxTranslations = 0;
	private long scannedTranslations = 0;
	private ArrayList<Translation> list_errors = new ArrayList<Translation>();
	private Connection conn;
	
	public long getMaxTranslations() {
		return maxTranslations;
	}

	public long getNumOfScannedTranslations() {
		return scannedTranslations;
	}
	
	public long getNumOfProblems() {
		synchronized (list_errors) {
			return list_errors.size();		
		}
	}
	public ArrayList<Translation> getListOfProblems() {
		return list_errors;
	}
	
	@Override
	public void run() {
		log.debug("thread starts: scanning translations (CheckMD5) - "+this+Thread.currentThread());
		
		try {
			countTranslations();
			scanTranslations();
			conn.rollback();
			conn.close();
		} catch (SQLException e) {
			log.error(e);
		}
		log.debug("thread ends: scanning translations (CheckMD5) - "+this+Thread.currentThread());
	}

	public Checker_MD5(Connection iConn) {
		conn = iConn;
	}

	private void countTranslations() throws SQLException {
		log.debug("get number of translations in database");
		try {
			PreparedStatement stmt = conn.prepareStatement("select count(*) from mltrans");
			stmt.execute();
			ResultSet rs = stmt.getResultSet();
			if (rs.next()) {
				maxTranslations = rs.getLong(1);
			}
			rs.close();
			stmt.close();
		} catch (SQLException e) {
			log.error(e);
			throw e;
		}
		log.debug("translations found: " + maxTranslations);
	}
	
	private void scanTranslations() throws SQLException {
		
		log.debug("scanning translations ...");
		long cntProblems = 0;
		try {
			PreparedStatement stmt = conn.prepareStatement("select lang,textkey,translation,textmd5,null,null,null,null,null,mcode,look,app,modul,dialog,isactive from mltrans");
			stmt.execute();
			ResultSet rs = stmt.getResultSet();
			while (rs.next()) {
//				try {
//					Thread.sleep(5);
//				} catch (InterruptedException e) {
//				}
				String md5_calc = shared.StringTools.getMD5Hash(rs.getString(2));
				String md5 = rs.getString(4);
				if (!md5.equalsIgnoreCase(md5_calc)) {
					cntProblems++;
					Translation T = new Translation(rs.getString(1),rs.getString(2),rs.getString(3),rs.getString(4), null, false);
					T.setVersion(rs.getString(5));
					T.setModul(rs.getString(6));
					T.setDialog(rs.getString(7));
					T.setDialogdescr(rs.getString(8));
					T.setDialogelement(rs.getString(9));
					T.setTr_mcode(rs.getString(10));
					T.setTr_look(rs.getString(11));
					T.setTr_app(rs.getString(12));
					T.setTr_modul(rs.getString(13));
					T.setTr_dialog(rs.getString(14));
					T.setTr_isactive(rs.getString(15));
					synchronized (list_errors) {
						list_errors.add(T);
					}
				}
				scannedTranslations++;
			}
			rs.close();
			stmt.close();
		} catch (SQLException e) {
			log.error(e);
			throw e;
		}
		maxTranslations = scannedTranslations;	// sync. numbers to get 100% (for sure)...
		log.debug("translation scan finished, problems found: " + cntProblems);
	}
	
}
