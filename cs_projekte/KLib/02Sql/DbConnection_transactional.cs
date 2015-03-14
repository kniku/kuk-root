using Npgsql;
using MySql;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Data.SqlClient;

namespace KLib.Sql
{
	public partial class DbConnection
	{
		IDbTransaction current_transaction = null;
		public bool isTransactionInProgress { get { return current_transaction != null; } }

		public void TSQL_startTransaction()
		{
			if (current_transaction == null)
			{
				switch (mConnectionManager.Provider)
				{
					case DbConnectionManager.ProviderType.SqlServer:
						current_transaction = NativeConnection.BeginTransaction(IsolationLevel.Snapshot);
						break;
					default:
						current_transaction = NativeConnection.BeginTransaction();
						break;
				}
			}
		}

		public void TSQL_commit()
		{
			if (current_transaction != null) current_transaction.Commit();
			current_transaction = null;
		}

		public void TSQL_rollback()
		{
			if (current_transaction != null) current_transaction.Rollback();
			current_transaction = null;
		}
	}
}
