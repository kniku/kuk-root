using System;
using System.Data;
using Microsoft.Extensions.DependencyInjection;
using System.Data.Common;

namespace WebAPI.DbAccess
{
	public class DbAccess
	{
		public IDbAccessFactory DbAccessFactory { get; }
		public string ConnectionString { get; }

		public DbAccess(string connectionString)
		{
			DbAccessFactory = AppGlobal.Application.ApplicationServices.GetService<IDbAccessFactory>();
			ConnectionString = connectionString;
		}

		public Exception CheckConnection()
		{
			try
			{
				GetConnection(true).Close();
			}
			catch (Exception e)
			{
				return e;
			}

			return null;
		}

		//public DbConnectionStringBuilder ConnectionStringBuilder
		//{
		//	get
		//	{
		//		var b = DbAccessFactory.DbConnectionStringBuilder;
		//		b.ConnectionString = ConnectionString;
		//		return b;
		//	}
		//}

		public DbConnection GetConnection(bool openConnection = false)
		{
			var r = DbAccessFactory.DbConnection;
			r.ConnectionString = ConnectionString;
			
			if (openConnection)
				r.Open();

			return r;
		}

		public DbCommand GetCommand(string sql, DbConnection connection = null)
		{
			var r = DbAccessFactory.DbCommand;
			if (connection == null)
			{
				connection = GetConnection(true);
			}

			r.Connection = connection;
			r.CommandText = sql;
			return r;
		}

	}
}
