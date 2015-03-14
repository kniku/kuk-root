package shared;

public final class Version {

	private static final String Name = "gserv";
	private static final String Version = "0.1a";

	public static String getVersionString() {
		return Version;
	}
	public static String getAppVersionString() {
		return Name + ", v" + Version;
	}
}
