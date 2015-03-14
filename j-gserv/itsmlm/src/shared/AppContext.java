package shared;

import javax.sql.DataSource;

public class AppContext {
	
	private DataSource DataSource = null;
	private boolean MltransHistoryable = false;
	
	public DataSource getDataSource() {
		return DataSource;
	}
	public void setDataSource(DataSource dataSource) {
		DataSource = dataSource;
	}
	public boolean isMltransHistoryable() {
		return MltransHistoryable;
	}
	public void setMltransHistoryable(boolean mltransHistoryable) {
		MltransHistoryable = mltransHistoryable;
	}

	
}
