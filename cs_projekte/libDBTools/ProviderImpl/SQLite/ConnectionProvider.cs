using System;
using System.Data;
using System.Data.SQLite;

namespace knk.DBTools.ProviderImpl.SQLite
{
	class ConnectionProvider : IConnectionProvider
	{

		#region IConnectionProvider Member

		public IDbConnection createConnection(string iServer, string iDatabase, string iUser, string iPassword, uint iPort)
		{
			IDbConnection r = null;
			// SQLite-style connection string
			string connstring = String.Format("Data Source={0};Version=3;Pooling=True;",
				iDatabase);
			// Making connection with sql provider
			r = new SQLiteConnection(connstring);
			return r;
		}

		public IDbConnection createConnection(string iConnectionString)
		{
			return new SQLiteConnection(iConnectionString);
		}

		public void shutdownConnection(IDbConnection conn)
		{
//			((SQLiteConnection)conn).Shutdown();
		}

/*		public IDbCommand getCommand(IDbConnection conn, string sql)
		{
			IDbCommand r = new NpgsqlCommand(sql, (NpgsqlConnection)conn);
			return r;
		}
*/		
		public IDbCommand createCommand()
		{
			return new SQLiteCommand();
		}

		public IDbDataParameter createParameter()
		{
			return new SQLiteParameter();
		}

		public DataSet FillDataSet(IDbConnection conn, string sql)
		{
//			DataTable r = new DataTable();

			DataSet r = new DataSet();
			SQLiteDataAdapter da = new SQLiteDataAdapter(sql, (SQLiteConnection)conn);
			da.Fill(r, "xxx");
			return r;
		}

		#endregion
	}
}
