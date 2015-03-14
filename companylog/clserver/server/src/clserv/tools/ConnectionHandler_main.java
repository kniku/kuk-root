package clserv.tools;

import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Iterator;

import org.apache.log4j.Logger;

import clserv.clienthandler.NewClientHandlerFactory;
import clserv.shared.I_ClientHandler;
import clserv.shared.I_EmployeeManager;


public class ConnectionHandler_main extends AConnectionHandler {
	private final Logger logger = Logger.getLogger(this.getClass());

	
/*	public ConnectionHandler_main(Socket iConn) {
		super(iConn);
	}
*/	public ConnectionHandler_main() {
		super();
	}

	public void run_handler() throws Exception {
		String Command = getClientCommand();
		if (Command != null) {
			Command=Command.replace("\r", "");
			if (Command.endsWith("\n")) {
				Command=Command.substring(0,Command.length()-1);
			}
			logger.info("MESSAGE:["+Command+"]");
			I_ClientHandler h = NewClientHandlerFactory.getHandler(this, Command);
			if (h != null) {
				int fi = Command.indexOf(">");
				int li = Command.lastIndexOf("<");
				if (fi < li) Command = Command.substring(fi + 1, li);
				else Command = null;
				//Command=Command.replace("\n", "");
				//Command=Command.replace(" ", "");
				//Command=Command.trim();
				h.handle(this, Command);
			} else {
				sendMessageToClientImmediately(createResponse_error("unknown/invalid handler type"));
			}
		} else {
			logger.debug("MESSAGE: war wohl nur ein PING...");
		}
	}

	@Override
	public String getClientCommand() {
		String r = null;
		ByteBuffer buffer = ByteBuffer.allocate(2000);
		ByteBuffer cmdbuffer = ByteBuffer.allocate(2000);
		byte [] bb = buffer.array();
		try {
			String tmp = null;
			int cnt;
			while ((cnt = mSocket.getInputStream().read(bb)) >= 0) {
				if (cmdbuffer.remaining() > bb.length) {
					throw new IOException("input buffer size exceeded!");
				}
				for (int i = 0; i < cnt; i++) {
					cmdbuffer.put(bb[i]);
				}
				tmp = new String(cmdbuffer.array(), 0, cmdbuffer.position(), "ISO-8859-1");
				if (!tmp.startsWith("<esm") || tmp.contains("</esm>") || tmp.endsWith("/>") || tmp.endsWith("/>\n")) break;
			}
			if (tmp != null && tmp.startsWith("<esm")) {
				r = tmp;
			}
		} catch (IOException e) {
			logger.error(null, e);
		}
		if (r == null) {
			sendMessageToClientImmediately(createResponse_error("unknown/invalid message"));
		}
		return r;
	}
	public String envelopClientMessage(String iType, String iAttrs, String iMessage) {
		if (iType == null) return null;
		if (iAttrs == null && iMessage == null) {
			return "<ecm t=\""+iType+"\"/>";
		}
		if (iMessage == null) {
			return "<ecm t=\""+iType+"\"><msg "+iAttrs+"/></ecm>";
		}
		if (iAttrs == null) {
			return "<ecm t=\""+iType+"\"><msg>"+iMessage+"</msg></ecm>";
		}
		return "<ecm t=\""+iType+"\"><msg "+iAttrs+">"+iMessage+"</msg></ecm>";
	}
	
	public String createResponse_error(String iMessage) {
		//if (iMessage != null) iMessage = "m=\"" + iMessage + "\"";
		return envelopClientMessage("error", null, iMessage);
	}
	public String createResponse_ok(String iMessage) {
		//if (iMessage != null) iMessage = "m=\"" + iMessage + "\"";
		return envelopClientMessage("ok", null, iMessage);
	}

	public String getHandlerName() {
		return "main";
	}

	private boolean sendMessage(String iTo, String iMsg, String iHost, String iPort) {
		boolean r = false;
		try {
			if (logger.isDebugEnabled()) {
				logger.debug("msg: trying client ["+iTo+"->"+iHost+":"+iPort+"]...");
			}
			Socket s = new Socket(iHost, Integer.parseInt(iPort));
			//s.setSoTimeout(10000);	// timeout von 10s
			if (s != null) {
				if (logger.isDebugEnabled()) {
					logger.debug("msg: sending ["+iMsg+"]...");
				}
	//			s.getOutputStream().write(iMsg.getBytes("ISO-8859-1"));
				s.getOutputStream().write(iMsg.getBytes());
				s.close();
				r = true;
			}
		} catch (Exception e) {
			// just catch it...
		}
		return r;
	}
	
	@Override
	public boolean sendMessageToUserImmediately(String iLogin, String iMessage) {
		boolean r = false;
		ArrayList<String> lReceiver = clserv.tools.AppContext.getInstance().getClientEntryList(iLogin);
		for (Iterator<String> iterator = lReceiver.iterator(); iterator.hasNext();) {
			String [] host_port = iterator.next().split(":");
			if (sendMessage(iLogin, iMessage, host_port[0], host_port[1]) == false) {
				if (logger.isDebugEnabled()) {
					logger.debug("msg: invalid client ["+iLogin+"->"+host_port[0]+":"+host_port[1]+"], canceling registration");
				}
				clserv.tools.AppContext.getInstance().removeUserClientEntry(iLogin, host_port[0], host_port[1]);
			} else {
				r = true;
			}
		}
		
		return r;
	}

	@Override
	public I_EmployeeManager getEmployeeManager() {
		return clserv.tools.AppContext.getInstance();
	}

}
