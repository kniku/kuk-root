using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace dbigen.classes.impl_generator
{
	class CGenCSharp : i_generator
	{
		StreamWriter writer;

		#region i_generator Member

		public void generate(CAppGlobal iAppGlobal, CGenerator iGenerator, StreamWriter iWriter)
		{
			classes.CTable aTable;
			classes.CStructure aStruct;

			writer = iWriter;
			iAppGlobal.log.pushPrefix("{{GenC#}}");

			string str = string.Format("// DBIGEN-generated file: DON'T edit!!!\n" +
				"using System;\n" +
				"using knk.DBTools;\n" +
				"\nnamespace {0}\n{{\n", iAppGlobal.parser.CurrentModule.getName());
			writer.Write(str);

			str =	"	public static class DBIClassManager\n" +
					"	{\n" +
					"		public static void RegisterClasses()\n" +
					"		{\n";
			
			// table loop
			foreach (classes.i_dbi_type aEntry in iAppGlobal.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).Values)
			{
				aTable = (classes.CTable)aEntry;
//				aStruct = (classes.CStructure)iAppGlobal.lookup.lookupDbiType(dbi_type.STRUCT, aTable.reftype);
				str += string.Format("			LookupRegistry.registerTable(typeof({0}));\n", aTable.getName());
			}

			str += 	"		}\n" +
					"	}\n";
			writer.Write(str);

			// table loop
			foreach (classes.i_dbi_type aEntry in iAppGlobal.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).Values)
			{
				aTable = (classes.CTable)aEntry;
				aStruct = (classes.CStructure)iAppGlobal.lookup.lookupDbiType(dbi_type.STRUCT, aTable.reftype);
				str = string.Format("\n\t[SQLTABLE(name=\"{0}\")] // {1}\n\tpublic class {2}: ATableStruct\n\t{{", aTable.getName().ToLower(), aTable.descr, aTable.getName());
				foreach (object o in aStruct.arrayMember)
				{
					classes.CStructMember aMember = (classes.CStructMember)o;
					classes.CType aType = (classes.CType)iAppGlobal.lookup.lookupDbiType(dbi_type.TYPE, aMember.reftype);

					str += string.Format("\n\t\t[SQLCOLUMN(name=\"{0}\"{1})]", aMember.name.ToLower(), aType.type == CType.basetype.AUTO ? ",auto=true" : "");
					str += "\n\t\tpublic ";

					switch (aType.type)
					{
						case CType.basetype.STRING:
                            str += string.Format("string {0} {{ get; set; }}\t// {2}; len.:{1}", aMember.name, aType.precision , aMember.descr);
							break;
						case CType.basetype.NUMERIC:
							if (aType.scale > 0)
							{
								str += string.Format("decimal {0} {{ get; set; }}\t// {3}; prec.:{1}.{2}", aMember.name, aType.precision, aType.scale, aMember.descr);
							}
							else
							{
								str += string.Format("long {0} {{ get; set; }}\t// {2}; prec.:{1}", aMember.name, aType.precision, aMember.descr);
							}
							break;
						case CType.basetype.DATETIME:
							str += string.Format("DateTime {0} {{ get; set; }}\t// {1};", aMember.name, aMember.descr);
							break;
						case CType.basetype.AUTO:
							str += string.Format("ulong {0} {{ get; set; }}\t// {1}; SERIAL", aMember.name, aMember.descr);
							break;
						case CType.basetype.GUID:
							str += string.Format("Guid {0} {{ get; set; }}\t// {1}; GUID", aMember.name, aMember.descr);
							break;
					}
				}
				str += string.Format("\n\t}}");
				writer.Write(str);
			}
			writer.Write("\n}");

			iAppGlobal.log.popPrefix();
		}

		#endregion
	}
}
