package plugins;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.sql.Connection;

import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Plugin;



public class UPNR_v1 implements I_Plugin {

	long m_upnr = 0;
	final String UPNR_FILE = "data/plugins/UPNR/upnr.dat";
	
	@Override
	public String getDescription() {
		return "UPNR Plugin";
	}

	@Override
	public String getVersion() {
		return "1.0.0";
	}

	@Override
	public void onNewInstance() {
		BufferedReader InputStream = null;
		try {
			InputStream = new BufferedReader(new FileReader(UPNR_FILE));
			String line = InputStream.readLine();
			InputStream.close();
			m_upnr = Long.parseLong(line);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public boolean isDatabaseNeeded() {
		return false;
	}
	
	@Override
	public void handle(I_ConnectionHandler iClientConnection, Connection iDatabaseConnection, String iCommand) {
		FileWriter writer;
		m_upnr++;
		try {
			writer = new FileWriter(UPNR_FILE, false);
			writer.write(Long.toString(m_upnr));
			writer.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		iClientConnection.sendMessageToClientImmediately(iClientConnection.envelopClientMessage("upnr", null, Long.toString(m_upnr)));
	}
}
