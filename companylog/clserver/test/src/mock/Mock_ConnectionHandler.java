package mock;

import test.ecoserv.plugins.Mock_EmployeeManager;
import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_EmployeeManager;

public class Mock_ConnectionHandler implements I_ConnectionHandler {

	private clserv.tools.ConnectionHandler_main m_ch = new clserv.tools.ConnectionHandler_main(null);
	private I_Mock_Server m_ServerMock = null;
	
	public Mock_ConnectionHandler(I_Mock_Server iServerMock) {
		m_ServerMock = iServerMock;
	}

	@Override
	public String createResponse_error(String message) {
		return m_ch.createResponse_error(message);
	}

	@Override
	public String createResponse_ok(String message) {
		return m_ch.createResponse_ok(message);
	}

	@Override
	public String envelopClientMessage(String type, String attrs, String message) {
		return m_ch.envelopClientMessage(type, attrs, message);
	}

	@Override
	public String getClientCommand() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void sendMessageToClientImmediately(String iMessage) {
		m_ServerMock.directFromServer(iMessage);
	}

	@Override
	public boolean sendMessageToUserImmediately(String iLogin, String iMessage) {
		return m_ServerMock.chatFromServer(iLogin, iMessage);
	}

	@Override
	public I_EmployeeManager getEmployeeManager() {
		return new Mock_EmployeeManager();
	}

}
