package clserv.clienthandler;

import java.nio.ByteBuffer;

import org.apache.log4j.Logger;

import clserv.shared.I_ConnectionHandler;
import clserv.tools.AppContext;


public class ClientHandler_telnet extends AClientHandler {
	private final Logger logger = Logger.getLogger(this.getClass());

	public void handle(String iCommand) throws Exception {
		ByteBuffer cmdbuffer = ByteBuffer.allocate(2000);
		ByteBuffer buffer = ByteBuffer.allocate(2000);
		String Message;
		int i, cnt;
		byte[] br = buffer.array();
		AppContext ac = AppContext.getInstance();
		
		mConnection.sendMessageToClientImmediately("\r\n<id clserv=\""+clserv.server.getVersionString()+"\"/>");
		mConnection.sendMessageToClientImmediately("\r\n\r\n>>");
		while ((cnt = mConnection.getSocket().getInputStream().read(br)) >= 0) {
			for (i = 0; i < cnt; i++) {
				if (br[i] == '\r') {
					continue; // skip
				}
				if (br[i] == '\n') {
					Message = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");
					logger.debug("telnet command ("
							+ mConnection.getSocket().getInetAddress() + "): ["
							+ Message + "]");
					cmdbuffer.clear();
					if (Message.equals("quit")) {
						mConnection.sendMessageToClientImmediately("\r\n<clserv> bye!\r\n");
						return;
					}
					if (Message.equals("stop")) {
						mConnection.sendMessageToClientImmediately("\r\n<clserv> i'm killed, bye!\r\n");
						mConnection.getSocket().close();
						logger.info("server stopped manually!");
						//ac.shutdown(); -- done by shutdown hook ...
						System.exit(0);
						return;
					}
					if (Message.equals("status")) {
						Message = ClientHandlerFactory.createResponse_Statusmeldung(ac);
						mConnection.sendMessageToClientImmediately(Message);
						continue;
					}
					if (Message.equals("rehash")) {
						ac.reset_all();
						mConnection.sendMessageToClientImmediately("\r\ndone.\r\n\r\n>>");
						continue;
					}
					if (Message.equals("help")) {
						mConnection.sendMessageToClientImmediately("\r\n<clserv> valid commands are:\r\n"
										+ "help ............ this help\r\n"
										+ "status .......... status information\r\n"
										+ "rehash .......... reload hashes (qms, ...)\r\n"
										+ "stop ............ stop server!!!\r\n"
										+ "quit ............ exit session\r\n"
										+ "\r\n>>");
						continue;
					}
					mConnection.sendMessageToClientImmediately("\r\n<clserv> unknown command: ["
									+ Message
									+ "]\r\ntry 'help' for more information\r\n\r\n>>");
				} else {
					cmdbuffer.put(br[i]);
				}
			}
		}
	}

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

}
