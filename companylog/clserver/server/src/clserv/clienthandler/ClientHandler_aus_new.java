package clserv.clienthandler;

import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;

import clserv.model.dao.DaoES_LOGS;
import clserv.shared.I_ConnectionHandler;


public class ClientHandler_aus_new extends AClientHandler_aus_base {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	@Override
	public Parameter getParameter(String iCommand) {
		Parameter r = new Parameter();
		String [] Parts = iCommand.split(":");
		r.mSearchPattern = Parts.length > 1 ? Parts[1] : null;
		if (r.mSearchPattern != null) {
			if (r.mSearchPattern.equals("*") || r.mSearchPattern.equals("")) r.mSearchPattern = null;
		}
		
		int year = Parts.length > 2 ? Integer.parseInt(Parts[2]) : 0;
		int month = Parts.length > 3 ? Integer.parseInt(Parts[3]) : 0;
		if (month > 0) month--;
		int day = Parts.length > 4 ? Integer.parseInt(Parts[4]) : 0;
		GregorianCalendar cal = new GregorianCalendar();
		if (day > 0) {
			cal.set(year, month, day, 0, 0, 0);
			cal.set(Calendar.MILLISECOND, 0);
			r.mDateFrom = new Timestamp(cal.getTimeInMillis());
			cal.set(year, month, day, 23, 59, 59);
			cal.set(Calendar.MILLISECOND, 999);
			r.mDateTo = new Timestamp(cal.getTimeInMillis());
		} else if (month > 0) {
			cal.set(year, month, 1, 0, 0, 0);
			cal.set(Calendar.MILLISECOND, 0);
			r.mDateFrom = new Timestamp(cal.getTimeInMillis());
			cal.set(year, month, cal.getActualMaximum(Calendar.DAY_OF_MONTH), 23, 59, 59);
			cal.set(Calendar.MILLISECOND, 999);
			r.mDateTo = new Timestamp(cal.getTimeInMillis());
		} else if (year > 0) {
			cal.set(year, 0, 1, 0, 0, 0);
			cal.set(Calendar.MILLISECOND, 0);
			r.mDateFrom = new Timestamp(cal.getTimeInMillis());
			cal.set(year, 11, 31, 23, 59, 59);
			cal.set(Calendar.MILLISECOND, 999);
			r.mDateTo = new Timestamp(cal.getTimeInMillis());
		}
		return r;
	}

	@Override
	public boolean handle_ENTRY(String command, DaoES_LOGS iEntry) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeData(iEntry));
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
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeTail());
		return true;
	}

	@Override
	public boolean handle_START(String command) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeHead());
		return true;
	}

}
