package bitools.shared;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;

import org.apache.log4j.Logger;

public class AppContext {
	private static AppContext mTheAppContext = null;
	private java.util.Properties mProperties = null;
	private static Logger logger = Logger.getLogger(AppContext.class);
	private String mAppName = null;

	// SINGLETON!
	public static AppContext getInstance(String iAppName) throws IOException {
		if (mTheAppContext == null) {
			mTheAppContext = new AppContext(iAppName);
		}
		return mTheAppContext;
	}

	private AppContext(String iAppName) throws IOException {
		mAppName = iAppName;
		mProperties = new java.util.Properties();
		FileInputStream f = openPropertyFile();
		if (f!=null) {
			mProperties.load(f);
			f.close();
		}
	}
	
	public FileInputStream openPropertyFile() throws FileNotFoundException {
		FileInputStream r = null;
		String fn = mAppName + ".properties";
		logger.debug("Suche property file: " + fn);
		URL u = logger.getClass().getClassLoader().getResource(fn);
		if (u != null) {
			logger.debug("property file gefunden: " + u.getPath());
			r = new FileInputStream(u.getPath());
		} else {
			logger.error("property file NICHT gefunden: " + fn);
		}
		return r;
	}
	
	public String getProperty(String iPropertyName) {
		return mProperties.getProperty(iPropertyName, null);
	}
	
	public void overruleProperty(String iPropertyName, String iPropertyValue) {
		mProperties.setProperty(iPropertyName, iPropertyValue);
	}
}
