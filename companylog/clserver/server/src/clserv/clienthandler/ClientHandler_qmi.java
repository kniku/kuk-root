package clserv.clienthandler;

import clserv.model.dao.DaoES_LOGS;
import clserv.shared.I_ConnectionHandler;

public class ClientHandler_qmi extends AClientHandler_qmi_base {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	@Override
	public Parameter getParameter(String command) {
		return null;
	}
	
	@Override
	public boolean handle_ENTRY(String command, DaoES_LOGS entry) {
		return false;
	}

	@Override
	public boolean handle_ENTRYFINISH(String command, DaoES_LOGS entry) {
		return false;
	}

	@Override
	public boolean handle_ENTRYSTART(String command, DaoES_LOGS entry) {
		return false;
	}

	@Override
	public boolean handle_FINISH(String command) {
		return false;
	}

	@Override
	public boolean handle_START(String command) {
		return false;
	}

}
