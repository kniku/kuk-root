package clserv.tools;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import org.apache.log4j.Logger;

public class UserClientIndex {
	private final Logger logger = Logger.getLogger(this.getClass());
	private String mDataDir = null;
	
	private class UserClientEntry {
		public HashMap<String, String> mEntryHash = new HashMap<String, String>();
	}
	
	private HashMap<String, UserClientEntry> mUserHash = new HashMap<String, UserClientEntry>();

	
	public UserClientIndex(String dataDir) {
		mDataDir = dataDir;
		File f = new File(dataDir);
		String [] lFiles = f.list();
		for (String aFile : lFiles) {
			String [] uce = aFile.split("#");
			if (uce.length == 3) {
				insertUserClient(uce[0], uce[1], uce[2], false);
			}
		}
	}

	public void insertUserClient(String iLogin, String iHost, String iPort, boolean iPersist) {
		UserClientEntry uce = null;
		
		if (iLogin == null || iHost == null || iPort == null) return;
		
		if (logger.isDebugEnabled()) {
			logger.debug("registering client: ["+iLogin+"->"+iHost+":"+iPort+"]");
		}
		
		uce = mUserHash.get(iLogin);
		if (uce == null) {
			uce = new UserClientEntry();
			mUserHash.put(iLogin, uce);
		}
		
		if (uce == null) return;

		if (iPersist) {
			try {
				FileWriter writer = new FileWriter(mDataDir+"/"+iLogin+"#"+iHost+"#"+iPort, false);
				writer.close();
			} catch (IOException e) {
				logger.fatal("error on persisting client registration!", e);
			}
		}
		
		iHost = iHost + ":" + iPort;
		if (uce.mEntryHash.get(iHost) == null) {
			uce.mEntryHash.put(iHost, "1");
		}
	}
	
	public void removeUserClient(String iLogin, String iHost, String iPort) {
		if (logger.isDebugEnabled()) {
			logger.debug("unregistering client: ["+iLogin+"->"+iHost+":"+iPort+"]");
		}
		File f = new File(mDataDir+"/"+iLogin+"#"+iHost+"#"+iPort);
		if (f.exists() == true) f.delete();
		if (f.exists() == true) {
			logger.fatal("error on removing client registration of ["+mDataDir+"/"+iLogin+"#"+iHost+"#"+iPort+"]");				
		}
		UserClientEntry uce = mUserHash.get(iLogin);
		if (uce != null) {
			iHost = iHost + ":" + iPort;
			if (uce.mEntryHash.get(iHost) != null) {
				uce.mEntryHash.remove(iHost);
			}
		}
	}
	public void removeUserClient(String iLogin, String iHostPort) {
		String [] host_port = iHostPort.split(":");
		if (host_port.length > 1) {
			removeUserClient(iLogin, host_port[0], host_port[1]);
		}
	}
	
	public Iterator<String> getClientUserIter() {
		return mUserHash.keySet().iterator();
	}

	public Iterator<String> getClientEntryIter(String iLogin) {
		Iterator <String> r = null;
		if (mUserHash.get(iLogin) != null) {
			return mUserHash.get(iLogin).mEntryHash.keySet().iterator();
		}
		return r;
	}

	public int getClientEntryCount() {
		int r = 0;
		for (Iterator<String> iterator = mUserHash.keySet().iterator(); iterator.hasNext();) {
			r += mUserHash.get(iterator.next()).mEntryHash.size();
		}
		return r;
	}

	public void checkAllClientEntries(String [] iUserFilter, ArrayList<String> oUsersOnline, ArrayList<String> oUsersOffline) {
		Object [] login_arr, entry_arr;
		synchronized (this) {
			login_arr = mUserHash.keySet().toArray();
		}
		for (int i = 0; i < login_arr.length; i++) {
			String login = (String )login_arr[i];
			synchronized (this) {
				entry_arr = mUserHash.get(login).mEntryHash.keySet().toArray();
			}
			for (int ii = 0; ii < entry_arr.length; ii++) {
				String e = (String )entry_arr[ii];
				boolean do_check = true;
				if (iUserFilter != null) {
					do_check = false;
					for (int j = 0; j < iUserFilter.length; j++) {
						if (iUserFilter[j].equals(login)) {
							do_check = true;
							break;
						}
					}
				}
				if (do_check == false) continue;
				
				if (logger.isDebugEnabled()) {
					logger.debug("client check for: ["+login+"->"+e+"]...");
				}
				if (checkEntry(e) == false) {
					if (logger.isDebugEnabled()) {
						logger.debug("check failed, removing entry.");
					}
					synchronized (this) {
						removeUserClient(login, e);
					}
					if (oUsersOffline != null) {
						if (oUsersOffline.contains(login) == false) {
							oUsersOffline.add(login);
						}
					}
				} else {
					if (logger.isDebugEnabled()) {
						logger.debug("check passed.");
					}
					if (oUsersOnline != null) {
						if (oUsersOnline.contains(login) == false) {
							oUsersOnline.add(login);
						}
					}
				}
			}
		}		
	}

	private boolean checkEntry(String iHostPort) {
		boolean r = false;
		String [] host_port = iHostPort.split(":");
		if (host_port.length > 1) {
			r = checkEntry(host_port[0], host_port[1]);
		}
		return r;
	}

	private boolean checkEntry(String iHost, String iPort) {
		boolean r = false;
		try {
			Socket s = new Socket(iHost, Integer.parseInt(iPort));
			s.close();
			r = true;
		} catch (Exception e) {
			// just catch it ...
		}
		return r;
	}
}
