using System;
using System.Data;
using System.Data.SqlClient;

namespace knk.DBTools.ProviderImpl.SqlServer
{
	class ConnectionProvider : IConnectionProvider
	{

		#region IConnectionProvider Member

		public IDbConnection createConnection(string iServer, string iDatabase, string iUser, string iPassword, uint iPort)
		{
			IDbConnection r = null;
			// SQL Server - style connection string
            if (iPort == 0) iPort = 1433;   // Sql Server default port...
//			string connstring = String.Format("Server={0};Port={1};User Id={2};Password={3};Database={4};Pooling=true;",
			string connstring = String.Format("Server={0},{1};User Id={2};Password={3};Database={4};Pooling=true;Connection Timeout=5;",
				iServer, iPort, iUser, iPassword, iDatabase);
			// Making connection with sql server provider
			r = new SqlConnection(connstring);
			return r;
		}

		public IDbConnection createConnection(string iConnectionString)
		{
			SqlConnection r = new SqlConnection(iConnectionString);
			int x = r.ConnectionTimeout;
			return r;
		}

		public void shutdownConnection(IDbConnection conn)
		{
			((SqlConnection)conn).Close();
		}

		public IDbCommand createCommand()
		{
			return new SqlCommand();
		}

		public IDbDataParameter createParameter()
		{
			return new SqlParameter();
		}

		public DataSet FillDataSet(IDbConnection conn, string sql)
		{
			DataSet r = new DataSet();
			SqlDataAdapter da = new SqlDataAdapter(sql, (SqlConnection)conn);
			da.Fill(r, "xxx");
			return r;
		}

		#endregion
	}
}
