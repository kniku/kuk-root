using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace dbigen.classes.impl_generator
{
	class CGenSql : i_generator
	{
		StreamWriter writer;

		void generate_postgres(CAppGlobal iAppGlobal, CGenerator iGenerator, classes.dbi_nts_tool iNTS)
		{
			foreach (classes.i_dbi_type aEntry in iAppGlobal.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).Values)
			{
				classes.CTable aTable = (classes.CTable)aEntry;
				classes.CStructure aStruct = (classes.CStructure)iAppGlobal.lookup.lookupDbiType(dbi_type.STRUCT, aTable.reftype);
				string tabledef = null;

				switch (iGenerator.mode)
				{
					case "create":
						tabledef = string.Format("create table {0} (\n", aTable.getName().ToLower());
						foreach (classes.CStructMember aMember in aStruct.arrayMember)
						{
							classes.CType aType = (classes.CType)iAppGlobal.lookup.lookupDbiType(dbi_type.TYPE, aMember.reftype);
							string sqltype = "???";
							string sqldefault = null;
							switch (aType.type)
							{
								case CType.basetype.STRING:
									sqltype = "character varying";
									if (aType.precision > 0)
									{
										sqltype = sqltype + "(" + aType.precision + ")";
									}
									if (aMember.sql_default != null)
									{
										sqldefault = " default '" + aMember.sql_default + "'";
									}
									break;
								case CType.basetype._SMALL:
									sqltype = "smallint";
									if (aMember.sql_default != null)
									{
										sqldefault = " default " + aMember.sql_default;
									}
									break;
								case CType.basetype.INT:
									sqltype = "integer";
									if (aMember.sql_default != null)
									{
										sqldefault = " default " + aMember.sql_default;
									}
									break;
								case CType.basetype.DOUBLE:
									sqltype = "double precision";
									if (aMember.sql_default != null)
									{
										sqldefault = " default " + aMember.sql_default;
									}
									break;
								case CType.basetype.NUMERIC:
									sqltype = string.Format("numeric({0},{1})",
										aType.precision > 0 ? aType.precision.ToString() : "10",
										aType.scale > 0 ? aType.scale.ToString() : "0");
									if (aMember.sql_default != null)
									{
										sqldefault = " default " + aMember.sql_default;
									}
									break;
								case CType.basetype.DATETIME:
									sqltype = "timestamp without time zone";
									if (aMember.sql_default != null)
									{
										sqldefault = iNTS.NTS2SQL(aMember.sql_default);
										if (sqldefault != null)
										{
											sqldefault = " default '" + sqldefault + "'";
										}
										else
										{
											iAppGlobal.log.LogError("Ungültiger default timestamp: {0}", aMember.sql_default);
										}
									}
									break;
								case CType.basetype.AUTO:
									sqltype = "serial";
									break;
								case CType.basetype.GUID:
									sqltype = "TODO";
									break;
							}

							string isNull = aMember.isNull ? "" : " not null";
							if (aMember.isNull)
							{
								foreach (string key in aTable.arrayKeyColumns)
								{
									if (key.ToLower().Equals(aMember.name.ToLower()))
									{
										isNull = " not null";
										break;
									}
								}
							}
							tabledef = tabledef + string.Format("	{0} {1}{2}{3},\n", aMember.name.ToLower(), sqltype, isNull, sqldefault == null ? "" : sqldefault);
						}

						if (aTable.arrayKeyColumns.Count > 0)
						{
							int cnt = 0;
							tabledef = tabledef + string.Format("	CONSTRAINT pk_{0} PRIMARY KEY (", aTable.getName().ToLower());
							foreach (string key in aTable.arrayKeyColumns)
							{
								tabledef = tabledef + string.Format("{0}{1}", cnt == 0 ? "" : ",", key.ToLower());
								cnt++;
							}
							tabledef += ")";
						}
						else
						{
							tabledef = tabledef.TrimEnd('\n', ',');
						}
						tabledef += "\n);\n\n";
						break;

					case "drop":
						tabledef = string.Format("drop table {0};\n", aTable.getName().ToLower());
						break;

					default:
						iAppGlobal.log.LogError("Ungültiger Modus: {0}", iGenerator.mode);
						break;
				}

				if (tabledef != null) writer.Write(tabledef);
			}

			// zweiter durchlauf wegen der constraints (foreign keys)
			if (iGenerator.mode == "create")
			{
				writer.Write("\n/* ***** foreign key constraints ***** */\n\n");
				foreach (classes.i_dbi_type aEntry in iAppGlobal.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).Values)
				{
					classes.CTable aTable = (classes.CTable)aEntry;

					foreach (CForKey aForKey in aTable.arrayForKeys)
					{
						if (!iAppGlobal.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).ContainsKey(aForKey.for_table))
						{
							iAppGlobal.log.LogWarning("foreign key (table {0}): unknown table found: {1}", aTable.getName(), aForKey.for_table);
						}
						string sql = string.Format("alter table {0} add foreign key ({1}) references {2} ({3});\n",
							aTable.getName(), aForKey.cols, aForKey.for_table, aForKey.for_cols);
						
						writer.Write(sql);
					}
				}
			}
		}

		void generate_mssql(CAppGlobal iAppGlobal, CGenerator iGenerator, classes.dbi_nts_tool iNTS)
		{
			writer.Write("SET IMPLICIT_TRANSACTIONS OFF;\n\n");

			foreach (classes.i_dbi_type aEntry in iAppGlobal.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).Values)
			{
				classes.CTable aTable = (classes.CTable)aEntry;
				classes.CStructure aStruct = (classes.CStructure)iAppGlobal.lookup.lookupDbiType(dbi_type.STRUCT, aTable.reftype);

				string tabledef = null;

				switch (iGenerator.mode)
				{
					case "create":
						tabledef = string.Format("create table {0} (\n", aTable.getName().ToLower());
						foreach (classes.CStructMember aMember in aStruct.arrayMember)
						{
							classes.CType aType = (classes.CType)iAppGlobal.lookup.lookupDbiType(dbi_type.TYPE, aMember.reftype);
							string sqltype = "???";
							string sqldefault = null;
							switch (aType.type)
							{
								case CType.basetype.STRING:
									sqltype = "nvarchar";
									if (aType.precision > 0)
									{
										sqltype = sqltype + "(" + aType.precision + ")";
									}
									else
									{
										//sqltype = sqltype + "(max)";
										sqltype = sqltype + "(4000)";
									}
									if (aMember.sql_default != null)
									{
										sqldefault = " default '" + aMember.sql_default + "'";
									}
									break;
								case CType.basetype.NUMERIC:
									sqltype = string.Format("numeric({0},{1})",
										aType.precision > 0 ? aType.precision.ToString() : "10",
										aType.scale > 0 ? aType.scale.ToString() : "0");
									if (aMember.sql_default != null)
									{
										sqldefault = " default " + aMember.sql_default;
									}
									break;
								case CType.basetype.DATETIME:
									sqltype = "datetime";
									if (aMember.sql_default != null)
									{
										sqldefault = iNTS.NTS2SQL(aMember.sql_default);
										if (sqldefault != null)
										{
											sqldefault = " default '" + sqldefault + "'";
										}
										else
										{
											iAppGlobal.log.LogError("Ungültiger default timestamp: {0}", aMember.sql_default);
										}
									}
									break;
								case CType.basetype.AUTO:
									sqltype = "int IDENTITY(1,1)";
									break;
								case CType.basetype.GUID:
									sqltype = "uniqueidentifier";
									break;
							}
							string isNull = aMember.isNull ? "" : " not null";
							if (aMember.isNull)
							{
								foreach (string key in aTable.arrayKeyColumns)
								{
									if (key.ToLower().Equals(aMember.name.ToLower()))
									{
										isNull = " not null";
										break;
									}
								}
							}
							tabledef = tabledef + string.Format("	{0} {1}{2}{3},\n", aMember.name.ToLower(), sqltype, isNull, sqldefault == null ? "" : sqldefault);
						}

						if (aTable.arrayKeyColumns.Count > 0)
						{
							int cnt = 0;
							tabledef = tabledef + string.Format("	CONSTRAINT pk_{0} PRIMARY KEY (", aTable.getName().ToLower());
							foreach (string key in aTable.arrayKeyColumns)
							{
								tabledef = tabledef + string.Format("{0}{1}", cnt == 0 ? "" : ",", key.ToLower());
								cnt++;
							}
							tabledef += ")";
						}
						else
						{
							tabledef = tabledef.TrimEnd('\n', ',');
						}
						tabledef += "\n);\ngo\n\n";
						break;

					case "drop":
						tabledef = string.Format("drop table {0};\ngo\n", aTable.getName().ToLower());
						break;

					default:
						iAppGlobal.log.LogError("Ungültiger Modus: {0}", iGenerator.mode);
						break;
				}

				if (tabledef != null) writer.Write(tabledef);
			}
		}

		#region i_generator Member

		public void generate(CAppGlobal iAppGlobal, CGenerator iGenerator, StreamWriter iWriter)
		{
			writer = iWriter;
			iAppGlobal.log.pushPrefix("{{GenSql}}");

			switch (iGenerator.dialect)
			{
				case "postgres":
					generate_postgres(iAppGlobal, iGenerator, new classes.dbi_nts_postgres());
					break;
				case "mssql":
					generate_mssql(iAppGlobal, iGenerator, new classes.dbi_nts_mssql());
					break;
				default:
					iAppGlobal.log.LogError("Unbekannter Dialekt: {0}", iGenerator.dialect);
					break;
			}

			iAppGlobal.log.popPrefix();
		}

		#endregion
	}
}
