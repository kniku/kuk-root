using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using knk.shared.tools;

namespace dbigen.classes
{
	// unterstützte Generatoren
	public enum eGenerators
	{
		[StringValue("none")] GenNONE,
		[StringValue("GenSql")] GenSql,
		[StringValue("GenC++")] GenCPP,
		[StringValue("GenC#")] GenCS
	}

	interface i_generator
	{
		void generate(CAppGlobal iAppGlobal, CGenerator iGenerator, StreamWriter iWriter);
	}

	class CGenerator : i_dbi_type
	{
		string name;
		public string dialect;	// sql dialekt
		public string mode;		// untermodus
		public string target;	// zieldatei

		public CGenerator(string iName)
		{
			name = iName;
		}

		public void generate(CAppGlobal iAppGlobal)
		{
			iAppGlobal.log.pushPrefix("{{Generator}}");
			iAppGlobal.log.LogInfo("\"{0}\" -> {1}", name, target);

			DirectoryInfo dir = Directory.GetParent(target);
			if (!dir.Exists)
			{
				dir.Create();
			}
			StreamWriter writer = File.CreateText(target);

			if (StringEnum.IsStringDefined(typeof(eGenerators), name))
			{
				eGenerators aGen = (eGenerators)StringEnum.Parse(typeof(eGenerators), name);
				switch (aGen)
				{
					case eGenerators.GenSql:
						new impl_generator.CGenSql().generate(iAppGlobal, this, writer);
						break;
					case eGenerators.GenCS:
						new impl_generator.CGenCSharp().generate(iAppGlobal, this, writer);
						break;
					default:
						iAppGlobal.log.LogWarning("Nicht implementierter Generator: {0}", name);
						break;
				}
			}
			else
			{
				iAppGlobal.log.LogError("Unbekannter Generator: {0}", name);
			}
			writer.Close();
			iAppGlobal.log.popPrefix();
		}

		#region i_dbi_type Member

		public dbi_type getDbiType()
		{
			return dbi_type.GENERATOR;
		}

		public string getName()
		{
			return name;
		}

		#endregion
	}
}
