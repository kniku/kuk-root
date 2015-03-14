package clserv.model.tables;

import clserv.model.tablespec.I_TableSpec;

/**
 * @author kuk
 *
 */
public class Schema {
	
	private static I_TableSpec[] theModel = {
		clserv.model.tables.ES_LOGS.getInstance(),
		clserv.model.tables.ES_PRELOGS.getInstance()
	};

	public static I_TableSpec[] getSchema() {
		return theModel;
	}
}
