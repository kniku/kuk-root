package plugins;
import java.sql.Connection;

import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Plugin;



public class Dummy_v2 implements I_Plugin {

	@Override
	public String getDescription() {
		return "Dummy Plugin";
	}

	@Override
	public String getVersion() {
		return "2.0.0";
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
