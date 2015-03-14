package clserv.tools;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.Types;

import org.apache.log4j.Logger;

import clserv.clienthandler.AClientHandler;
import clserv.clienthandler.ClientHandlerFactory;


class Handler extends Thread {
	private String message = null;
	
	public Handler(String iMessage) {
		message = iMessage;
	}

	@Override
	public void run() {
		AClientHandler h = ClientHandlerFactory.getHandler(message);
		if (h != null) {
			try {
				h.handle(null, message);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}	
}

public class DBListener extends Thread {
	private final Logger logger = Logger.getLogger(this.getClass());
/*
	
	create or replace TRIGGER TRG_ES_LOGS_AI AFTER INSERT ON ES_LOGS
	FOR EACH ROW
	BEGIN
		dbms_alert.signal('ECOLOGALERT', '<'||
			case :new.entry_type
				when 'I' then 'in'
				else 'out'
			end||
			' l="'||:new.login||
	        '" i="'||nvl(:new.info,'')||
	        '" t="'||to_char(:new.entry_ts,'yyyymmddHH24MISS')||
	        '"/>'
	        );
	END;
	/
 */
	@Override
	public void run() {
		Connection mQMSDB = null;
		CallableStatement cs1 = null;
		CallableStatement cs2 = null;
		
		logger.info("Neuer DB Listener wird gestartet...");
		try {
			mQMSDB = AppContext.getInstance().getDatabase().getConnection();
			cs1 = mQMSDB.prepareCall("{call dbms_alert.register('ECOLOGALERT')}");
			cs1.execute();
//			cs2 = mQMSDB.prepareCall("{call dbms_alert.waitone('ECOLOGALERT', ?, ?)}");
			cs2 = mQMSDB.prepareCall("{call dbms_alert.waitany(?, ?, ?, 10)}");
			cs2.registerOutParameter(1, Types.VARCHAR);
			cs2.registerOutParameter(2, Types.VARCHAR);
			cs2.registerOutParameter(3, Types.INTEGER);
			logger.debug("Warte auf DB signal (ECOLOGALERT) ...");
			do {
//				logger.debug("Warte auf DB signal (ECOLOGALERT) ...");
				cs2.execute();
				if (cs2.getInt(3) == 0) {
//					String Result = cs2.getString(2);
					Handler h = new Handler(cs2.getString(2));
					h.start();
//					System.out.print(Result + "\n");// TODO: processing log
				} else {
					if (isInterrupted()) {
						logger.info("DB Listener wurde gestoppt.");
						break;
					}
				}
			} while (true);
		} catch (Exception e) {
			e.printStackTrace();
/*		} finally {
			if (cs2 != null)
				try {
					CallableStatement cstmt3 = mQMSDB.prepareCall("begin sys.dbms_alert.remove('I_GOT_IT'); end;");
					cstmt3.execute();
					cs2.close();
					if (cs1 != null) cs1.close();
					if (mQMSDB != null) AppContext.getInstance().getDatabase().releaseConnection(mQMSDB);			
				} catch (SQLException e) {
					// just catch it...
				}
*/		}
	}	
}
