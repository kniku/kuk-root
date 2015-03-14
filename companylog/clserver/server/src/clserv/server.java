package clserv;
import clserv.tools.Server;

public class server {
//	private static Logger logger = Logger.getLogger(server.class);
	private static final String Version = "1.0.0a";

	public static String getVersionString() {
		return Version;
	}
	/**
	 * @param args
	 */
	public static void main(String[] args) {

//		BasicConfigurator.configure();
//		PropertyConfigurator.configureAndWatch( "log4j.properties", 60*1000 );		
		System.out.println(
				"clserv, v" + getVersionString() + " startup, vm="
				+ System.getProperty("java.home")
				+ " ("
				+ System.getProperty("java.version")
				+ " on "
				+ System.getProperty("os.arch")
				+ " "
				+ System.getProperty("os.name")
				+ " "
				+ System.getProperty("os.version")
				+ ")...");
		System.out.println("current dir ="+System.getProperty("user.dir"));
		System.out.println("current time="+clserv.shared.Helper.getCurrentTime());

		Server theServer = new Server();
		//theServer.runAll();
		theServer.start();
		
/*		try {
			Thread.sleep(5000);
		} catch (Exception e) {
		}
		theServer.stop();
		System.exit(0);
*/
		//		logger.info("shutdown...");
		// ...
//		logger.info("shutdown done.");
	}

}
