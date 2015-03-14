package clserv.clienthandler;

import java.text.DecimalFormat;

import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;
import clserv.tools.AppContext;


public class NewClientHandler_info implements I_ClientHandler /*extends AClientHandler*/ {

	@Override
	public void handle(I_ConnectionHandler iConn, String command) throws Exception {
		iConn.sendMessageToClientImmediately(iConn.envelopClientMessage("info", createResponse_info(),null));
		return;
	}

	public String createResponse_info() {
		AppContext ac = AppContext.getInstance();
		String r = "version=\"" + clserv.server.getVersionString();
		r += "\" name=\""+ac.getMServerName();
		r += "\" requests=\""+ac.getCntConnections();
		r += "\" threads=\""+Thread.activeCount();
		r += "\" maxthreads=\""+ac.getMaxThreadsAllTime();
		r += "\" uptime=\""+new DecimalFormat("0.00").format(ac.getUptime()/1000/60/60F)+"\"";
		return r;
	}
	
}
