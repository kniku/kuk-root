package clserv.shared;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import org.apache.log4j.Logger;

public final class ConnectionPool {
	private final Logger logger = Logger.getLogger(this.getClass());
	private int mMaxPoolsize_allTime = 0;
	private int mMaxPoolsize_limit = 0;
	private String mDriver = null;
	private String mConnect = null;
	private String mUser = null;
	private String mPass = null;
	private ArrayList<Connection> pool_free = new ArrayList<Connection>();
	private HashMap<Connection, Connection> pool_used = new HashMap<Connection, Connection>();
	
	private Connection getNewConnection() throws ClassNotFoundException, SQLException {
		Connection r = null;
		Class.forName(mDriver);
		r = DriverManager.getConnection(mConnect, mUser, mPass);
		r.setAutoCommit(false);
		int aps = getPoolsize_inUse() + getPoolsize_notInUse() + 1;
		if (aps > mMaxPoolsize_allTime) {
			mMaxPoolsize_allTime = aps;
		}
		return r;
	}
	public Connection getConnection() throws ClassNotFoundException, SQLException
	{
		Connection r = null;
		synchronized (this) {
			if (pool_free.isEmpty()) {
				r = getNewConnection();
				if (logger.isDebugEnabled()) logger.debug("Neue Connection erzeugt: [" + r.toString() + "]");
			} else {
				r = pool_free.remove(0);
				if (logger.isDebugEnabled()) logger.debug("Benutze vorhandene Connection: [" + r.toString() + "]");
			}
			if (r != null) {
				pool_used.put(r, r);
			}
		}
		return r;
	}
	
	public void releaseConnection(Connection iConnection) throws SQLException
	{
		synchronized (this) {
			Connection r = pool_used.remove(iConnection);
			if (r != null) {
				if (logger.isDebugEnabled()) logger.debug("Connection freigegeben: [" + r.toString() + "]");
				if (pool_free.size() < mMaxPoolsize_limit) {
					pool_free.add(r);
					if (logger.isDebugEnabled()) logger.debug("Connection im freien Pool abgelegt: [" + r.toString() + "]");
				} else {
					r.close();
					if (logger.isDebugEnabled()) logger.debug("Max. Poolgroesse Ueberschritten! Connection wurde geschlossen: [" + r.toString() + "]");
				}
			}
		}
	}

	public void closeAllConnections() {
		if (logger.isDebugEnabled()) logger.debug("Alle Datenbankverbindungen werden nun geschlossen!");
		synchronized (this) {
			Iterator<Connection> i = pool_free.iterator();
			Connection c;
			while (i.hasNext()) {
				c = i.next();
				try {
					c.close();
				} catch (SQLException e) {
					// just catch ...
				}
			}
			pool_free.clear();

			i = pool_used.values().iterator();
			while (i.hasNext()) {
				c = i.next();
				try {
					c.close();
				} catch (SQLException e) {
					// just catch it...
				}
			}
			pool_used.clear();
		}
	}
	
	public ConnectionPool(String driver, String connect, String user, String pass, int iMaxPoolSize) {
		mDriver = driver;
		mConnect = connect;
		mUser = user;
		mPass = pass;
		mMaxPoolsize_limit = iMaxPoolSize;
	}
	
	public void shutdown() {
		/* embedded hsqldb shutdown */
		if (mConnect.contains(":hsqldb:file:")) {
			try {
				Connection c = getConnection();
				Statement s = c.createStatement();
				s.execute("SHUTDOWN");
				s.close();
				releaseConnection(c);
			} catch (Exception e) {
				logger.fatal(null, e);
			}
		}
		/* closing all connections */
		closeAllConnections();
	}
	
	public int getPoolsize_inUse() {
		return pool_used.size();
	}
	public int getPoolsize_notInUse() {
		return pool_free.size();
	}
	public int getPoolsize_allTime() {
		return mMaxPoolsize_allTime;
	}
}
