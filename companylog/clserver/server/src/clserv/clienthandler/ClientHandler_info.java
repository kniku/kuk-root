package clserv.clienthandler;

import java.io.IOException;

import clserv.shared.I_ConnectionHandler;


public class ClientHandler_info extends AClientHandler {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	public void handle(String iCommand) throws IOException {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_info());
		return;
	}

}
