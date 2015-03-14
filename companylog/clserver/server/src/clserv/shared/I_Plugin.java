package clserv.shared;

import java.sql.Connection;

public interface I_Plugin {
	/**
	 * called once after class instantiation
	 */
	public void onNewInstance();
	/**
	 * @return your plugin version
	 */
	public String getVersion();
	/**
	 * @return your plugin description
	 */
	public String getDescription();
	/**
	 * @return If clserv database connection needed
	 * @see method handle(): parameter iDatabaseConnection
	 */
	public boolean isDatabaseNeeded();
	/**
	 * @param iClientConnection for sending/retrieving messages from client
	 * @param iDatabaseConnection a connection to the clserv database or null,
	 * depending on result of preceding isDatabaseNeeded() call...
	 * @param iCommand the initial client command/message
	 * @see isDatabaseNeeded()
	 */
	public void handle(I_ConnectionHandler iClientConnection, Connection iDatabaseConnection, String iCommand);
}
