package clserv.clienthandler;

import java.io.IOException;
import java.text.DecimalFormat;

import clserv.shared.I_ConnectionHandler;
import clserv.tools.AppContext;


public class ClientHandler_info_alt extends AClientHandler {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	public void handle(String iCommand) throws IOException {
		AppContext ac = AppContext.getInstance();
		mConnection.sendMessageToClientImmediately("INF:"+
				clserv.server.getVersionString()+":"+
				new DecimalFormat("0.00").format(ac.getUptime()/1000/60/60F)+":"+
				ac.getCntConnections()+":"+
				Thread.activeCount());
		return;
	}

}
