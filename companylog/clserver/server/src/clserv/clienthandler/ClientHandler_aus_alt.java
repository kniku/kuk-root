package clserv.clienthandler;

import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;

import clserv.model.dao.DaoES_LOGS;
import clserv.shared.I_ConnectionHandler;


public class ClientHandler_aus_alt extends AClientHandler_aus_base {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}

	@Override
	public Parameter getParameter(String iCommand) {
		Parameter r = new Parameter();
		String datePattern = iCommand.substring(41);
		int index, year = 0, month = 0, day = 0;
		if ((index = datePattern.indexOf(".")) > 0) {
			datePattern = datePattern.substring(0, index);
		}
		GregorianCalendar cal = new GregorianCalendar();
		
		if (datePattern.length() < 5) year = cal.get(GregorianCalendar.YEAR);
		else year = Integer.parseInt(datePattern.substring(0, 4));
		
		if (datePattern.length() > 6) month = Integer.parseInt(datePattern.substring(4, 6)) - 1;

		if (datePattern.length() > 7) day = Integer.parseInt(datePattern.substring(6, 8));
		
		if (month == 0) {
			cal.set(year, 0, 1, 0, 0, 0);
			cal.set(Calendar.MILLISECOND, 0);
			r.mDateFrom = new Timestamp(cal.getTimeInMillis());
			cal.set(year, 11, 31, 23, 59, 59);
			cal.set(Calendar.MILLISECOND, 999);
			r.mDateTo = new Timestamp(cal.getTimeInMillis());
		} else if (day == 0) {
			cal.set(year, month, 1, 0, 0, 0);
			cal.set(Calendar.MILLISECOND, 0);
			r.mDateFrom = new Timestamp(cal.getTimeInMillis());			
			cal.set(year, month, cal.getActualMaximum(Calendar.DAY_OF_MONTH), 23, 59, 59);
			cal.set(Calendar.MILLISECOND, 999);
			r.mDateTo = new Timestamp(cal.getTimeInMillis());
		} else {
			cal.set(year, month, day, 0, 0, 0);
			cal.set(Calendar.MILLISECOND, 0);
			r.mDateFrom = new Timestamp(cal.getTimeInMillis());			
			cal.set(year, month, day, 23, 59, 59);
			cal.set(Calendar.MILLISECOND, 999);
			r.mDateTo = new Timestamp(cal.getTimeInMillis());
		}
		return r;
	}

	@Override
	public boolean handle_ENTRY(String command, DaoES_LOGS iEntry) {
		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeData_neu(iEntry));
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
//		mConnection.sendMessageToClientImmediately(ClientHandlerFactory.createResponse_pzeData(null));
		return true;
	}

	@Override
	public boolean handle_START(String command) {
		mConnection.sendMessageToClientImmediately("AUA:\n");
		return true;
	}

}
