package clienthandler;

import java.net.Socket;

import shared.ConnectionHelper;
import shared.I_ClientHandler;
import shared.Version;

public class ClientHandler_info implements I_ClientHandler {

	@Override
	public void handle(Socket mSocket, String cmd) {
		String msg = Version.getAppVersionString();
		ConnectionHelper.sendMessageToClientImmediately(mSocket, msg);
	}

}
