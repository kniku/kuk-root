package clserv.clienthandler;

import java.io.IOException;

import clserv.shared.I_ConnectionHandler;


public class ClientHandler_in extends AClientHandler_es_logger {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	public void handle(String iCommand) throws IOException {
		handle(iCommand, clserv.model.tables.Columns.StrConstrENTRY_TYPE_In);
	}

}
