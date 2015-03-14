package clserv.clienthandler;

import java.io.IOException;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.GregorianCalendar;

import org.apache.log4j.Logger;

import clserv.model.dao.DaoES_LOGS;
import clserv.model.ddl.DDLTools;
import clserv.model.tables.ES_LOGS;
import clserv.tools.AppContext;


public abstract class AClientHandler_aus_base extends AClientHandler implements IStatemachine {
	private final Logger logger = Logger.getLogger(this.getClass());

	protected class Parameter {
		String mSearchPattern = null;
		Timestamp mDateFrom = null; 
		Timestamp mDateTo = null; 
	}
	
	public void handle(String iCommand) throws IOException {
		boolean hasWildcards = false;

		Parameter myParams = (Parameter )getParameter(iCommand);
		if (myParams == null) return;
		
		if (myParams.mSearchPattern != null) {
			if (myParams.mSearchPattern.indexOf("*") >= 0) {
				hasWildcards = true;
				myParams.mSearchPattern = myParams.mSearchPattern.replaceAll("\\*", "%");
				myParams.mSearchPattern = myParams.mSearchPattern.replaceAll("\\?", "_");
			}
		}
		
		if (myParams.mSearchPattern != null) {
			myParams.mSearchPattern = myParams.mSearchPattern.toUpperCase();
			if (hasWildcards) {
				myParams.mSearchPattern = " and upper(qmdb10.qmpernname) like '"+myParams.mSearchPattern+"' ";
			} else {
				myParams.mSearchPattern = " and (upper(qmdb10.qmperkubez)='"+myParams.mSearchPattern+"' or upper(qmdb10.qmpernname) = '"+myParams.mSearchPattern+"') ";				
			}
		} else {
			myParams.mSearchPattern = "";
		}

		if (myParams.mDateFrom == null || myParams.mDateTo == null) {
			GregorianCalendar cal = new GregorianCalendar();
			if (myParams.mDateFrom == null) {
				// filter = heute
				cal.set(Calendar.HOUR_OF_DAY, 0);
				cal.set(Calendar.MINUTE, 0);
				cal.set(Calendar.SECOND, 0);
				cal.set(Calendar.MILLISECOND, 0);
				myParams.mDateFrom = new Timestamp(cal.getTimeInMillis());
			}
			if (myParams.mDateTo == null) {
				cal.set(Calendar.HOUR_OF_DAY, 23);
				cal.set(Calendar.MINUTE, 59);
				cal.set(Calendar.SECOND, 59);
				cal.set(Calendar.MILLISECOND, 999);
				myParams.mDateTo = new Timestamp(cal.getTimeInMillis());
			}
		}
		
		try {
			Connection c = AppContext.getInstance().getDatabase().getConnection();
			String s = "select "+DDLTools.DDL_HELPER_create_columnlist(ES_LOGS.getInstance(), null)+
	    	" from "+clserv.model.tables.ES_LOGS.getInstance().getName()+",qmdb10"+
	    	" where qmdb10.QMMANNUMMR='01' and qmdb10.qmperkubez="+clserv.model.tables.ES_LOGS.ColumnLOGIN.getName(null)+
	    	myParams.mSearchPattern+
	    	" and "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName(null)+">=? and "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName(null)+"<=?"+
	    	" order by "+clserv.model.tables.ES_LOGS.ColumnENTRY_TS.getName(null);
			PreparedStatement stmt = c.prepareStatement(s);
			stmt.setTimestamp(1, myParams.mDateFrom);
			stmt.setTimestamp(2, myParams.mDateTo);
			logger.trace("Statement to execute=["+s+"]");
			logger.trace("Parameter 1="+myParams.mDateFrom.toString());
			logger.trace("Parameter 2="+myParams.mDateTo.toString());
			stmt.execute();
			ResultSet rs = stmt.getResultSet();
			int count = 0;
			handle_START(iCommand);
			DaoES_LOGS Log = null;
			for (; rs.next() ;count++) {
				if (Log == null) {
					Log = new DaoES_LOGS(rs.getString(ES_LOGS.ColumnLOGIN.getName(null)), rs.getTimestamp(ES_LOGS.ColumnENTRY_TS.getName(null)));
				} else {
					Log.setLogin(rs.getString(ES_LOGS.ColumnLOGIN.getName(null)));
					Log.setEntry_ts(rs.getTimestamp(ES_LOGS.ColumnENTRY_TS.getName(null)));
				}
				Log.setEntry_type(rs.getString(ES_LOGS.ColumnENTRY_TYPE.getName(null)));
				Log.setInfo(rs.getString(ES_LOGS.ColumnINFO.getName(null)));
				logger.trace(Log.toString());
				handle_ENTRY(iCommand, Log);
			}
			handle_FINISH(iCommand);
			stmt.close();
			rs.close();
			c.rollback();
			AppContext.getInstance().getDatabase().releaseConnection(c);
		} catch (Exception e) {
			logger.error("Logliste konnte nicht geladen werden!", e);
		}

	}

}
