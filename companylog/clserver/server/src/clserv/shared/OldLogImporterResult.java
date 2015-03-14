package clserv.shared;

public class OldLogImporterResult {
	public int file_lines = 0;
	public int imports_ok = 0;
	public int imports_err = 0;
	public int imports_dbl = 0;
	
	public void reset() {
		file_lines = 0;
		imports_ok = 0;
		imports_err = 0;
		imports_dbl = 0;
	}
}
