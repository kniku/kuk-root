package shared;

import java.net.Socket;

public interface I_ClientHandler {

	void handle(Socket mSocket, String cmd);

}
