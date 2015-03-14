package bo;

public class ListOfTranslationFilter {
	public String language = null;
	public String version = null;
	public String text = null;
	public String translation = null;
	public int source = 0;
	public boolean useCase = false;
	public boolean useInstr = false;
	
	public ListOfTranslationFilter(String language, String version,	String text, String translation, int source, boolean useCase, boolean useInstr) {
		if (language != null) {
			if (language.equalsIgnoreCase("englisch")) this.language = "en";
			else if (language.equalsIgnoreCase("deutsch")) this.language = "de";
			else if (language.equalsIgnoreCase("russisch")) this.language = "ru";
			else this.language = language;
		}
		this.version = version;
		this.text = text;
		this.translation = translation;
		this.source = source;
		this.useCase = useCase;
		this.useInstr = useInstr;
	}
}
