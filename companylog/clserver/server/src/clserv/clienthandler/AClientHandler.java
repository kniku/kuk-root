package clserv.clienthandler;

import clserv.shared.I_ClientHandler;
import clserv.tools.AConnectionHandler;


public abstract class AClientHandler implements I_ClientHandler {
	
	AConnectionHandler mConnection = null;
	
	public void handle(AConnectionHandler iConnection, String iCommand) throws Exception {
		mConnection = iConnection;
		handle(iCommand);
	}
	abstract void handle(String iCommand) throws Exception;
}
