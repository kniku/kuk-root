package clserv.clienthandler;

import java.sql.Connection;

import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Plugin;
import clserv.tools.AppContext;


public final class NewClientHandler_plugins implements I_ClientHandler /*extends AClientHandler*/ {

	I_Plugin m_PluginInstance = null;
	
	public NewClientHandler_plugins(I_Plugin iPlugin) {
		m_PluginInstance = iPlugin;
	}

	@Override
	public void handle(I_ConnectionHandler iConn, String iCommand) throws Exception {
		synchronized (m_PluginInstance) {
			Connection db_conn = null;
			if (m_PluginInstance.isDatabaseNeeded()) {
				db_conn = AppContext.getInstance().getDatabase().getConnection();
			}
			m_PluginInstance.handle(iConn, db_conn, iCommand);
			if (db_conn != null) {
				AppContext.getInstance().getDatabase().releaseConnection(db_conn);
			}
		}
		return;
	}

}
