package clserv.shared;

public final class GlobalConstants {
	// clserv property file und Defaultwerte der Konfig.
	public static final String CLServ_PROPERTY_FILE = "clserv.properties";
	public static final String CLServ_Port = "7880";
	public static final String CLServ_Port_deprecated = "0";	// disabled!
	// QMS Datenbank Defaultverbindung
	public static final String QMSDB_Default_User = "qms";
	public static final String QMSDB_Default_Pass = "zoso";
	public static final String QMSDB_Default_Connect = "jdbc:oracle:oci8:@";
	public static final String QMSDB_Default_Driver = "oracle.jdbc.driver.OracleDriver";
	public static final String CLServ_ServerName = "";
	public static final String CLServ_TimeoutClient = "10";
	public static final String CLServ_DBMaxPoolSize = "5";
	public static final String CLServ_TimeoutWatcher = "300";
}
