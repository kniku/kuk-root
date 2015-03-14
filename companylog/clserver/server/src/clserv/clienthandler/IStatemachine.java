package clserv.clienthandler;

import clserv.model.dao.DaoES_LOGS;

abstract interface IStatemachine {
	abstract Object getParameter(String iCommand);
	abstract boolean handle_START(String iCommand);
	abstract boolean handle_ENTRYSTART(String iCommand, DaoES_LOGS iEntry);
	abstract boolean handle_ENTRY(String iCommand, DaoES_LOGS iEntry);
	abstract boolean handle_ENTRYFINISH(String iCommand, DaoES_LOGS iEntry);
	abstract boolean handle_FINISH(String iCommand);	
}
