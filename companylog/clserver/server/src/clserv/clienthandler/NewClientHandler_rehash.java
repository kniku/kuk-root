package clserv.clienthandler;

import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;
import clserv.tools.AppContext;

public class NewClientHandler_rehash implements I_ClientHandler /*extends AClientHandler*/ {

	@Override
	public void handle(I_ConnectionHandler iConn, String command) throws Exception {
		AppContext.getInstance().reset_all();
		iConn.sendMessageToClientImmediately(iConn.createResponse_ok(null));
	}

}
