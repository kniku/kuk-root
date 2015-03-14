package shared.bo;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;


public class KontoManager {

	public void loadList(Connection iConn, IKontoManagerWalker iWalker, Object iCalldata) throws SQLException {
		PreparedStatement stmt = iConn.prepareStatement(
				"select id,blz,ktonr,curcode,ktoname,ktodescr from konten"
			);
			stmt.execute();
			ResultSet rs = stmt.getResultSet();
			while (rs.next()) {
				Konto k = new Konto();
				k.setId(rs.getLong(1));
				k.setBlz(rs.getString(2));
				k.setKontonummer(rs.getString(3));
				k.setWaehrung(rs.getString(4));
				k.setKontoname(rs.getString(5));
				k.setBeschreibung(rs.getString(6));
				if (iWalker.handle(k, iCalldata) == false) break;
			}
			stmt.close();
	}
}
