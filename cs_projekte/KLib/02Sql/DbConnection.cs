//#define DbTypeNeeded
using Npgsql;
using MySql;
//using System.Data.SqlClient;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

namespace KLib.Sql
{
	public partial class DbConnection
	{
		static log4net.ILog Logger = log4net.LogManager.GetLogger(typeof(DbConnectionManager));

		private IDbConnection NativeConnection { set; get; }
		DbConnectionManager mConnectionManager;

		/// <summary>
		/// Konstruktor: Erzeugt eine Connection für den geg. ConnectionManager
		/// </summary>
		/// <param name="iConnectionManager"></param>
		public DbConnection(DbConnectionManager iConnectionManager)
		{
			uint Port = iConnectionManager.Port;
			string Server = iConnectionManager.Server;
			mConnectionManager = iConnectionManager;
			string connstring;

			switch (iConnectionManager.Provider)
			{
				case DbConnectionManager.ProviderType.Postgres:
					if (Port == 0) Port = 5432;   // postgres default port...
					if (Server == null) Server = "localhost";
					connstring = String.Format("Server={0};Port={1};User Id={2};Password={3};Database={4};Pooling=true;",
						Server, Port, iConnectionManager.User, iConnectionManager.Password, iConnectionManager.Database);
					NativeConnection = new NpgsqlConnection(connstring);
					break;
				case DbConnectionManager.ProviderType.MySql:
					if (Port == 0) Port = 3306;   // mysql default port...
					if (Server == null) Server = "localhost";
					connstring = String.Format("Server={0};Port={1};User Id={2};Password={3};Database={4};Pooling=true;",
						Server, Port, iConnectionManager.User, iConnectionManager.Password, iConnectionManager.Database);
					NativeConnection = new MySql.Data.MySqlClient.MySqlConnection(connstring);
					break;
				case DbConnectionManager.ProviderType.SqlServer:
					if (Port == 0) Port = 1433;   // SqlServer default port...
					if (Server == null) Server = "localhost";
					connstring = String.Format("Server={0},{1};User Id={2};Password={3};Database={4};Pooling=true;Connection Timeout=5;",
						Server, Port, iConnectionManager.User, iConnectionManager.Password, iConnectionManager.Database);
					NativeConnection = new System.Data.SqlClient.SqlConnection(connstring);
					break;
				default:
					throw new NotImplementedException("Providertyp nicht unterstützt: " + mConnectionManager.Provider);
			}
		}

		public void open()
		{
			Logger.DebugFormat("DbConnection open: {0}", GetHashCode());
			try
			{
				if (NativeConnection != null && NativeConnection.State != ConnectionState.Open) NativeConnection.Open();
			}
			catch (Exception _ex)
			{
				Logger.ErrorFormat("DbConnection open: {0} ERROR: {1}", GetHashCode(), _ex.Message);
			}
		}

		public void close()
		{
			Logger.DebugFormat("DbConnection close: {0}", GetHashCode());
			if (NativeConnection != null && NativeConnection.State != ConnectionState.Closed) NativeConnection.Close();
		}

		public ConnectionState getState()
		{
			return NativeConnection.State;
		}

		IDbCommand createCommand()
		{
			IDbCommand r = null;

			switch (mConnectionManager.Provider)
			{
				case DbConnectionManager.ProviderType.Postgres:
					r = new NpgsqlCommand();
					break;
				case DbConnectionManager.ProviderType.MySql:
					r = new MySql.Data.MySqlClient.MySqlCommand();
					break;
				case DbConnectionManager.ProviderType.SqlServer:
					r = new System.Data.SqlClient.SqlCommand();
					break;
				default:
					throw new NotImplementedException("Providertyp nicht unterstützt: " + mConnectionManager.Provider);
			}
			return r;
		}

		IDbDataParameter createParameter()
		{
			IDbDataParameter r = null;

			switch (mConnectionManager.Provider)
			{
				case DbConnectionManager.ProviderType.Postgres:
					r = new NpgsqlParameter();
					break;
				case DbConnectionManager.ProviderType.MySql:
					r = new MySql.Data.MySqlClient.MySqlParameter();
					break;
				case DbConnectionManager.ProviderType.SqlServer:
					r = new System.Data.SqlClient.SqlParameter();
					break;
				default:
					throw new NotImplementedException("Providertyp nicht unterstützt: " + mConnectionManager.Provider);
			}

			return r;
		}

		void initParam(IDbDataParameter iParam, string iParamName, string iObjectType, object iObject)
		{
#if DbTypeNeeded
			Type t = null;
			iParam.ParameterName = iParamName;

			if (iObject != null)
			{
				t = iObject.GetType();
				iObjectType = t.Name;
				iParam.Value = iObject;
			}

			switch (iObjectType)
			{
				case "Int32":
					iParam.DbType = DbType.Int32;
					break;
				case "Int64":
					iParam.DbType = DbType.Int64;
					break;
				case "Double":
					iParam.DbType = DbType.Double;
					break;
				case "DateTime":
					iParam.DbType = DbType.DateTime;
					break;
				case "String":
					iParam.DbType = DbType.String;
					if (iObject != null) iParam.Size = ((String)iObject).Length;
					if (iParam.Size == 0) iParam.Size = 1;	// sonst Problem mit leeren strings
					break;
				case "Guid":
					iParam.DbType = DbType.Guid;
					break;
				default:
					throw new NotImplementedException("Datentyp nicht unterstützt: " + iObjectType);
			}
#else
			iParam.ParameterName = iParamName;
			iParam.Value = iObject;
#endif
		}

		IDbCommand prepareSQL(string iSql, params object[] iParams)
		{
			IDbCommand r = createCommand();
			if (r != null)
			{
				r.Connection = NativeConnection;

				switch (mConnectionManager.Provider)
				{
					case DbConnectionManager.ProviderType.SqlServer:
						iSql = iSql.Replace(':', '@');
						break;
					case DbConnectionManager.ProviderType.MySql:
						iSql = iSql.Replace(':', '?');
						break;
					case DbConnectionManager.ProviderType.Postgres:
						// nothing to do...
						//iSql = iSql.Replace(':', '$');
						break;
					default:
						throw new NotImplementedException("Providertyp nicht unterstützt: " + mConnectionManager.Provider);
				}

				r.CommandText = iSql;
				r.Transaction = current_transaction;

				if (iParams != null)
				{
					for (int i = 0; i < iParams.Count(); i++)
					{
						IDbDataParameter aParam = createParameter();
						initParam(aParam, (i + 1).ToString(), null, iParams[i]);
						r.Parameters.Add(aParam);
					}
				}
				r.Prepare();
			}
			return r;
		}

		public IEnumerator<DataRow> execSQL_select(string iSql, params object[] iSqlParams)
		{
			IEnumerator<DataRow> r = null;

			DataSet ds = new DataSet();
			IDataAdapter da = null;

			IDbCommand cmd = prepareSQL(iSql, iSqlParams);

			switch (mConnectionManager.Provider)
			{
				case DbConnectionManager.ProviderType.Postgres:
					da = new NpgsqlDataAdapter(cmd as NpgsqlCommand);
					//da = new NpgsqlDataAdapter(iSql, (NpgsqlConnection)NativeConnection);
					break;
				case DbConnectionManager.ProviderType.SqlServer:
					da = new System.Data.SqlClient.SqlDataAdapter(cmd as System.Data.SqlClient.SqlCommand);
					//da = new SqlDataAdapter(iSql, (SqlConnection)NativeConnection);
					break;
				default:
					throw new NotImplementedException("Providertyp nicht unterstützt: " + mConnectionManager.Provider);
			}

			da.Fill(ds);

			if (ds.Tables[0] != null)
			{
				r = ds.Tables[0].Rows.GetEnumerator() as IEnumerator<DataRow>;
			}

			return r;
		}

	}
}
