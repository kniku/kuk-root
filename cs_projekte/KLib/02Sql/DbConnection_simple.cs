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

		// Fertiges SQL Kommando (ohne Parameter) absetzen
		public Object execSQL(string iSql)
		{
			return execSQL(iSql, null);
		}

		// Fertiges SQL Kommando absetzen
		// Parameter werden sauber angebunden
		// Beispiel: execSQL("SELECT ktonr FROM konten where id=:2 and curcode=:1", "EUR", 2);
		// Beispiel: execSQL("SELECT count(*) FROM konten", null);
		public Object execSQL(string iSql, params object[] iParams)
		{
			return execSQL(prepareSQL(iSql, iParams));
		}
	}
}
