using System.Data;

namespace knk.DBTools.ProviderImpl
{
	interface IConnectionProvider
	{
		IDbConnection createConnection(string iServer, string iDatabase, string iUser, string iPassword, uint iPort);
		IDbConnection createConnection(string iConnectionString);
		void shutdownConnection(IDbConnection conn);
		IDbCommand createCommand();
		IDbDataParameter createParameter();
		DataSet FillDataSet(IDbConnection conn, string sql);
	}
}
