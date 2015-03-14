package test.handler;

import junit.framework.Assert;
import mock.I_Mock_Server;
import mock.Mock_ConnectionHandler;

import org.junit.Test;

import clserv.clienthandler.NewClientHandlerFactory;
import clserv.shared.I_ClientHandler;
import clserv.shared.I_ConnectionHandler;

public class TestSQL  implements I_Mock_Server {
	private I_ConnectionHandler h_conn = new Mock_ConnectionHandler(this);
	@Test
	public void CreateAndDeleteGame() {
		String cmd = "<esm t=\"SQL\"><msg>select * from es_logs</msg></esm>";
		I_ClientHandler h = NewClientHandlerFactory.getHandler(h_conn, cmd);
		Assert.assertNotNull(h);
		if (h!= null) {
			try {
				h.handle(h_conn, cmd);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	@Override
	public boolean chatFromServer(String iUser, String iMessage) {
		return true;
	}
	@Override
	public void directFromServer(String iMessage) {
	}
}
