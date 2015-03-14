package controller;

import java.sql.Connection;
import java.sql.Date;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.util.Map;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.zkoss.zk.ui.Sessions;
import org.zkoss.zk.ui.event.Event;
import org.zkoss.zk.ui.util.GenericForwardComposer;
import org.zkoss.zul.ListModelList;
import org.zkoss.zul.Listbox;
import org.zkoss.zul.Messagebox;

import shared.AppConstants;
import shared.AppContext;
import shared.DBMSHelper;
import bo.std.types.AbstractStdObjectStructure;
import bo.std.types.AbstractStdType;
import bo.std.types.AbstractStdTypeStructure;
import controller.std.list.StdListboxUtils;

public class HistoryView extends GenericForwardComposer {
	private static final long serialVersionUID = 1L;
	private static final Log log = LogFactory.getLog(HistoryView.class);
	private Listbox lb_history;		// auto wired

	class MyType extends AbstractStdTypeStructure {

		MyType() {
			super("TEST");
		}

		String [] labels = {"Übersetzung", "Zeitpunkt", "Aktion", "Wer"};
		@Override
		public int getLastMemberIndex() {
			return labels.length - 1;
		}

		@Override
		public String getMemberLabelByIndex(int iIndex) {
			return labels[iIndex];
		}
		
	}
	
	class MyObject extends AbstractStdObjectStructure {
		MyObject(AbstractStdType mType) {
			super(mType);
		}

		String translation;
		Timestamp timestamp;
		String action;
		String who;
		
		@Override
		public String getValueStringByIndex(int iIndex) {
			String r;
			switch (iIndex) {
			case 0:
				r = translation;
				break;
			case 1:
				//r = DateFormat.getDateInstance().format(timestamp);
				r = DateFormat.getDateTimeInstance().format(timestamp);
				//r = timestamp.toString();
				break;
			case 2:
				r = action;
				break;
			case 3:
				r = who;
				break;
			default:
				r = null;
				break;
			}
			return r;
		}
	}
	
	public void onCreate(Event event) throws SQLException, InterruptedException {
		log.debug("HistoryView:onCreate");
		
		@SuppressWarnings("unchecked")
		Map<String, String> m = this.arg;
		String text = (String )m.get("text");
		if (text != null) {
			
			MyType MT = new MyType();
			StdListboxUtils.initializeStdListbox(lb_history, MT);
			
			AppContext ac = (AppContext )Sessions.getCurrent().getAttribute(AppConstants.ATTR_APPCONTEXT);
			Connection conn = DBMSHelper.getNewConnection((DataSource )ac.getDataSource());
			String query = "select translation,updkz,updip,updts from mltrans_history where textkey=? order by updts desc"; 
			PreparedStatement stmt = conn.prepareStatement(query);
//			xx = new ListModelList();
			stmt.setString(1, text);
			if (log.isDebugEnabled()) log.debug("HistoryView: stmt:[" + query + "]:[" + text + "]");
			stmt.execute();
			ResultSet rs = stmt.getResultSet();
			while (rs.next()) {
				MyObject MO = new MyObject(new MyType());
				if (log.isDebugEnabled()) log.debug("HistoryView: found: " + rs.getString(1) + " - " + rs.getString(2) + " - " + rs.getString(3) + " - " + rs.getDate(4).toString());
				MO.translation = rs.getString(1);
				MO.timestamp = rs.getTimestamp(4);
				MO.action = rs.getString(2);
				if ("C".equals(MO.action)) MO.action = "changed"; 
				else if ("D".equals(MO.action)) MO.action = "deleted"; 
				else if ("N".equals(MO.action)) MO.action = "inserted"; 
				MO.who = rs.getString(3);
				ListModelList ll =(ListModelList )lb_history.getListModel(); 
				ll.add(MO);
			}
			stmt.close();
			conn.rollback();
			conn.close();
		}
	}
}
