package clserv.clienthandler;

import clserv.model.dao.DaoES_LOGS;
import clserv.shared.I_ConnectionHandler;

public class ClientHandler_qmi_alt extends AClientHandler_qmi_base {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}
	
	@Override
	public Parameter getParameter(String command) {
		Parameter r = new Parameter();
		String flag = command.substring(41, 42);
		if (flag.equals("K")) {
			r.mFlag = ParameterFlag.Kuerzel;
		} else if (flag.equals("I")) {
				r.mFlag = ParameterFlag.Intern;
		} else if (flag.equals("F")) {
			r.mFlag = ParameterFlag.Firma;
		}
		r.mPattern = command.substring(43, 73).trim();
		r.mFirma = command.substring(74, 104).trim();
		return r;
	}
	
	@Override
	public boolean handle_ENTRY(String iCommand, DaoES_LOGS entry) {
		mConnection.sendMessageToClientImmediately(iCommand);
		return true;
	}

	@Override
	public boolean handle_ENTRYFINISH(String command, DaoES_LOGS entry) {
		return true;
	}

	@Override
	public boolean handle_ENTRYSTART(String command, DaoES_LOGS entry) {
		return true;
	}

	@Override
	public boolean handle_FINISH(String command) {
		return true;
	}

	@Override
	public boolean handle_START(String command) {
		return true;
	}
}
