using System;
using System.Collections;

namespace knk.DBTools
{
	public class ColumnLookupInfo
	{
		public string name_struct { get; private set; }
		public string name_sql { get; private set; }
		public string type_cs { get; private set; }
		public bool isAuto { get; set; }

		public ColumnLookupInfo(string iname_struct, string iname_sql, string itype_cs)
		{
			name_struct = iname_struct;
			name_sql = (iname_sql == null ? iname_struct : iname_sql);
			type_cs = itype_cs;
		}
	}

	public class TableLookupInfo
	{
		private Hashtable mColumns = new Hashtable();
		public string name_struct { get; private set; }
		public string name_sql { get; private set; }
		public Type type { get; set; }

		public TableLookupInfo(string iname_struct, string iname_sql)
		{
			name_struct = iname_struct;
			name_sql = (iname_sql == null ? iname_struct : iname_sql);
		}

		public void addColumnSpec(ColumnLookupInfo iColSpec)
		{
			mColumns.Add(iColSpec.name_sql, iColSpec);
		}

		public ICollection Columns { get { return mColumns.Values; } }
	}

	public static class LookupRegistry
	{

		static Hashtable TableRegistry = new Hashtable();

		public static ICollection Tables { get { return TableRegistry.Values; } }

		public static TableLookupInfo lookupTable(string iTableName)
		{
			return (TableLookupInfo)TableRegistry[iTableName.ToUpper()];
		}

/*		public static string lookupColumnListSQL(string iTablename, bool iQualified)
		{
			string r = null;
			TableLookupInfo ti = lookupTable(iTablename);
			if (ti != null)
			{
				foreach (ColumnLookupInfo ci in ti.Columns)
				{
					if (r == null) r = "";
					else r += ",";
					if (iQualified) r += (ti.name_sql + ".");
					r += ci.name_sql;
				}
			}
			return r;
		}
*/
/*		public static ArrayList lookupColumnArrayListSQL(string iTablename, bool iQualified)
		{
			ArrayList r = new ArrayList();
			TableLookupInfo ti = lookupTable(iTablename);
			if (ti != null)
			{
				foreach (ColumnLookupInfo ci in ti.Columns)
				{
					if (iQualified) r.Add(ti.name_sql + "." + ci.name_sql);
					else r.Add(ci.name_sql);
				}
			}
			return r;
		}
*/
		public static string getTableNameFromAnnotations(Type iTableStructType)
		{
			string r = null;
			if (iTableStructType.IsSubclassOf(typeof(ATableStruct)))
			{
				// Bestimme Tabellenname
				r = iTableStructType.Name;
				object[] attributes = iTableStructType.GetCustomAttributes(typeof(SQLTABLE), false);
				if (attributes != null && attributes.Length > 0)
				{
					SQLTABLE tabInfo = (SQLTABLE)attributes[0];
					if (tabInfo.name != null) r = tabInfo.name;
				}
			}
			return r;
		}

		public static void registerTable(Type iTableStructType)
		{
			int cnt_cols = 0;
			if (iTableStructType.IsSubclassOf(typeof(ATableStruct)))
			{
				string sName = iTableStructType.Name;
				object[] attributes = iTableStructType.GetCustomAttributes(typeof(SQLTABLE), false);
				if (attributes == null || attributes.Length == 0)
				{
					throw new Exception("can't register class \"" + sName + "\" as table! attribute SQL not present.");
				}

				// Bestimme Tabellenname
				string tName = getTableNameFromAnnotations(iTableStructType);

				TableLookupInfo tInfo = new TableLookupInfo(sName, tName.ToUpper());

				System.Reflection.MemberInfo[] arrmi = iTableStructType.GetProperties();
				foreach (System.Reflection.MemberInfo mi in arrmi)
				{
					attributes = mi.GetCustomAttributes(typeof(SQLCOLUMN), false);
					if (attributes != null && attributes.Length > 0)
					{
						cnt_cols++;
						SQLCOLUMN colInfo = (SQLCOLUMN)attributes[0];
						string cName = (colInfo.name == null ? mi.Name : colInfo.name);
						ColumnLookupInfo cInfo = new ColumnLookupInfo(mi.Name, cName.ToUpper(), iTableStructType.GetProperty(mi.Name).PropertyType.Name);
						cInfo.isAuto = colInfo.auto;
						tInfo.addColumnSpec(cInfo);
						//						System.Console.WriteLine(mi.Name + ": " + iTableStructType.GetProperty(mi.Name).PropertyType + "; SQL: " + cName);
					}
				}
				if (cnt_cols > 0 && !TableRegistry.ContainsKey(tInfo.name_sql))
				{
					tInfo.type = iTableStructType;
					TableRegistry.Add(tInfo.name_sql, tInfo);
				}
				else
				{
					Console.Error.WriteLine("can't register class \"" + sName + "\": no columns or tablename already registerd!");
				}
			}
		}
	}
}
