using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Data.SqlClient;

namespace KLib.Sql
{
	public partial class DbConnection
	{

		#region PRIVATE

		// Fertiges SQL Kommando absetzen
		// iCommand muss mittels prepareSQL(...) erzeugt worden sein
		Object execSQL(IDbCommand iCommand)
		{
			Object r = null;

			if (iCommand != null)
			{
//				Object o;
				if (iCommand.CommandText.StartsWith("select", true, null))
				{
					r = iCommand.ExecuteScalar();
				}
				else
				{
					r = iCommand.ExecuteNonQuery();
				}
			}
			return r;
		}

		#endregion // PRIVATE

		//	Beispiel: execSQL("SELECT count(*) FROM konten");
		/// <summary>
		/// Fertiges SQL Kommando absetzen (ohne Parameter).
		/// Liefert immer die erste Spalte der ersten Trefferzeile.
		/// </summary>
		/// <param name="iSql"></param>
		/// <returns></returns>
		public Object execSQL(string iSql)
		{
			return execSQL(iSql, null);
		}

		//	Beispiel: execSQL("SELECT ktonr FROM konten where id=:2 and curcode=:1", "EUR", 2);
		/// <summary>
		/// Fertiges SQL Kommando absetzen. Parameter werden sauber angebunden.
		/// Liefert immer die erste Spalte der ersten Trefferzeile.
		/// </summary>
		/// <param name="iSql"></param>
		/// <param name="iParams"></param>
		/// <returns></returns>
		public Object execSQL(string iSql, params object[] iParams)
		{
			return execSQL(prepareSQL(iSql, iParams));
		}


		//	Beispiel:
		//	IEnumerator<DataRow> List = conn.execSQL_select("select * from config");
		//	while (List.MoveNext())
		//	{
		//		object Value = List.Current["grp"];	// access by name
		//		Value = List.Current[1];		// access by index, etc...
		//	}
		/// <summary>
		/// Allgemeines SELECT Statement absetzen.
		/// Ergebnisse werden ueber DataRow Eintraege zurueckgeliefert.
		/// </summary>
		/// <param name="iSql">SQL Statement</param>
		/// <param name="iSqlParams">Die angebundenen Variablen</param>
		/// <returns>Enumerator der Ergebniszeilen</returns>
		public IEnumerator<DataRow> execSQL_select(string iSql, params object[] iSqlParams)
		{
			IEnumerator<DataRow> r = null;

			IDataAdapter da = createDataAdapter(prepareSQL(iSql, iSqlParams));
			if (da != null)
			{
				DataSet ds = new DataSet();

				da.Fill(ds);

				if (ds.Tables[0] != null)
				{
					r = ds.Tables[0].Rows.GetEnumerator() as IEnumerator<DataRow>;
				}
			}

			return r;
		}
	}
}
