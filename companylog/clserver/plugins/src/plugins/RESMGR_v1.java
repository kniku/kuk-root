package plugins;

import java.sql.Connection;

import clserv.shared.I_ConnectionHandler;
import clserv.shared.I_Plugin;

import plugins.RESMGR.Resource;
import plugins.RESMGR.ResourceManager;

public class RESMGR_v1 implements I_Plugin {

	@Override
	public String getDescription() {
		return "Resource Manager";
	}

	@Override
	public String getVersion() {
		return "1.0";
	}

	// shell: es.<esm t="RESMGR_v1"><msg t="get" l="login" name="xxx"/></esm>
	// shell: es.<esm t="RESMGR_v1"><msg t="release" l="login" name="xxx" value="yyy"/></esm>
	// shell: es.<esm t="RESMGR_v1"><msg t="status" name="xxx"/></esm>
	// shell: es.<esm t="RESMGR_v1"><msg t="reload"/></esm>
	// shell: es.<esm t="RESMGR_v1"><msg t="list"/></esm>
	
	@Override
	public void handle(I_ConnectionHandler iClientConnection, Connection iDatabaseConnection, String iCommand) {
		boolean success = false;
		plugins.RESMGR.Parameter p = new plugins.RESMGR.Parameter();
		p.parseCommand(iCommand);
		Resource aRes = null;
		String aStr = null;
		
		if (p.m_RequestType != null) {
			if (p.m_RequestType.equals("reload")) {
				ResourceManager.getInstance().reloadResources();
				iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));
				success = true;
			} else if (p.m_RequestType.equals("list")) {
				Object [] reslist = ResourceManager.getInstance().getListOfResources();
				aStr = "";
				if (reslist != null) {
					for (int i = 0; i < reslist.length; i++) {
						aRes = ResourceManager.getInstance().getResource((String )reslist[i]);
						if (aRes != null) {
							aStr += "\n<Resource name=\"" + aRes.m_ResourceName + "\" type=\"" + aRes.m_ResourceType + "\"/>";
						}
					}
					aStr += "\n";
				}
				iClientConnection.sendMessageToClientImmediately(iClientConnection.envelopClientMessage("RESMGR", "t=\"list\"", aStr));
				success = true;
			} else if (p.m_ResourceName != null) {
				aRes = ResourceManager.getInstance().getResource(p.m_ResourceName);
				if (aRes != null) {
					if (p.m_RequestType.equals("get")) {
						aStr = aRes.getNextValue(p.m_Login);
						if (aStr != null) {
							iClientConnection.sendMessageToClientImmediately(iClientConnection.envelopClientMessage("RESMGR", "t=\"get\"", aStr));
						} else {
							iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error("internal error"));						
						}
						success = true;
					} else if (p.m_RequestType.equals("release")) {
						if (aRes.releaseValue(p.m_Login, p.m_ResourceValue)) {
							iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_ok(null));						
						} else {
							iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error("internal error"));						
						}
						success = true;
					} else if (p.m_RequestType.equals("status")) {
						aStr = aRes.getStatus();
						iClientConnection.sendMessageToClientImmediately(iClientConnection.envelopClientMessage("RESMGR", "t=\"status\" name=\""+aRes.m_ResourceName+"\"", aStr));
						success = true;
					}
				} else {
					iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error("unknown resource"));					
					success = true;
				}
			}
		}		
		if (!success) {
			iClientConnection.sendMessageToClientImmediately(iClientConnection.createResponse_error("invalid request"));
		}
	}

	@Override
	public boolean isDatabaseNeeded() {
		return false;
	}

	@Override
	public void onNewInstance() {
		ResourceManager.getInstance();	// initialize res manager 
	}

}
