using System;
using System.Collections;
using knk.shared.logger;
using knk.shared.tools;

namespace knk.DBTools
{
	public class ConnectionPool
	{
		/// <summary>
		/// Alle unterstützten DBMS Typen
		/// </summary>
		public enum ProviderType {
			[StringValue("none")]		NONE = 0,
			[StringValue("postgres")]	Postgres,
			[StringValue("sqlite")]		SQLite,
			[StringValue("sqlserver")]	SqlServer
		}

		// Für Konstruktor Typ 1: Alle Parameter
		string mServer, mDatabase, mUser, mPassword;
		uint mPort;
		// Für Konstruktor Typ 2: Fertiger Connectionstring
		string mConnectionString;
		// DBMS Typ
		public ConnectionPool.ProviderType Provider { private set; get; }

		ArrayList arrConn = new ArrayList();

		/// <summary>
		/// Erzeugt einen neuen ConnectionPool mit gegebenene Parametern
		/// </summary>
		/// <param name="iProvider">Der DBMS Typ der Verbindung<see cref="ProviderType"/></param>
		/// <param name="iServer">Der Servername oder IP Adresse</param>
		/// <param name="iDatabase">Der Datenbankname</param>
		/// <param name="iUser">Der Datenbankbenutzer</param>
		/// <param name="iPassword">Kennwort</param>
		/// <param name="iPort">Die Portnummer (0=Typabhängiger Default Port wird verwendet)</param>
		public ConnectionPool(ProviderType iProvider, string iServer, string iDatabase, string iUser, string iPassword, uint iPort)
		{
			mServer = iServer;
			mDatabase = iDatabase;
			mUser = iUser;
			mPassword = iPassword;
			mPort = iPort;
			Provider = iProvider;
		}

		/// <summary>
		/// Erzeugt einen neuen ConnectionPool mit gegebenen Connection String (URL)
		/// </summary>
		/// <param name="iProvider">Der DBMS Typ der Verbindung<see cref="ProviderType"/></param>
		/// <param name="iConnectionString">Der Connectionstring</param>
		public ConnectionPool(ProviderType iProvider, string iConnectionString)
		{
			mConnectionString = iConnectionString;
			Provider = iProvider;
		}

		private Connection findFreeConnection()
		{
			Connection r = null;
			foreach (Connection aConn in arrConn)
			{
				if (aConn != null && aConn.IConnection.State == System.Data.ConnectionState.Closed)
				{
					r = aConn;
					break;
				}
			}
			return r;
		}

		public Connection getConnection()
		{
			Connection r = findFreeConnection();
			if (r == null)
			{
				if (mConnectionString == null)
					r = new Connection(Provider, mServer, mDatabase, mUser, mPassword, mPort);
				else
					r = new Connection(Provider, mConnectionString);
				arrConn.Add(r);
			}
			if (r != null) r.IConnection.Open();
			return r;
		}

		public bool checkConnection(ILogger iLog)
		{
			bool r = false;

			try
			{
				Connection c = getConnection();
				if (c != null && c.IConnection.State == System.Data.ConnectionState.Open)
				{
					r = true;
				}
				releaseConnection(c);
			}
			catch (Exception e)
			{
				if (iLog != null)
				{
					iLog.LogError("{0}", e.Message);
				}
			}
			return r;
		}

		public Connection releaseConnection(Connection iConnection)
		{
			if (iConnection != null)
			{
				if (iConnection.isTransactionInProgress) iConnection.TSQL_rollback();
				if (iConnection.IConnection.State == System.Data.ConnectionState.Open) iConnection.IConnection.Close();
			}
			return null;
		}

		public void closeAllConnections()
		{
			foreach (Connection aConn in arrConn)
			{
				releaseConnection(aConn);
			}
		}

		public void shutdown()
		{
			closeAllConnections();
			foreach (Connection aConn in arrConn)
			{
				aConn.shutdown();
			}
			arrConn.Clear();
		}
	}
}
