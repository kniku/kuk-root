//#define DbTypeNeeded
using Npgsql;
using MySql;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Data.SQLite;

namespace KLib.Sql
{
	public partial class DbConnection
	{
		//static log4net.ILog Logger = log4net.LogManager.GetLogger(typeof(DbConnectionManager));
		private static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

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
				case DbConnectionManager.ProviderType.SQLite:
					if (!System.IO.File.Exists(iConnectionManager.Database))
					{
						SQLiteConnection.CreateFile(iConnectionManager.Database);
					}
					connstring = String.Format(@"Data Source={0};Version=3;FailIfMissing=True;", iConnectionManager.Database);
					NativeConnection = new SQLiteConnection(connstring);
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

		public bool open()
		{
			bool r = false;

			Logger.DebugFormat("DbConnection {0}: open", GetHashCode());
			try
			{
				if (NativeConnection != null && NativeConnection.State != ConnectionState.Open) NativeConnection.Open();
				r = NativeConnection.State == ConnectionState.Open;
			}
			catch (Exception _ex)
			{
				Logger.ErrorFormat("DbConnection {0}: ERROR: {1}", GetHashCode(), _ex.Message);
				if (mConnectionManager.setThrowExceptions) throw;	// weiterwerfen, wenn gewünscht...
			}

			return r;
		}

		public bool close()
		{
			bool r = false;

			Logger.DebugFormat("DbConnection {0}: close", GetHashCode());
			try
			{
				if (NativeConnection != null && NativeConnection.State != ConnectionState.Closed) NativeConnection.Close();
				r = NativeConnection.State == ConnectionState.Closed;
			}
			catch (Exception _ex)
			{
				Logger.ErrorFormat("DbConnection {0}: ERROR: {1}", GetHashCode(), _ex.Message);
				if (mConnectionManager.setThrowExceptions) throw;	// weiterwerfen, wenn gewünscht...
			}

			return r;
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
				case DbConnectionManager.ProviderType.SQLite:
					r = new SQLiteCommand();
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
				case DbConnectionManager.ProviderType.SQLite:
					r = new SQLiteParameter();
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
			// seems to be a bug of npgsql (postgres)
			if (mConnectionManager.Provider == DbConnectionManager.ProviderType.Postgres)
			{
				if (iObject is Int16)
				{
					iParam.DbType = DbType.Int16;
				}
				else if (iObject is Int32)
				{
					iParam.DbType = DbType.Int32;
				}
			}
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
					case DbConnectionManager.ProviderType.SQLite:
//						iSql = iSql.Replace(':', '@');
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

		IDataAdapter createDataAdapter(IDbCommand iCmd)
		{
			IDataAdapter r = null;
			
			if (iCmd != null)
			{
				switch (mConnectionManager.Provider)
				{
					case DbConnectionManager.ProviderType.MySql:
						r = new MySql.Data.MySqlClient.MySqlDataAdapter(iCmd as MySql.Data.MySqlClient.MySqlCommand);
						break;
					case DbConnectionManager.ProviderType.Postgres:
						r = new NpgsqlDataAdapter(iCmd as NpgsqlCommand);
						break;
					case DbConnectionManager.ProviderType.SqlServer:
						r = new System.Data.SqlClient.SqlDataAdapter(iCmd as System.Data.SqlClient.SqlCommand);
						break;
					case DbConnectionManager.ProviderType.SQLite:
						r = new SQLiteDataAdapter(iCmd as SQLiteCommand);
						break;
					default:
						throw new NotImplementedException("Providertyp nicht unterstützt: " + mConnectionManager.Provider);
				}
			}
			return r;
		}
	}
}
