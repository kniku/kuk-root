package clserv.shared;

public class Constants {

	public enum RequestType {QMI, PZE, AUS, INOUT, INFO, TELNET, HELP, ALL_OF_THEM, MSG, REG, SQL}
	public final static class RequestTypeElements {
		public static final String IN = "in";
		public static final String OUT = "out";
		public static final String AUS = "aus";
		public static final String HELP = "help";
		public static final String PZE = "pze";
		public static final String QMI = "qmi";
		public static final String TELNET = "*";
		public static final String REG = "reg";
		public static final String UNREG = "unreg";
		public static final String MSG = "msg";
	}

	public final static class RequestValueAttrs {
		public final static String Login = "l";
		public final static String Info = "i";
		public final static String IsPrelog = "pre";
		public final static String Timestamp = "t";
		public static final String Port = "port";
		public static final String To = "to";
		public static final String Msg = "m";
	}	

	public enum EntryType {IN, OUT}
	public final static class EntryTypeStr {
		public static final String IN = "I";
		public static final String OUT = "O";
	}

	public final static class ResponseTypeElements {
		public static final String INFO = "info";
	}
	
	public final static class ResponseValueAttrs {
		public final static String INFO_ServerVersion = 	"v";
		public final static String INFO_ServerName = 		"n";
		public final static String INFO_ServerUptime = 		"u";
		public final static String INFO_ServerThreads = 	"t";
		public final static String INFO_ServerThreadsMax = 	"mt";
		public final static String INFO_ServerRequests = 	"r";
	}
	
}
