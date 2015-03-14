package clserv.model;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Iterator;

import clserv.model.dao.DaoES_LOGS;
import clserv.model.dao.DaoES_PRELOGS;
import clserv.shared.I_Employee;



public class Mitarbeiter implements I_Employee {
	// gender types
	public enum Gender {MALE, FEMALE, UNKNOWN}
	
	private String login;	// qmperkubez
	private String name = null;	// qmpernname
	private String vname = null;	// qmpervname
	private String durchwahl = null;	// qmperteldw
	private String telprivat = null;	// QMPERTELPR
	private String telprivat2 = null;	// QMPERTELP2
	private String handyprivat = null;	// QMPERTHYPR
	private String telfirma = null;	// QMPERTELFA
	private String handyfirma;	// qmperthyfa
	private Gender gender = Gender.UNKNOWN;	// qmpergesch
	private String autonummer;	// QMPERPGRPR
	private boolean aktiv = false;	// QMPERAKTIV
	private String position = null;	// QMPERFPOSN

	private boolean anonym = false;	// Anonymer/Neuer Mitarbeiter?
	
	private ArrayList<DaoES_LOGS> mLogs = null;
	private ArrayList<DaoES_PRELOGS> mPreLogs = null;
	
	public Mitarbeiter(String login) {
		this.login = login;
	}
	@Override
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	@Override
	public String getFirstName() {
		return vname;
	}
	public void setVname(String vname) {
		this.vname = vname;
	}
	public String getLogin() {
		return login;
	}
	public String getDurchwahl() {
		return durchwahl;
	}
	public void setDurchwahl(String durchwahl) {
		this.durchwahl = durchwahl;
	}
	public boolean isAktiv() {
		return aktiv;
	}
	public void setAktiv(boolean aktiv) {
		this.aktiv = aktiv;
	}
	
	static public Mitarbeiter erzeugeAnonymenMitarbeiter(String iLogin) {
		Mitarbeiter r = new Mitarbeiter(iLogin);
		r.name = "Doe("+iLogin+")";
		r.vname = "Jane/John";
		r.gender = Gender.UNKNOWN;
		r.durchwahl = "---";
		r.aktiv = true;
		r.anonym = true;
		return r;
	}
	public static boolean istAnonymerMitarbeiter(Mitarbeiter ma) {
		return ma.anonym;
	}
	public Gender getGender() {
		return gender;
	}
	public void setGender(Gender gender) {
		this.gender = gender;
	}
	public boolean getActive() {
		return aktiv;
	}
	
	public String toString() {
		String r = super.toString();
		if (r != null) {
			r = r+" ["+getLogin()+":"+getFirstName()+":"+getName()+":"+getGender()+":"+getDurchwahl()+":active="+getActive()+":anonym="+anonym+"]";
		}
		return r;
	}

	/**
	 * Liefert den entsprechenden Log-Eintrag f�r diesen Mitarbeiter oder null, wenn keiner vorhanden
	 * Ist iIndex < 0, so wird der allerletzte geliefert
	 * @return Der entsprechende Log-Eintrag oder null
	 */
	public DaoES_LOGS getLog(int iIndex) {
		DaoES_LOGS r = null;
		if (mLogs != null) {
			if (iIndex < 0) {
				iIndex = mLogs.size() - 1;
			}
			if (iIndex >= 0 && iIndex < mLogs.size()) r = mLogs.get(iIndex);
		}
		return r;
	}
	/**
	 * Liefert den entsprechenden PreLog-Eintrag f�r diesen Mitarbeiter oder null, wenn keiner vorhanden
	 * Ist iIndex < 0, so wird der allerletzte geliefert
	 * @return Der entsprechende PreLog-Eintrag oder null
	 */
	public DaoES_PRELOGS getPreLog(int iIndex) {
		DaoES_PRELOGS r = null;
		if (mPreLogs != null) {
			if (iIndex < 0) {
				iIndex = mPreLogs.size() - 1;
			}
			if (iIndex >= 0 && iIndex < mPreLogs.size()) r = mPreLogs.get(iIndex);
		}
		return r;
	}

	public void removePreLog(int iIndex) {
		if (mPreLogs != null) {
			if (iIndex < 0 || iIndex >= mPreLogs.size()) {
				return;
			}
			mPreLogs.remove(iIndex);
		}
	}
	
	/**
	 * Einfügen eines neuen Log-Eintrags für diesen Mitarbeiter
	 * @param iLog Der neue Eintrag
	 */
	public void addLog(DaoES_LOGS iLog) {
		if (mLogs == null) {
			mLogs = new ArrayList<DaoES_LOGS>();
		}
		mLogs.add(iLog);
	}
	/**
	 * Einfügen eines neuen PreLog-Eintrags für diesen Mitarbeiter
	 * @param iPreLog Der neue Eintrag
	 */
	public void addPreLog(DaoES_PRELOGS iPreLog) {
		if (mPreLogs == null) {
			mPreLogs = new ArrayList<DaoES_PRELOGS>();
		}
		mPreLogs.add(iPreLog);
	}

	/**
	 * Liefert einen Iterator der aktuelle Logliste des Mitarbeiters, oder null, wenn kein Eintrag vorhanden
	 * @return Iterator der Logs
	 */
	public Iterator<DaoES_LOGS> getLogIterator() {
		return mLogs != null ? mLogs.iterator() : null;
	}
	/**
	 * Liefert einen Iterator der aktuelle PreLogliste des Mitarbeiters, oder null, wenn kein Eintrag vorhanden
	 * @return Iterator der PreLogs
	 */
//	public Iterator<DaoES_PRELOGS> getPreLogIterator() {
//		return mPreLogs != null ? mPreLogs.iterator() : null;
//	}
	
	/**
	 * Liefert die aktuelle Anzahl der Log Einträge für diesen Mitarbeiter
	 * @return Anzahl Log Einträge, sofern vorhanden, sonst 0
	 */
	public int getLogCount() {
		return mLogs == null ? 0 : mLogs.size();
	}
	/**
	 * Liefert die aktuelle Anzahl der PreLog Einträge für diesen Mitarbeiter
	 * @return Anzahl PreLog Einträge, sofern vorhanden, sonst 0
	 */
//	public int getPreLogCount() {
//		return mPreLogs == null ? 0 : mPreLogs.size();
//	}
	
	/**
	 * Ausleeren der Log Eintragsliste dieses Mitarbeiters
	 */
	public void resetLogs() {
		if (mLogs != null) {
			mLogs.clear();
		}
	}
	/**
	 * Ausleeren der PreLog Eintragsliste dieses Mitarbeiters
	 */
//	public void resetPreLogs() {
//		if (mPreLogs != null) {
//			mPreLogs.clear();
//		}
//	}
	public String getTelprivat() {
		return telprivat;
	}
	public void setTelprivat(String telprivat) {
		this.telprivat = telprivat;
	}
	public String getTelprivat2() {
		return telprivat2;
	}
	public void setTelprivat2(String telprivat2) {
		this.telprivat2 = telprivat2;
	}
	public String getHandyprivat() {
		return handyprivat;
	}
	public void setHandyprivat(String handyprivat) {
		this.handyprivat = handyprivat;
	}
	public String getTelfirma() {
		return telfirma;
	}
	public void setTelfirma(String telfirma) {
		this.telfirma = telfirma;
	}
	public String getHandyfirma() {
		return handyfirma;
	}
	public void setHandyfirma(String handyfirma) {
		this.handyfirma = handyfirma;
	}
	public String getAutonummer() {
		return autonummer;
	}
	public void setAutonummer(String autonummer) {
		this.autonummer = autonummer;
	}
	public String getGenderAsString() {
		if (gender == Gender.MALE) {
			return "M";
		} else if (gender == Gender.FEMALE) {
			return "W";
		} else {
			return "?";
		}
	}
	public String getPosition() {
		return position;
	}
	public void setPosition(String position) {
		this.position = position;
	}
	/**
	 * Entfernt alle veralteten Logs dieses Mitarbeiters.
	 * Achtung: Nur synchronized verwenden!
	 * Veraltet: < Heute 00:00:00
	 */
	public void cleanup_old_logs() {
		Calendar cal = GregorianCalendar.getInstance();
		cal.set(Calendar.MILLISECOND, 0);
		cal.set(Calendar.SECOND, 0);
		cal.set(Calendar.MINUTE, 0);
		cal.set(Calendar.HOUR, 0);
		Timestamp ts_heute0uhr = new Timestamp(cal.getTimeInMillis());
		DaoES_LOGS e;
		while ((e = getLog(0)) != null) {
			if (e.getEntry_ts().before(ts_heute0uhr)) {
				mLogs.remove(0);
			} else {
				break;
			}
		}
		
		DaoES_PRELOGS epl;
		while ((epl = getPreLog(0)) != null) {
			if (epl.getEntry_ts().before(ts_heute0uhr)) {
				mPreLogs.remove(0);
			} else {
				break;
			}
		}
	}
}
