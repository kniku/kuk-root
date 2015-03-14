package clserv.clienthandler;

import java.sql.Timestamp;

import org.apache.log4j.Logger;

import clserv.model.dao.DaoES_LOGS;
import clserv.shared.I_ConnectionHandler;
import clserv.tools.AppContext;



public class ClientHandler_pze_alt extends AClientHandler_pze_base {
	private final Logger logger = Logger.getLogger(this.getClass());
	
	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	@Override
	public Object getParameter(String command) {
		//es3.5:PZE:kuk                           :0000000000
		Parameter r = new Parameter();
		String x = command.substring(41, 51);
		long l = Integer.parseInt(x);
		if (l != 0)	{
			r.mLastRequest = new Timestamp(l * 1000);
		}
		if (logger.isTraceEnabled() && r.mLastRequest != null) logger.trace("RECV: ["+x+"]->["+r.mLastRequest.toString()+"]");
		return r;
	}

	@Override
	public boolean handle_ENTRY(String command, DaoES_LOGS iEntry) {
		String s;
		s = ClientHandlerFactory.createResponse_pzeData_neu(iEntry);
		mConnection.sendMessageToClientImmediately(s);
		return true;
	}

	@Override
	public boolean handle_ENTRYFINISH(String command, DaoES_LOGS entry) {
		return true;
	}

	@Override
	public boolean handle_ENTRYSTART(String command, DaoES_LOGS entry) {
		return true;
	}

	@Override
	public boolean handle_FINISH(String command) {
		return true;
	}

	@Override
	public boolean handle_START(String iCommand) {
		String s;
		boolean r = true;
		// TODO: richtiger last_request...
		long last_request = Integer.parseInt(iCommand.substring(41).trim());
		if (last_request == 0 || AppContext.getInstance().getLastLogEntry() == 0 || last_request < AppContext.getInstance().getLastLogEntry() / 1000) {
			// s.Format("PZA:%010lu\n",lastt);				
			s = String.format("PZA:%010d\n",new Object[]{AppContext.getInstance().getLastLogEntry() / 1000});
		} else {
			// nothing changed: s="PZA:----------";				
			s = "PZA:----------";
			r = false;
		}
		if (logger.isTraceEnabled()) logger.trace("SEND: ["+s+"]");
		mConnection.sendMessageToClientImmediately(s);
		return r;
	}
}
