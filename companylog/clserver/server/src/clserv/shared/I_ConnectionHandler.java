package clserv.shared;

public interface I_ConnectionHandler {
	public String getClientCommand();
	public void sendMessageToClientImmediately(String iMessage);
	public boolean sendMessageToUserImmediately(String iLogin, String iMessage);
	public String envelopClientMessage(String iType, String iAttrs, String iMessage);
	public String createResponse_error(String iMessage);
	public String createResponse_ok(String iMessage);
	public I_EmployeeManager getEmployeeManager();
}
