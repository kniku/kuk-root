package plugins;
import java.sql.Connection;

import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Plugin;



public class Dummy implements I_Plugin {

	@Override
	public String getDescription() {
		return "Dummy Plugin";
	}

	@Override
	public String getVersion() {
		return "1.0.3";
	}

	@Override
	public void onNewInstance() {
		// do some initializations ...
	}

	@Override
	public boolean isDatabaseNeeded() {
		return false;
	}
	
	@Override
	public void handle(I_ConnectionHandler iClientConnection, Connection iDatabaseConnection, String iCommand) {
		iClientConnection.sendMessageToClientImmediately(iClientConnection.envelopClientMessage("dummy", "v=\""+getVersion()+"\"", getDescription()));
	}


}
