package clserv.clienthandler;

import java.io.IOException;

import clserv.shared.Constants;
import clserv.shared.I_ConnectionHandler;


public class ClientHandler_help extends AClientHandler {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	public void handle(String iCommand) throws IOException {
		mConnection.sendMessageToClientImmediately("\n\rclserv protokoll:\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.RequestTypeElements.HELP+"/>"+" ... Diese Hilfe...\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.RequestTypeElements.IN+" "+Constants.RequestValueAttrs.Login+"=\"LOGIN\"["+" "+Constants.RequestValueAttrs.Info+"=\"INFO\"]"+"/>"+" ... Mitarbeiter LOGIN kommt...\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.RequestTypeElements.OUT+" "+Constants.RequestValueAttrs.Login+"=\"LOGIN\"["+" "+Constants.RequestValueAttrs.Info+"=\"INFO\"]"+"/>"+" ... Mitarbeiter LOGIN geht...\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.ResponseTypeElements.INFO+"/>"+" ... Server Status\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.RequestTypeElements.PZE+"["+" "+clserv.shared.Constants.RequestValueAttrs.Timestamp+"=\"YYYYMMDDhhmmss\"]"+"/>"+" ... Aktueller Mitarbeiterstatus\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.RequestTypeElements.MSG+" "+Constants.RequestValueAttrs.Login+"=\"FROM\""+" "+Constants.RequestValueAttrs.To+"=\"TO\" " +Constants.RequestValueAttrs.Msg+"=\"MSG\""+"/>"+" ... chat message...\n\r");
		mConnection.sendMessageToClientImmediately("<"+clserv.shared.Constants.RequestTypeElements.TELNET+">"+" ...... Telnet Session starten\n\r");
	}

}
