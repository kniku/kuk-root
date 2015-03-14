package shared;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

import javax.sql.DataSource;


public class DBMSHelper {
	
	private static void setDefaults(Connection iConn) throws SQLException {
		if (iConn != null) {
			iConn.setAutoCommit(false);
		}
	}

	public static Connection getNewConnection(DataSource iDatasource) throws SQLException {
		Connection r = null;
		if (iDatasource != null) {
			r = iDatasource.getConnection();
			setDefaults(r);
		}
		return r;
	}
	
	public static Connection getNewConnection(String iDriver, String iConnect, String iUser, String iPass) throws ClassNotFoundException, SQLException {
		Connection r = null;
		Class.forName(iDriver);
		r = DriverManager.getConnection(iConnect, iUser, iPass);
		setDefaults(r);
		return r;
	}
}

