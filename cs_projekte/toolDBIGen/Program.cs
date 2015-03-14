using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using knk.shared.logger;

namespace dbigen
{
	class Program
	{
		static void Main(string[] args)
		{

			if (args.Length < 1)
			{
				System.Console.WriteLine("usage: {0} [buildfile]", System.IO.Path.GetFileName(System.Reflection.Assembly.GetExecutingAssembly().Location));
				Environment.Exit(0);
			}

			CAppGlobal ag = new CAppGlobal();
			ag.log.LogInfo("DBIGEN: startup...");
			ag.log.setLogLevel(LogLevel.Info);
			ag.parser.parseFile(ag, args[0]);
			ag.parser.generate(ag);

#if false
			foreach (classes.i_dbi_type aEntry in ag.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TYPE).Values)
			{
				classes.CType aType = (classes.CType)aEntry;
				ag.log.LogInfo("TYPE: name={0};descr={1};type={2};len={3};scale={4};def={5}", aType.getName(), aType.descr, aType.getDbiType(), aType.length, aType.scale, aType.sql_default);
			}
			foreach (classes.i_dbi_type aEntry in ag.lookup.getDbiTypeHash(dbigen.classes.dbi_type.STRUCT).Values)
			{
				classes.CStructure aStruct = (classes.CStructure)aEntry;
				ag.log.LogInfo("STRUCT: name={0};descr={1}", aStruct.getName(), aStruct.descr);
				foreach (object o in aStruct.arrayMember)
				{
					classes.CStructMember aMember = (classes.CStructMember)o;
					ag.log.LogInfo("..MEMBER: name={0};descr={1};ref={2};null={3};def={4}", aMember.name, aMember.descr, aMember.reftype, aMember.isNull, aMember.sql_default);
				}
			}
			foreach (classes.i_dbi_type aEntry in ag.lookup.getDbiTypeHash(dbigen.classes.dbi_type.TABLE).Values)
			{
				classes.CTable aTable = (classes.CTable)aEntry;
				ag.log.LogInfo("TABLE: name={0};descr={1};ref={2}", aTable.getName(), aTable.descr, aTable.reftype);
				foreach (string key in aTable.arrayKeyColumns)
				{
					ag.log.LogInfo("..PK: col={0}", key);
				}
			}
#endif
			ag.log.LogInfo("DBIGEN: done.");
		}
	}
}
