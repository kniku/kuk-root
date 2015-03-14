package clserv.model.dao;

import java.sql.Timestamp;

public class DaoES_PRELOGS {
	
	// **********************
	// structure
	// **********************
	private String login;
	private String info = null;
	private String entry_type = null;
	private Timestamp entry_ts;
	private String socket_client = null;

	// **********************
	// constructor
	// **********************
	public DaoES_PRELOGS(String login, Timestamp entry_ts) {
		this.login = login;
		this.entry_ts = entry_ts;
	}

	// **********************
	// setter
	// **********************
	public void setLogin(String login) {
		this.login = login;
	}

	public void setInfo(String info) {
		this.info = info;
	}

	public void setEntry_type(String entry_type) {
		this.entry_type = entry_type;
	}

	public void setEntry_ts(Timestamp entry_ts) {
		this.entry_ts = entry_ts;
	}
	
	// **********************
	// getter
	// **********************
	public String getLogin() {
		return login;
	}
	public String getInfo() {
		return info;
	}
	public String getEntry_type() {
		return entry_type;
	}
	public Timestamp getEntry_ts() {
		return entry_ts;
	}

	public void setSocket_client(String socket_client) {
		this.socket_client = socket_client;
	}

	public String getSocket_client() {
		return socket_client;
	}
}
