package clserv.model.dao;

import java.sql.Timestamp;

import clserv.model.tables.Columns;


public class DaoES_LOGS {

	private String login;
	private String info = null;
	private String entry_type = null;
	private Timestamp entry_ts;
	private String origin = Columns.StrConstrORIGIN_Std;
	private String socket_client = null;
	
	public DaoES_LOGS(String login, Timestamp entry_ts) {
		this.login = login;
		this.entry_ts = entry_ts;
	}
	public Timestamp getEntry_ts() {
		return entry_ts;
	}
	public void setEntry_ts(Timestamp entry_ts) {
		this.entry_ts = entry_ts;
	}
	public String getEntry_type() {
		return entry_type;
	}
	public void setEntry_type(String entry_type) {
		if (entry_type != null) {
			this.entry_type = entry_type.substring(0, java.lang.Math.min(clserv.model.tables.ES_LOGS.ColumnENTRY_TYPE.getLength(), entry_type.length()));
		} else {
			this.entry_type = entry_type;
		}
	}
	public String getOrigin() {
		return origin;
	}
	public void setOrigin(String iOrigin) {
		if (iOrigin != null) {
			this.origin = iOrigin.substring(0, java.lang.Math.min(clserv.model.tables.ES_LOGS.ColumnORIGIN.getLength(), iOrigin.length()));
		} else {
			this.origin = iOrigin;
		}
	}
	public String getInfo() {
		return info;
	}
	public void setInfo(String info) {
		if (info != null && info.length() > 0) {
			this.info = info.substring(0, java.lang.Math.min(clserv.model.tables.ES_LOGS.ColumnINFO.getLength(), info.length()));
		} else {
			this.info = null;
		}
	}
	public String getLogin() {
		return login;
	}
	public void setLogin(String login) {
		if (login != null) {
			this.login = login.substring(0, java.lang.Math.min(clserv.model.tables.ES_LOGS.ColumnLOGIN.getLength(), login.length()));
		} else {
			this.login = login;
		}
	}

	public String toString() {
		String r = super.toString();
		if (r != null) {
			r = r+" ["+getLogin()+":"+getEntry_type()+":"+getEntry_ts().toString()+":"+getInfo()+
					":"+getOrigin()+":"+getSocket_client()+"]";
		}
		return r;
	}
	public void setSocket_client(String socket_client) {
		this.socket_client = socket_client;
	}
	public String getSocket_client() {
		return socket_client;
	}

}
