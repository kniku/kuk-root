package bo;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.ArrayList;

public class ListOfLanguages {
	private ArrayList<Language> list = new ArrayList<Language>();
	
	public void reload(Connection iConn) throws Exception {
		
		PreparedStatement stmt = iConn.prepareStatement(
				"select language,descr from mllang order by descr"
		);
		stmt.execute();
		ResultSet rs = stmt.getResultSet();
		while (rs.next()) {
			Language L = new Language(rs.getString(1),rs.getString(2));
			list.add(L);
		}
		stmt.close();
	}

	public ArrayList<Language> getList() {
		return list;
	}

}
