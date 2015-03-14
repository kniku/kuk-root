using System;
using System.Data;
using Npgsql;

namespace knk.DBTools.ProviderImpl.Postgres
{
	class ConnectionProvider : IConnectionProvider
	{

		#region IConnectionProvider Member

		public IDbConnection createConnection(string iServer, string iDatabase, string iUser, string iPassword, uint iPort)
		{
			IDbConnection r = null;
			// PostgeSQL-style connection string
            if (iPort == 0) iPort = 5432;   // postgres default port...
			string connstring = String.Format("Server={0};Port={1};User Id={2};Password={3};Database={4};Pooling=true;",
				iServer, iPort, iUser, iPassword, iDatabase);
			// Making connection with Npgsql provider
			r = new NpgsqlConnection(connstring);
			return r;
		}

		public IDbConnection createConnection(string iConnectionString)
		{
			return new NpgsqlConnection(iConnectionString);
		}

		public void shutdownConnection(IDbConnection conn)
		{
			((NpgsqlConnection)conn).ClearPool();
		}

		public IDbCommand createCommand()
		{
			return new NpgsqlCommand();
		}

		public IDbDataParameter createParameter()
		{
			return new NpgsqlParameter();
		}

		public DataSet FillDataSet(IDbConnection conn, string sql)
		{
			DataSet r = new DataSet();
			NpgsqlDataAdapter da = new NpgsqlDataAdapter(sql, (NpgsqlConnection)conn);
			da.Fill(r, "xxx");
			return r;
		}

		#endregion
	}
}
