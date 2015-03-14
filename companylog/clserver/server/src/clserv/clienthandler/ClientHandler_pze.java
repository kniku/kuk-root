package clserv.clienthandler;

import clserv.model.dao.DaoES_LOGS;
import clserv.shared.I_ConnectionHandler;

public class ClientHandler_pze extends AClientHandler_pze_base {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}
	@Override
	public Object getParameter(String command) {
		return null;
	}

	@Override
	public boolean handle_ENTRY(String iCommand, DaoES_LOGS iEntry) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeData(iEntry));
		return true;
	}

	@Override
	public boolean handle_ENTRYFINISH(String iCommand, DaoES_LOGS iEntry) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeDataFinish(iEntry));
		return true;
	}

	@Override
	public boolean handle_ENTRYSTART(String iCommand, DaoES_LOGS iEntry) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeDataStart(iEntry));
		return true;
	}

	@Override
	public boolean handle_FINISH(String iCommand) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeTail());
		return true;
	}

	@Override
	public boolean handle_START(String iCommand) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeHead());
		return true;
	}
}
