package clserv.clienthandler;

import java.io.IOException;

import clserv.shared.I_ConnectionHandler;


public class ClientHandler_out_alt extends AClientHandler_es_logger {

	@Override
	public void handle(I_ConnectionHandler connection, String command) throws Exception {
		// TODO: change handle method to new interface
	}
	public void handle(String iCommand) throws IOException {
		String info = null;
		if (iCommand.length() > 45) {
			info = iCommand.substring(45);
			info = info.trim();
			if (info.length() > 0) {
				info = " " + clserv.shared.Constants.RequestValueAttrs.Info + "=\"" + info + "\"";
			} else info = null;
		}
		iCommand = "<" + clserv.shared.Constants.RequestTypeElements.OUT + " " + clserv.shared.Constants.RequestValueAttrs.Login + "=\""+iCommand.substring(10,13)+"\""+(info == null ? "" : info) + " />";
		handle(iCommand, clserv.model.tables.Columns.StrConstrENTRY_TYPE_Out);
	}

}
