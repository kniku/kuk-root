import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import shared.ServiceStarter;
import shared.Version;


public class GServer {


	public static void main(String[] args) {
		Log log = LogFactory.getLog(GServer.class);
		log.info(
				Version.getAppVersionString() + " startup, vm="
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
		log.info("current dir="+System.getProperty("user.dir"));
		log.info("current time="+shared.Helper.getCurrentTime());
		
		ServiceStarter starter = new ServiceStarter();
		starter.start();
	}

}
