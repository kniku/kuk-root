package clserv.shared;

public interface I_ClientHandler {
	void handle(I_ConnectionHandler iConnection, String iCommand) throws Exception;
}