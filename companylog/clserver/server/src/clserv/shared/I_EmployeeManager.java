package clserv.shared;


public interface I_EmployeeManager {

	/**
	 * Suche eines Mitarbeiters in der globalen Mitarbeiterliste mit Vorname,Name
	 * @param iVorname Der Vorname des gesuchten Mitarbeiters
	 * @param iNachname Der Nachname des gesuchten Mitarbeiters
	 * @return Das gefundene Mitarbeiterobjekt oder null
	 */
	public I_Employee findeMitarbeiterMitNamen(String iVorname, String iNachname);
	/**
	 * Suche eines Mitarbeiters in der globalen Mitarbeiterliste
	 * @param iLogin Der gesuchte Mitarbeiter (Login)
	 * @return Das gefundene Mitarbeiterobjekt oder null
	 */
	public I_Employee findeMitarbeiterMitLogin(String iLogin);
}
