using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace KLib.Sql
{
	public partial class DbConnection
	{
		void mapRow(Object iObj, DataRow iRow)
		{
			PropertyInfo[] propertyInfos;
			propertyInfos = iObj.GetType().GetProperties(BindingFlags.Public|BindingFlags.Instance);

			foreach (PropertyInfo propertyInfo in propertyInfos)
			{
				if (propertyInfo.Name.Length < 8 || !propertyInfo.Name.StartsWith("valsql_") || !propertyInfo.CanWrite) continue;

				string sqlname = propertyInfo.Name.Substring(7);
				Logger.InfoFormat("property {0}", sqlname);

				if (iRow.Table.Columns.Contains(sqlname))
				{
					Object x = iRow[sqlname];
					if (x != null)
					{
						var val = Convert.ChangeType(x, propertyInfo.PropertyType);
						propertyInfo.SetValue(iObj, val);
					}
				}
			}
		}

		public bool execSQL_selectIntoObject(Object iObj, string iSql, params object[] iSqlParams)
		{
			bool r = false;

			IEnumerator<DataRow> List = execSQL_select(iSql, iSqlParams);
			while (List.MoveNext())
			{
				r = true;
				mapRow(iObj, List.Current);
				break;
			}

			return r;
		}

		public bool execSQL_insertFromObject(string iTableName, Object iObj)
		{
			bool r = false;

			object[] vals;
			PropertyInfo[] propertyInfos;
			propertyInfos = iObj.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance);

			string insertSql = "insert into " + iTableName + " (";
			int cnt = 0;
			foreach (PropertyInfo propertyInfo in propertyInfos)
			{
				if (propertyInfo.Name.Length < 8 || !propertyInfo.Name.StartsWith("valsql_") || !propertyInfo.CanWrite) continue;

				string sqlname = propertyInfo.Name.Substring(7);
				Logger.InfoFormat("property {0}", sqlname);

				if (cnt > 0) insertSql += ",";
				insertSql += sqlname;

				vals[cnt] = propertyInfo.GetValue(iObj);
				cnt++;
			}
			insertSql += ") values (";

			for (int i = 1; i <= cnt; i++)
			{
				if (i > 1) insertSql += ",";
				insertSql += ":" + i;
			}
			insertSql += ")";

			execSQL(insertSql, vals);
			return r;
		}
	}
}
