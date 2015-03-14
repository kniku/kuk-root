package clserv.clienthandler;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Iterator;

import org.apache.log4j.Logger;

import clserv.model.Mitarbeiter;
import clserv.model.dao.DaoES_LOGS;
import clserv.shared.Helper;
import clserv.tools.AppContext;




public final class ClientHandlerFactory {
	private static Logger logger = Logger.getLogger(ClientHandlerFactory.class);
	private static SimpleDateFormat dateformat = new SimpleDateFormat("yyyyMMddHHmmss");

	public static AClientHandler getHandler(String iCommand) {
		AClientHandler r = null;
		AppContext ac = AppContext.getInstance();
		if (iCommand != null) {
			if (iCommand.equals("<" + clserv.shared.Constants.RequestTypeElements.TELNET + ">")) {
				// command: <*>
				r = new ClientHandler_telnet();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.TELNET);
				logger.info("handler=[telnet]");
			} else if (iCommand.equals("<" + clserv.shared.Constants.ResponseTypeElements.INFO+"/>")) {
				// command: <info/>
				r = new ClientHandler_info();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.INFO);
				logger.info("handler=[info]");
			} else if (iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.IN+" ")) {
				if (ac.getServerMode() == 1 && iCommand.indexOf("t=\"") < 0) {
					logger.trace("handler=[in], handler disabled (slave mode)!"+iCommand);
					return null;	// keine normalen logs im reinen slave modus!
				}
				// command: <in l="LOGIN"[ i="INFO"][ pre="1"]/>
				// example: <in l="kuk"/>
				// example: <in l="kuk" i="INFO" pre="1"/>
				r = new ClientHandler_in();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.INOUT);
				logger.info("handler=[in]");
			} else if (iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.OUT+" ")) {
				if (ac.getServerMode() == 1 && iCommand.indexOf("t=\"") < 0) {
					logger.trace("handler=[out], handler disabled (slave mode)!");
					return null;	// keine normalen logs im reinen slave modus!
				}
				// command: <out l="LOGIN"[ i="INFO"][ pre="1"] />
				r = new ClientHandler_out();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.INOUT);
				logger.info("handler=[out]");
			} else if (iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.REG+" ") ||
					   iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.UNREG+" ")) {
				// command: <[un]reg l="LOGIN" port="1235" />
				r = new ClientHandler_reg();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.REG);
				logger.info("handler=[reg]");
			} else if (iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.MSG+" ")) {
				// command: <reg l="LOGIN" port="1235" />
				r = new ClientHandler_msg();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.MSG);
				logger.info("handler=[reg]");
			} else if (iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.PZE)) {
				// command: <pze[:t=YYYYMMDDhhmmss]>
				r = new ClientHandler_pze();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.PZE);
				logger.info("handler=[pze]");
			} else if (iCommand.startsWith(clserv.shared.Constants.RequestTypeElements.AUS+":")) {
				// command: <aus:p=name_pattern[:y=jahr][:m=monat][:d=tag]>
				// anm.: <aus:p=*> entspricht <pze> allerdings ohne optimierung durch timestamp!
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.AUS);
				r = new ClientHandler_aus_new();
				logger.info("handler=[aus]");
			} else if (iCommand.startsWith(clserv.shared.Constants.RequestTypeElements.QMI+":")) {
				// command: <qmi:p=PATTERN:i=T/f:e=T/f:n=t/F:k=t/F:f=t/F>
				r = new ClientHandler_qmi();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.QMI);
				logger.info("handler=[qmi]");
			} else if (iCommand.startsWith("<"+clserv.shared.Constants.RequestTypeElements.HELP)) {
				// command: <help/>
				r = new ClientHandler_help();
				ac.notifyAboutRequest(clserv.shared.Constants.RequestType.HELP);
				logger.info("handler=[help]");				
			} else if (iCommand.startsWith("es3.5:")) {
				// altes protokoll!
				if (iCommand.substring(5, 10).equals(":ENT:")) {
					if (ac.getServerMode() == 1) {
						logger.trace("handler=[ENT], handler disabled (slave mode)!");				
						return null;	// keine normalen logs im reinen slave modus!
					}
					// [es3.5:ENT:kuk                           :I:K:test                          ]
					//  0123456789012345678901234567890123456789012345
					if (iCommand.substring(40, 42).equals(":I")) {
						logger.info("handler=[ENT:I]");				
						r = new ClientHandler_in_alt();
					} else {
						logger.info("handler=[ENT:O]");				
						r = new ClientHandler_out_alt();						
					}
					ac.notifyAboutRequest(clserv.shared.Constants.RequestType.INOUT);
				} else if (iCommand.substring(5, 10).equals(":INF:")) {
					// [es3.5:INF:kuk                           :]
					//  0123456789012345678901234567890123456789012345
					logger.info("handler=[INF]");				
					r = new ClientHandler_info_alt();
					ac.notifyAboutRequest(clserv.shared.Constants.RequestType.INFO);
				} else if (iCommand.substring(5, 10).equals(":PZE:")) {
					// [es3.5:PZE:kuk                           :0000000000]
					//  0123456789012345678901234567890123456789012345
					logger.info("handler=[PZE]");				
					r = new ClientHandler_pze_alt();
					ac.notifyAboutRequest(clserv.shared.Constants.RequestType.PZE);
				} else if (iCommand.substring(5, 10).equals(":QMI:")) {
					// [es3.5:QMI:kuk                           :I|F|K:Pattern(30):Firma(30)]
					//  0123456789012345678901234567890123456789012345
					logger.info("handler=[QMI]");				
					r = new ClientHandler_qmi_alt();
					ac.notifyAboutRequest(clserv.shared.Constants.RequestType.QMI);
				} else if (iCommand.substring(5, 10).equals(":AUS:")) {
					// [es3.5:AUS:kuk                           :DATUMPATTERN]
					// [es3.5:AUS:                              :20070602*.pze
					//  0123456789012345678901234567890123456789012345
					logger.info("handler=[AUS]");				
					r = new ClientHandler_aus_alt();
					ac.notifyAboutRequest(clserv.shared.Constants.RequestType.AUS);
				}
			}
		}
		if (r == null) {
			logger.error("unknown command/no handler found!");			
		}
		return r;
	}

	public static String createResponse_ok(String iMessage) {
		// <ok[ m="message"]/>
		String r = null;
		if (iMessage != null) {
			r = "<ok m=\"" + iMessage+"\"/>";
		} else {
			r = "<ok/>";
		}
		return r;
	}	

	public static String createResponse_error(String iMessage) {
		// <error[ m="message"]/>
		String r = null;
		if (iMessage != null) {
			r = "<error m=\"" + iMessage+"\"/>";
		} else {
			r = "<error/>";
		}
		return r;
	}	
	
	public static String createResponse_info() {
		// <info:version:requests:threads:max threads:uptime>
		// <info v="Version" n="Name" r="requests" t="threads" mt="max threads" u="uptime"/>
		AppContext ac = AppContext.getInstance();
		String r = "<info v=\"" + clserv.server.getVersionString();
		r += "\" n=\""+ac.getMServerName();
		r += "\" r=\""+ac.getCntConnections();
		r += "\" t=\""+Thread.activeCount();
		r += "\" mt=\""+ac.getMaxThreadsAllTime();
		r += "\" u=\""+new DecimalFormat("0.00").format(ac.getUptime()/1000/60/60F)+"\"/>";
		return r;
	}
	public static String createResponse_pzeHead() {
		// <pze-start t="time_as_long">
		String r = "<pze "+clserv.shared.Constants.RequestValueAttrs.Timestamp+"=\""+Helper.getCurrentTimeMillis()+"\">";
		return r;
	}
	public static String createResponse_pzeDataStart(DaoES_LOGS iLog) {
		return "<entry l=\""+iLog.getLogin()+"\">";
	}
	public static String createResponse_pzeData(DaoES_LOGS iLog) {
		// <login:entry_type:timestamp:info>
		String r = null;
		if (iLog != null) {
			r = "<log io=\""+iLog.getEntry_type()+"\" ts=\""+dateformat.format(iLog.getEntry_ts())+"\""+(iLog.getInfo() == null ? "" : (" i=\""+iLog.getInfo()+"\""))+"/>";
		}			
		return r;
	}
	public static String createResponse_pzeDataFinish(DaoES_LOGS entry) {
		return "</entry>";
	}
	public static String createResponse_pzeTail() {
		return "</pze>";
	}

	public static SimpleDateFormat createDateFormat_alt() {
		SimpleDateFormat r = new SimpleDateFormat("HH:mm:ss,dd-MM-yy");
		return r;
	}
	
	public static String createResponse_pzeData_alt_mit_Mitarbeiter(DaoES_LOGS iLog, Mitarbeiter ma) {
		SimpleDateFormat df = ClientHandlerFactory.createDateFormat_alt();
		String r = null;
//		[00,0000004702,06:44:00,03-07-07,IN,00,Reissner        ,Thomas     (572),(ret)                         ]
		r = "00,0000004702,";
		// zeitpunkt
		r += df.format(iLog.getEntry_ts());
		// fix
		if (iLog.getEntry_type().equals(clserv.model.tables.Columns.StrConstrENTRY_TYPE_In)) {
			r += ",IN,00,";				
		} else {
			r += ",OU,00,";
		}
		// name
		r += String.format("%-16s,", new Object[]{ma.getName()});
		// vname
		if (ma.getDurchwahl().length() == 4) {
			r += String.format("%-10s", new Object[]{ma.getFirstName()});			
			r += "(" + ma.getDurchwahl() + "),";
		} else if (ma.getDurchwahl().length() == 3) {
			r += String.format("%-11s", new Object[]{ma.getFirstName()});
			r += "(" + ma.getDurchwahl() + "),";
		} else {
			r += String.format("%-16s,", new Object[]{ma.getFirstName()});			
		}
		// info
		if (iLog.getInfo() == null || iLog.getInfo().length() < 25) {
			r += "(" + ma.getLogin() + ")";
			r += String.format("%-25.25s\n", new Object[]{iLog.getInfo() == null ? "" : iLog.getInfo()});
		} else {
			r += String.format("%-30.30s\n", new Object[]{iLog.getInfo() == null ? "" : iLog.getInfo()});
		}
		return r;
	}
	public static String createResponse_pzeData_alt(DaoES_LOGS iLog) {
		String r = null;
		Mitarbeiter ma = (Mitarbeiter )AppContext.getInstance().findeMitarbeiterMitLogin(iLog.getLogin());
		if (ma == null) {
			ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(iLog.getLogin());
		}
		r = createResponse_pzeData_alt_mit_Mitarbeiter(iLog, ma);
		return r;
	}

	public static String createResponse_pzeData_neu(DaoES_LOGS iLog) {
		SimpleDateFormat df = ClientHandlerFactory.createDateFormat_alt();
		String r = null;
		Mitarbeiter ma = (Mitarbeiter )AppContext.getInstance().findeMitarbeiterMitLogin(iLog.getLogin());
		if (ma == null) {
			ma = Mitarbeiter.erzeugeAnonymenMitarbeiter(iLog.getLogin());
		}
//		[00,0000004702,06:44:00,03-07-07,IN,00,Reissner        ,Thomas     (572),(ret)                         ]
		r = "00,0000004702,";
		// zeitpunkt
		r += df.format(iLog.getEntry_ts());
		// fix
		if (iLog.getEntry_type().equals(clserv.model.tables.Columns.StrConstrENTRY_TYPE_In)) {
			r += ",IN,00,";				
		} else {
			r += ",OU,00,";
		}
		// name
		r += String.format("%-16s,", new Object[]{ma.getName()});
		// vname
		if (ma.getDurchwahl().length() == 4) {
			r += String.format("%-10s", new Object[]{ma.getFirstName()});			
			r += "(" + ma.getDurchwahl() + "),";
		} else if (ma.getDurchwahl().length() == 3) {
			r += String.format("%-11s", new Object[]{ma.getFirstName()});
			r += "(" + ma.getDurchwahl() + "),";
		} else {
			r += String.format("%-16s,", new Object[]{ma.getFirstName()});			
		}
		// info
		if (iLog.getInfo() == null || iLog.getInfo().length() < 25) {
			r += "(" + ma.getLogin() + ")";
			r += String.format("%-25.25s", new Object[]{iLog.getInfo() == null ? "" : iLog.getInfo()});
		} else {
			r += String.format("%-30.30s", new Object[]{iLog.getInfo() == null ? "" : iLog.getInfo()});
		}
		// Zusatz
		if (ma.getPosition() != null) {
			r += ma.getPosition();// + " / " + ma.getAutonummer();
		}
		r += "\n";
		return r;
	}
	
	public static String createResponse_Statusmeldung(AppContext ac) {
		int count_logs = 0, count_ma = 0;
		synchronized (ac) {
			Iterator<Mitarbeiter> maIter = ac.getMitarbeiterIterator();
			while (maIter != null && maIter.hasNext()) {
				count_ma++;
				Mitarbeiter ma = maIter.next();
				count_logs += ma.getLogCount();
			}
		}
		String r = "\r\n== clservJ ==\n\rversion: "
			+ clserv.server.getVersionString()
			+ "\r\n"
			+ "server name: "
			+ ac.getMServerName()
			+ "\r\n"
			+ "uptime: "
			+ new DecimalFormat("0.00").format(ac.getUptime() / 1000 / 60 / 60F)
			+ " h\r\n" + "threads act/max: "
			+ java.lang.Thread.activeCount() + "/"
			+ ac.getMaxThreadsAllTime() + "\r\n"
			+ "java (version): "
			+ System.getProperty("java.home")
			+ " (" + System.getProperty("java.version") + ")\r\n"	
			+ "memory max/total/free: "
			+ Runtime.getRuntime().maxMemory() / (1024*1024) + "M/"
			+ Runtime.getRuntime().totalMemory() / (1024*1024) + "M/"
			+ Runtime.getRuntime().freeMemory() / (1024*1024) + "M\r\n"
			+ "employees/logs: "
			+ count_ma + "/" + count_logs + "\r\n"
			+ "DB pool: in use/free/limit/max(all time): "
			+ clserv.shared.Database.getActualPoolsize_inUse() + "/"
			+ clserv.shared.Database.getActualPoolsize_notInUse() + "/"
			+ ac.getDBMaxPoolSize() + "/"
			+ clserv.shared.Database.getActualPoolsize_allTime() + "\r\n"
			+ "emergency logs: "
			+ ac.getCntEmergencyLogs() + "\r\n"
			+ "TimeoutClient (sec.): "
			+ ac.getTimeoutClient() + "\r\n"
			+ "TimeoutWatcher (sec.): "
			+ ac.getTimeoutWatcher() + "\r\n"
			+ "clients registered: "
			+ ac.getCntUserClients() + "\r\n"
			+ "client connections: "
			+ ac.getCntConnections() + "\r\n"
			+ "requests: "
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.ALL_OF_THEM) + "\r\n"
			+ "qmi:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.QMI)
			+ ",pze:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.PZE)
			+ ",aus:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.AUS)
			+ ",inout:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.INOUT)
			+ ",reg:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.REG)
			+ ",msg:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.MSG)
			+ ",info:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.INFO)
			+ ",telnet:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.TELNET)
			+ ",help:"
			+ ac.getRequestCount(clserv.shared.Constants.RequestType.HELP)
			+ "\r\n";
			Object[] plugin_array = clserv.tools.PluginManager.getInstance().getListOfPlugins();
			if (plugin_array != null/* && plugin_array.length > 0*/) {
				r += "active plugins:\r\n";
				for (int i = 0; i < plugin_array.length; i++) {
					r += "- " + plugin_array[i];
					String ver = clserv.tools.PluginManager.getInstance().getInstanceOfPlugin((String)plugin_array[i]).getVersion();
					if (ver != null) r += " v" + ver;
					String desc = clserv.tools.PluginManager.getInstance().getInstanceOfPlugin((String)plugin_array[i]).getDescription();
					if (desc != null) r += ", " + desc;
					r += "\r\n";
				}
			}
			r += "\r\n>>";
		return r;
	}

}
