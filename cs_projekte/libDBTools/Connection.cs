using System;
using System.Collections;
using System.Linq;
using System.Data;
using Npgsql;

namespace knk.DBTools
{
	public class Connection
	{
		public IDbConnection IConnection { private set; get; }
		public ConnectionPool.ProviderType Provider { private set; get; }
		ProviderImpl.IConnectionProvider mConnectionProvider;

		public Connection(ConnectionPool.ProviderType iProvider, string iServer, string iDatabase, string iUser, string iPassword, uint iPort)
		{
			mConnectionProvider = ProviderImpl.ProviderFactory.createProvider(iProvider);
			if (mConnectionProvider != null)
			{
				IConnection = mConnectionProvider.createConnection(iServer, iDatabase, iUser, iPassword, iPort);
				Provider = iProvider;
			}
			else
			{
				// no way ...
				throw new Exception("database provider not created!");
			}
		}

		public Connection(ConnectionPool.ProviderType iProvider, string iConnectionString)
		{
			mConnectionProvider = ProviderImpl.ProviderFactory.createProvider(iProvider);
			if (mConnectionProvider != null)
			{
				IConnection = mConnectionProvider.createConnection(iConnectionString);
				Provider = iProvider;
			}
			else
			{
				// no way ...
				throw new Exception("database provider not created!");
			}
		}

		public void shutdown()
		{
			mConnectionProvider.shutdownConnection(IConnection);
		}

		private void initParam(IDbDataParameter iParam, string iParamName, string iObjectType, object iObject)
		{
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
					break;
			}
		}

		public IDbCommand prepareSQL(string iSql, params object[] iParams)
		{
			IDbCommand r = mConnectionProvider.createCommand();
			if (r != null)
			{
				r.Connection = IConnection;
				r.CommandText = iSql;
				r.Transaction = current_transaction;

				if (iParams != null)
				{
					for (int i = 0; i < iParams.Count(); i++)
					{
						IDbDataParameter aParam = mConnectionProvider.createParameter();
						initParam(aParam, (i + 1).ToString(), null, iParams[i]);
						r.Parameters.Add(aParam);
					}
				}
				r.Prepare();
			}
			return r;
		}

		#region simple SQL Interface

		// Fertiges SQL Kommando absetzen
		public string execSQL(string iSql)
		{
			return execSQL(iSql, null);
		}

		// Fertiges SQL Kommando absetzen
		// iCommand muss mittels prepareSQL(...) erzeugt worden sein
		public string execSQL(IDbCommand iCommand)
		{
			string r = null;
			
			if (iCommand != null)
			{
				Object o;
				if (iCommand.CommandText.StartsWith("select", true, null))
				{
					o = iCommand.ExecuteScalar();
				}
				else
				{
					o = iCommand.ExecuteNonQuery();
				}
				if (o != null) r = o.ToString();
			}
			return r;
		}

		// Fertiges SQL Kommando absetzen
		// Parameter werden sauber angebunden
		// Beispiel: execSQL("SELECT ktonr FROM konten where id=:2 and curcode=:1", "EUR", 2);
		public string execSQL(string iSql, params object[] iParams)
		{
			return execSQL(prepareSQL(iSql, iParams));
		}

		#endregion simple SQL Interface

		#region SQL select into ArrayList Interface

		IDataReader current_reader = null;

		public int execSQL_select(string iSql, params ArrayList[] iArgs)
		{
			return execSQL_select(iSql, -1, iArgs);
		}
		public int execSQL_select(IDbCommand iCommand, params ArrayList[] iArgs)
		{
			return execSQL_select(iCommand, -1, iArgs);
		}

		public int execSQL_select(IDbCommand iCommand, int iArraysize, params ArrayList[] iArgs)
		{
			int r = -1;
			int i;
			int cnt_args = iArgs.Count();

			if (current_reader == null && iCommand != null)
			{
				current_reader = iCommand.ExecuteReader();
			}

			// clean lists
			for (i = 0; i < cnt_args; i++)
			{
				((ArrayList)iArgs[i]).Clear();
			}

			if (current_reader != null)
			{
				r = 0;
				int cnt_fields = 0;
				while (current_reader.Read())
				{
					r++;
					if (r == 1)
					{
						cnt_fields = current_reader.FieldCount;
						if (cnt_args < cnt_fields) cnt_fields = cnt_args;
					}
					for (i = 0; i < cnt_fields; i++)
					{
						ArrayList arr = (ArrayList)iArgs[i];
						arr.Add(current_reader.GetValue(i));
					}
					if (r == iArraysize) break;
				}

				if (r == 0 || iArraysize < 0)
				{
					// all data read -> close reader...
					execSQL_cleanup();
				}
			}
			return r;
		}

		public int execSQL_select(string iSql, int iArraysize, params ArrayList[] iArgs)
		{
			return execSQL_select(current_reader == null ? prepareSQL(iSql) : null, iArraysize, iArgs);
		}

		public delegate bool CbDataHandler(DataTable Table, DataRow Row, object Calldata);

		public int execSQL_select(string iSql, CbDataHandler DataHandler, object Calldata)
		{
			int r = -1;

			DataSet ds = mConnectionProvider.FillDataSet(IConnection, iSql);

			DataTable xxx = ds.Tables[0];
			if (xxx != null)
			{
				if (DataHandler(xxx, null, Calldata) == true)
				{
					r = xxx.Rows.Count;
					for (int i = 0; i < r; i++)
					{
						if (DataHandler(xxx, xxx.Rows[i], Calldata) == false)
						{
							// user break request;
							break;
						}
					}
				}
			}
			//ds.Clear();
			return r;
		}

		public void execSQL_cleanup()
		{
			if (current_reader != null)
			{
				current_reader.Close();
				current_reader = null;
			}
		}

		#endregion SQL select into ArrayList Interface

		#region Transactional Interface

		IDbTransaction current_transaction = null;
		public bool isTransactionInProgress { get { return current_transaction != null; } }

		public void TSQL_startTransaction()
		{
			if (current_transaction == null)
			{
				switch (Provider)
				{
					case ConnectionPool.ProviderType.SqlServer:
						current_transaction = IConnection.BeginTransaction(IsolationLevel.Snapshot);
						break;
					default:
						current_transaction = IConnection.BeginTransaction();
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

		#endregion Transactional Interface

		#region Standard Record Interface

		private void mapEntry(TableLookupInfo li, DataRow iRow, Object iObj)
		{
			foreach (ColumnLookupInfo cli in li.Columns)
			{
				foreach (DataColumn aCol in iRow.Table.Columns)
				{
					if (aCol.ColumnName.ToLower().Equals(cli.name_sql.ToLower()))
					{
						Object dobj = iRow[cli.name_sql];
						string type_of_dobj = dobj.GetType().Name;
						if (dobj != null && type_of_dobj.Equals(cli.type_cs))
						{
							iObj.GetType().GetProperty(cli.name_struct).SetValue(iObj, dobj, null);
						}
						break;
					}
				}
#if false
				if (iRow.Table.Columns.Contains(cli.name_sql))
				{
					Object dobj = iRow[cli.name_sql];
					string type_of_dobj = dobj.GetType().ToString();
					if (dobj != null && type_of_dobj.Equals(cli.type_cs))
					{
						iObj.GetType().GetProperty(cli.name_struct).SetValue(iObj, dobj, null);
					}
				}
#endif
			}
		}

		public ArrayList execSTD_select(string iTableName, string sql)
		{
			ArrayList r = new ArrayList();

			TableLookupInfo li = LookupRegistry.lookupTable(iTableName);
			if (li != null)
			{

				DataSet ds = mConnectionProvider.FillDataSet(IConnection, (sql == null ? "select * from " + li.name_sql : sql));

				DataTable xxx = ds.Tables[0];
				if (xxx != null)
				{
					for (int i = 0; i < xxx.Rows.Count; i++)
					{
						Object iStruct = Activator.CreateInstance(li.type);
						r.Add(iStruct);
						mapEntry(li, xxx.Rows[i], iStruct);
					}
				}
				ds.Clear();
			}
			return r;
		}

		public int execSTD_select(ATableStruct iStruct, string sql)
		{
			int r = -1;

			TableLookupInfo li = LookupRegistry.lookupTable(iStruct.TableName());
			if (li != null)
			{
				DataSet ds = mConnectionProvider.FillDataSet(IConnection, (sql == null ? "select * from " + li.name_sql : sql));

				DataTable xxx = ds.Tables[0];
				if (xxx != null)
				{
					r = xxx.Rows.Count;
					for (int i = 0; i < r; i++)
					{
						mapEntry(li, xxx.Rows[i], iStruct);
					}
				}
				ds.Clear();
			}
			return r;
		}

		IDbCommand prepareSTD_insert(TableLookupInfo iTableinfo)
		{
			IDbCommand r = mConnectionProvider.createCommand();
			r.Connection = IConnection;
			IDbDataParameter aParam;

			string partCols = "";
			string partParms = "";
			foreach (ColumnLookupInfo ci in iTableinfo.Columns)
			{
				partCols += ci.name_sql + ",";
				partParms += (Provider == ConnectionPool.ProviderType.SqlServer ? "@" : ":") + ci.name_sql + ",";
				aParam = mConnectionProvider.createParameter();
				initParam(aParam, ci.name_sql, ci.type_cs, null);
				if (aParam.DbType == DbType.String)
				{
					aParam.Size = 4000;	// dummy length für prepare...
				}
				r.Parameters.Add(aParam);
			}
			partCols = partCols.TrimEnd(',');
			partParms = partParms.TrimEnd(',');
			r.CommandText = string.Format("insert into {0} ({1}) values ({2})", iTableinfo.name_sql, partCols, partParms);
			r.Prepare();

			return r;
		}

		/// <summary>
		/// Einfügen einer STD Struktur in die zugehörige Tabelle
		/// </summary>
		/// <param name="iValue">Die befüllte Struktur</param>
		/// <returns>Immer 1</returns>
		public int execSTD_insert(ATableStruct iValue)
		{
			TableLookupInfo ti = LookupRegistry.lookupTable(iValue.TableName());
			IDbCommand cmd = prepareSTD_insert(ti);

			int idxParam = 0;
			foreach (ColumnLookupInfo ci in ti.Columns)
			{
				IDbDataParameter aParam = ((IDbDataParameter)cmd.Parameters[idxParam]);
				object aValue = iValue.GetType().GetProperty(ci.name_struct).GetValue(iValue, null);
				if (aValue == null && aParam.DbType == DbType.String) aValue = "";	// HACK!!!
				aParam.Value = aValue;
				idxParam++;
			}

			cmd.ExecuteNonQuery();
			cmd.Dispose();

			return 1;
		}

		/// <summary>
		/// Einfügen einer Liste von befüllten Tabellenstrukturen in die zugehörige Tabelle
		/// Alle Einträge der Liste müssen vom selben Typ sein!
		/// </summary>
		/// <param name="iArray">Die Liste mit den Tabellenstrukturen</param>
		/// <returns>Anzahl der eingefügten Werte</returns>
		public int execSTD_insert(ArrayList iArray /* ATableStruct */)
		{
			int r = -1;

			if (iArray != null && iArray.Count > 0)
			{
				if (iArray[0] is ATableStruct)
				{
					TableLookupInfo ti = LookupRegistry.lookupTable(((ATableStruct)iArray[0]).TableName());
					IDbCommand cmd = prepareSTD_insert(ti);

					foreach (ATableStruct aStruct in iArray)
					{
						int idxParam = 0;
						foreach (ColumnLookupInfo ci in ti.Columns)
						{
							IDbDataParameter aParam = ((IDbDataParameter)cmd.Parameters[idxParam]);
							object aValue = aStruct.GetType().GetProperty(ci.name_struct).GetValue(aStruct, null);
							aParam.Value = aValue;
							idxParam++;
						}
						cmd.ExecuteNonQuery();
						if (r < 0) r = 1; else r++;
					}
					cmd.Dispose();
				}
			}
			return r;
		}

		#endregion Standard Record Interface

	}
}
