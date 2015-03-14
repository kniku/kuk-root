package bitools.database;

import java.sql.Connection;
import java.sql.DriverManager;

import org.apache.log4j.Logger;

import bitools.shared.AppContext;

public class ConnectionHelper {
	private static Logger logger = Logger.getLogger(ConnectionHelper.class);
	
	public static boolean checkDBConnection(AppContext iAppContext) {
		boolean r = false;
		// open/check database connection
		logger.debug("teste db connection: " + iAppContext.getProperty("DB_Url") + "...");
		Connection c;
		try {
			c = getDBConnection(iAppContext);
			c.close();
			logger.debug("db connection: ok");
			r = true;
		} catch (Exception e) {
			// just catch it
		}
		
		return r;
	}
	
	public static Connection getDBConnection(AppContext iAppContext) throws Exception  {
		String clazz = iAppContext.getProperty("DB_Driver");
		String url = iAppContext.getProperty("DB_Url");
		String user = iAppContext.getProperty("DB_User");
		String passwd = iAppContext.getProperty("DB_Password");
		Connection r = null;
		try {
			Class.forName(clazz);
			r = DriverManager.getConnection(url, user, passwd);
			r.setAutoCommit(false);
		} catch (Exception e) {
			logger.error("connection fehlgeschlagen! class=" + clazz + ", url=" + url + ", user=" + user, e);
			throw e;
		}
		return r;
	}
	public static Connection releaseDBConnection(Connection iConn) throws Exception  {
		if (iConn != null) {
			iConn.rollback();
			iConn.close();
		}
		return null;
	}
}
