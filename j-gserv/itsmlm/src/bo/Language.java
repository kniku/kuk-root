package bo;

public class Language {
	private String language;
	private String descr;

	public Language(String language, String descr) {
		this.language = language;
		this.descr = descr;
	}

	public String getLanguage() {
		return language;
	}

	public String getDescr() {
		return descr;
	}

	public String getCombo() {
		return descr + " (" + language + ")";
	}
	
	public static String getLanguageFromCombo(String iCombo) {
		if (iCombo != null && !iCombo.isEmpty()) {
			int i1 = iCombo.lastIndexOf("(");
			int i2 = iCombo.lastIndexOf(")");
			return iCombo.substring(i1 + 1, i2);
		} else return null;
	}
	
	public static String getDescrFromCombo(String iCombo) {
		if (iCombo != null && !iCombo.isEmpty()) {
			int i1 = iCombo.lastIndexOf("(");
			return iCombo.substring(0, i1 - 1);
		} else return null;
	}

}
