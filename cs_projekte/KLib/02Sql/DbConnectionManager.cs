﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KLib.Sql
{
	/// <summary>
	/// Verwaltet Verbindungen zu einer bestimmten Datenbank.
	/// </summary>
	public class DbConnectionManager
	{
//		static log4net.ILog Logger = log4net.LogManager.GetLogger(typeof(DbConnectionManager));
		private static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);

		/// <summary>
		/// Unterstützte Datenbank Provider
		/// </summary>
		public enum ProviderType
		{
			NONE = 0,
			Postgres,
			SQLite,
			SqlServer,
			MySql
		}

		//public IDbConnection IConnection { private set; get; }
		public string Server { private set; get; }
		public string Database { private set; get; }
		public string User { private set; get; }
		public string Password { private set; get; }
		public uint Port { private set; get; }
		public ProviderType Provider { private set; get; }
		public string ConnectionString { private set; get; }


		/// <summary>
		/// Sollen Exceptions an Applikation durchgereicht werden (default=false)
		/// </summary>
		public bool setThrowExceptions { set; get; }

		public DbConnectionManager(ProviderType iProvider, string iConnectionString)
		{
			Logger.DebugFormat("new DbConnectionManager with connection string. Type={0}", iProvider);
			Provider = iProvider;
			ConnectionString = iConnectionString;
		}

		public DbConnectionManager(ProviderType iProvider, string iServer, string iDatabase, string iUser, string iPassword, uint iPort = 0)
		{
			Logger.DebugFormat("new DbConnectionManager: {0}@{1}:{2}/{3} - Type={4}", iUser, iServer, iPort, iDatabase, iProvider);
			Provider = iProvider;
			Server = iServer;
			Database = iDatabase;
			User = iUser;
			Password = iPassword;
			Port = iPort;
			setThrowExceptions = false;
		}

		/// <summary>
		/// Eine freie Verbindung anfordern
		/// </summary>
		/// <returns>Eine Verbindung zu der Datenbank</returns>
		public DbConnection getConnection()
		{
			DbConnection r = new DbConnection(this);
			Logger.DebugFormat("new DbConnection: {0}", r.GetHashCode());
			return r;
		}

	}
}
