package clserv.tools;

import clserv.clienthandler.AClientHandler;
import clserv.clienthandler.ClientHandlerFactory;
import clserv.shared.I_EmployeeManager;


public class ConnectionHandler_deprecated extends AConnectionHandler {
//	private static Logger logger = Logger.getLogger(ConnectionHandler_deprecated.class);

/*	public ConnectionHandler_deprecated(Socket iConn) {
		super(iConn);
	}
*/
	public ConnectionHandler_deprecated() {
		super();
	}

	public void run_handler() throws Exception {
		String Command = getClientCommand();
		AClientHandler h = ClientHandlerFactory.getHandler(Command);
		if (h != null) {
			h.handle(this, Command);
		}
	}

	public String getHandlerName() {
		return "deprecated";
	}

	@Override
	public String createResponse_error(String message) {
		return null;
	}

	@Override
	public String createResponse_ok(String message) {
		return null;
	}

	@Override
	public String envelopClientMessage(String type, String attrs, String message) {
		return null;
	}

	@Override
	public boolean sendMessageToUserImmediately(String login, String message) {
		return false;
	}

	@Override
	public I_EmployeeManager getEmployeeManager() {
		return null;
	}

}
