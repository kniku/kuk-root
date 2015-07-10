using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Collections;

namespace dbigen
{
	static class tools
	{
		public static string getAttr(CAppGlobal iAppGlobal, XmlTextReader reader, string iAttrName, bool iMandatory)
		{
			string r = reader.GetAttribute(iAttrName);
			if (iMandatory && r == null) {
				iAppGlobal.log.LogError("{0}({1}): Attribut \"{2}\" fehlt!", reader.BaseURI, reader.LineNumber, iAttrName);
			}
			return r;
		}

		internal static void check_attributes(CAppGlobal iAppGlobal, XmlTextReader reader, params string[] iAttrs)
		{
			while (reader.MoveToNextAttribute())
			{
				bool found = false;
				string aAttr = reader.Name;

				foreach (string aValidAttr in iAttrs)
				{
					if (aAttr.Equals(aValidAttr))
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					iAppGlobal.log.LogError("{0}({1}): Ungültiges Attribut: \"{2}\"!", reader.BaseURI, reader.LineNumber, aAttr);
				}
			}
			reader.MoveToElement();
		}
	}

	interface element_parser
	{
		string getName();
		void parse(XmlTextReader reader, CAppGlobal iAppGlobal);
	}

	class parse_module : element_parser
	{
		string name;
		public string getName()
		{
			return name;
		}

		public parse_module(string iName)
		{
			name = iName;
		}

		public void parse(XmlTextReader reader, CAppGlobal iAppGlobal)
		{
//			iAppGlobal.log.LogDebug("MODULE: {0}-{1}:{2}:{3}", reader.Depth, reader.NodeType, reader.Name, reader.Value);
			if (reader.NodeType == XmlNodeType.Element)
			{
				if (iAppGlobal.parser.CurrentModule != null)
				{
					iAppGlobal.log.LogWarning("{0}({1}): Verschachteltes Modul: \"{2}\" - ignoriert!", reader.BaseURI, reader.LineNumber, reader.Name);
				}
				else
				{
					string name = reader.GetAttribute("Name");
					if (name == null) name = "dbigen";	// default module
					iAppGlobal.parser.CurrentModule = new dbigen.classes.CModule(name);
				}
			}
/*			if (reader.NodeType == XmlNodeType.EndElement || (reader.NodeType == XmlNodeType.Element && reader.IsEmptyElement))
			{
				iAppGlobal.parser.CurrentModule = null;
			}
*/		}
	}

	class parse_generator : element_parser
	{
		string name;
		public string getName()
		{
			return name;
		}
		public parse_generator(string iName)
		{
			name = iName;
		}
		public void parse(XmlTextReader reader, CAppGlobal iAppGlobal)
		{
//			iAppGlobal.log.LogDebug("GENERATOR: {0}-{1}:{2}:{3}", reader.Depth, reader.NodeType, reader.Name, reader.Value);
			if (reader.NodeType == XmlNodeType.Element)
			{
				tools.check_attributes(iAppGlobal, reader, "Name", "Dialect", "Mode", "Target");

				string name = tools.getAttr(iAppGlobal, reader, "Name", true);
				if (name != null)
				{
					classes.CGenerator generator = new dbigen.classes.CGenerator(name);
					generator.dialect = tools.getAttr(iAppGlobal, reader, "Dialect", false);
					generator.mode = tools.getAttr(iAppGlobal, reader, "Mode", false);
					generator.target = iAppGlobal.parser.base_dir + "\\" + tools.getAttr(iAppGlobal, reader, "Target", true);
					iAppGlobal.parser.GeneratorStack.Add(generator);
				}
			}
		}
	}

	class parse_type : element_parser
	{
		classes.CType type;
		bool inDefault = false;
		string name;
		public string getName()
		{
			return name;
		}
		public parse_type(string iName)
		{
			name = iName;
		}
		public void parse(XmlTextReader reader, CAppGlobal iAppGlobal)
		{
			switch (reader.NodeType)
			{
				case XmlNodeType.Element:
					switch (reader.Name)
					{
						case "Type":
							iAppGlobal.log.pushPrefix("{{TYPE}} ");
							type = new dbigen.classes.CType(tools.getAttr(iAppGlobal, reader, "Name", true));
							type.descr = tools.getAttr(iAppGlobal, reader, "Descr", false);
							string s = tools.getAttr(iAppGlobal, reader, "Type", false);
							switch (s)
							{
								case "string":
									type.type = classes.CType.basetype.STRING;
									break;
								case "int2":
									type.type = classes.CType.basetype._SMALL;
									break;
								case "int":
									type.type = classes.CType.basetype.INT;
									break;
								case "double":
									type.type = classes.CType.basetype.DOUBLE;
									break;
								case "numeric":
									type.type = classes.CType.basetype.NUMERIC;
									break;
								case "datetime":
									type.type = classes.CType.basetype.DATETIME;
									break;
								case "auto":
									type.type = classes.CType.basetype.AUTO;
									break;
								case "guid":
									type.type = classes.CType.basetype.GUID;
									break;
								default:
									iAppGlobal.log.LogError("{0}({1}): Unbekannter Basistyp: \"{2}\"!", reader.BaseURI, reader.LineNumber, s);
									break;
							}
							tools.check_attributes(iAppGlobal, reader, "Name", "Descr", "Type",
								type.type == dbigen.classes.CType.basetype.STRING ? "Length" : null,
								type.type == dbigen.classes.CType.basetype.NUMERIC ? "Scale" : null,
								type.type == dbigen.classes.CType.basetype.NUMERIC ? "Precision" : null
								);
							s = tools.getAttr(iAppGlobal, reader, "Length", false);
							if (s != null) type.precision = int.Parse(s);
							s = tools.getAttr(iAppGlobal, reader, "Precision", false);
							if (s != null) type.precision = int.Parse(s);
							s = tools.getAttr(iAppGlobal, reader, "Scale", false);
							if (s != null) type.scale = int.Parse(s);
							if (type.getName() != null) iAppGlobal.lookup.addDbiType(type);
							break;
						case "Default":
							inDefault = true;
							break;
						default:
							iAppGlobal.log.LogWarning("Unbekanntes Unterelement: {0}", reader.Name);
							break;
					}
					break;
				case XmlNodeType.Text:
					if (inDefault)
					{
						type.sql_default = reader.Value;
						inDefault = false;
					}
					break;
			}
			if (reader.Name.Equals("Type") && (reader.NodeType == XmlNodeType.EndElement || (reader.NodeType == XmlNodeType.Element && reader.IsEmptyElement)))
			{
				iAppGlobal.log.popPrefix();
			}
		}
	}

	class parse_struct : element_parser
	{
		classes.CStructure currentStructure;
		string name;
		classes.CStructMember currentMember = null;
		bool inDefault = false;

		public string getName()
		{
			return name;
		}
		public parse_struct(string iName)
		{
			name = iName;
		}

		public void parse(XmlTextReader reader, CAppGlobal iAppGlobal)
		{
			string s;
			switch (reader.NodeType)
			{
				case XmlNodeType.Element:
					switch (reader.Name)
					{
						case "Structure":
							iAppGlobal.log.pushPrefix("{{STRUCT}} ");
							currentStructure = new dbigen.classes.CStructure(tools.getAttr(iAppGlobal, reader, "Name", true));
							currentStructure.descr = tools.getAttr(iAppGlobal, reader, "Descr", false);
							if (currentStructure.getName() != null) iAppGlobal.lookup.addDbiType(currentStructure);
							break;
						case "Member":
							currentMember = new dbigen.classes.CStructMember(tools.getAttr(iAppGlobal, reader, "Name", true));
							if (currentStructure.hashMember.ContainsKey(currentMember.name))
							{
								iAppGlobal.log.LogError("{0}({1}): Member doppelt in der Struktur: \"{2}\"!", reader.BaseURI, reader.LineNumber, currentMember.name);
							}
							currentStructure.addMember(currentMember);
							currentMember.descr = tools.getAttr(iAppGlobal, reader, "Descr", false);
							currentMember.reftype = tools.getAttr(iAppGlobal, reader, "RefType", true);
							classes.CType basetype = (classes.CType)iAppGlobal.lookup.lookupDbiType(dbigen.classes.dbi_type.TYPE, currentMember.reftype);
							if (basetype == null)
							{
								iAppGlobal.log.LogError("{0}({1}): Unbekannter Basistyp: \"{2}\"!", reader.BaseURI, reader.LineNumber, currentMember.reftype);
							}
							else
							{
								currentMember.sql_default = basetype.sql_default;
							}
							s = tools.getAttr(iAppGlobal, reader, "Null", false);
							if (s == null || s.Equals("true", StringComparison.CurrentCultureIgnoreCase)) currentMember.isNull = true;
							break;
						case "Default":
							inDefault = true;
							break;
						default:
							iAppGlobal.log.LogWarning("Unbekanntes Unterelement: {0}", reader.Name);
							break;
					}
					break;
				case XmlNodeType.Text:
					if (inDefault)
					{
						currentMember.sql_default = reader.Value;
						inDefault = false;
					}
					break;
			}
			if (reader.NodeType == XmlNodeType.EndElement || (reader.NodeType == XmlNodeType.Element && reader.IsEmptyElement))
			{
				switch (reader.Name)
				{
					case "Structure":
						iAppGlobal.log.popPrefix();
						break;
					case "Member":
						currentMember = null;
						break;
				}
			}
		}
	}

	class parse_table : element_parser
	{
		classes.CTable currentTable;
		string name;
		classes.CStructure currentStruct;

		public string getName()
		{
			return name;
		}
		public parse_table(string iName)
		{
			name = iName;
		}

		public void parse(XmlTextReader reader, CAppGlobal iAppGlobal)
		{
			switch (reader.NodeType)
			{
				case XmlNodeType.Element:
					switch (reader.Name)
					{
						case "Table":
							iAppGlobal.log.pushPrefix("{{TABLE}} ");
							currentTable = new dbigen.classes.CTable(tools.getAttr(iAppGlobal, reader, "Name", true));
							currentTable.descr = tools.getAttr(iAppGlobal, reader, "Descr", false);
							currentTable.reftype = tools.getAttr(iAppGlobal, reader, "RefType", true);
							currentStruct = (classes.CStructure)iAppGlobal.lookup.lookupDbiType(dbigen.classes.dbi_type.STRUCT, currentTable.reftype);
							if (currentStruct == null)
							{
								iAppGlobal.log.LogError("{0}({1}): Unbekannte Basisstruktur: \"{2}\"!", reader.BaseURI, reader.LineNumber, currentTable.reftype);
							}
							if (currentTable.getName() != null) iAppGlobal.lookup.addDbiType(currentTable);

							string primKey = tools.getAttr(iAppGlobal, reader, "PrimKey", false);
							if (!string.IsNullOrEmpty(primKey))
							{
								string[] cols = primKey.Split(',');
								foreach (string aCol in cols)
								{
									string col = aCol.Trim();
									if (!currentStruct.hashMember.ContainsKey(col))
									{
										iAppGlobal.log.LogError("{0}({1}): Unbekannte Spalte: \"{2}\"!", reader.BaseURI, reader.LineNumber, col);
									}
									currentTable.arrayKeyColumns.Add(col);
								}
							}
							break;
						case "ForKey":
							currentTable.arrayForKeys.Add(new dbigen.classes.CForKey() { cols = tools.getAttr(iAppGlobal, reader, "Column", true), for_table = tools.getAttr(iAppGlobal, reader, "ForTable", true), for_cols = tools.getAttr(iAppGlobal, reader, "ForColumn", true) });
							break;
						default:
							iAppGlobal.log.LogWarning("Unbekanntes Unterelement: {0}", reader.Name);
							break;
					}
					break;
				case XmlNodeType.Text:
					//if (inKey)
					//{
					//	string [] cols = reader.Value.Split(',');
					//	foreach (string aCol in cols)
					//	{
					//		string col = aCol.Trim();
					//		if (!currentStruct.hashMember.ContainsKey(col))
					//		{
					//			iAppGlobal.log.LogError("{0}({1}): Unbekannte Spalte: \"{2}\"!", reader.BaseURI, reader.LineNumber, col);
					//		}
					//		currentTable.arrayKeyColumns.Add(col);
					//	}
					//}
					break;
			}
			if (reader.NodeType == XmlNodeType.EndElement || (reader.NodeType == XmlNodeType.Element && reader.IsEmptyElement))
			{
				switch (reader.Name)
				{
					case "Table":
						iAppGlobal.log.popPrefix();
						break;
				}
			}
		}
	}

	class CXMLParser
	{
		public string base_dir = null;
		ArrayList parser_stack = new ArrayList();
		ArrayList generator_stack = new ArrayList();
		public ArrayList GeneratorStack
		{
			get { return generator_stack; }
			set { generator_stack = value; }
		}
		dbigen.classes.CModule currentModule;

		internal dbigen.classes.CModule CurrentModule
		{
			get { return currentModule; }
			set { currentModule = value; }
		}

		public void parseFile(CAppGlobal iAppGlobal, string iFile)
		{
			if (base_dir != null) iFile = base_dir + "\\" + iFile;
			FileInfo f = new FileInfo(iFile);
//			DirectoryInfo dir = f.Directory;
			if (base_dir == null) base_dir = f.Directory.FullName;
	
			XmlTextReader reader = new XmlTextReader(f.FullName);
			XmlWriterSettings ws = new XmlWriterSettings();
			ws.Indent = true;

			try
			{
				while (reader.Read())
				{
					if (!"Whitespace".Equals(reader.NodeType.ToString()))
					{
						iAppGlobal.log.LogDebug("depth={0};type={1};name={2};value={3};empty={4}", reader.Depth, reader.NodeType, reader.Name, reader.Value, reader.IsEmptyElement);
					}
					switch (reader.NodeType)
					{
						case XmlNodeType.Element:
							switch (reader.Name)
							{
								case "Generator":
									parser_stack.Add(new parse_generator(reader.Name));
									break;
								case "Module":
									parser_stack.Add(new parse_module(reader.Name));
									break;
								case "Type":
									parser_stack.Add(new parse_type(reader.Name));
									break;
								case "Structure":
									parser_stack.Add(new parse_struct(reader.Name));
									break;
								case "Table":
									parser_stack.Add(new parse_table(reader.Name));
									break;
								case "DBI_DATA_DESCRIPTION":
									continue;
								case "Import":
									string file = reader.GetAttribute("File");
									if (file == null)
									{
										iAppGlobal.log.LogError("{0}({1}): Import ohne File!", iFile, reader.LineNumber);
									}
									else
									{
										iAppGlobal.parser.parseFile(iAppGlobal, file);
									}
									continue;
								case "Sql":
								case "PrimKey":
								case "Column":
								case "Index":
								case "ForKey":
								case "ForTable":
								case "Member":
								case "Default":
									break;
								default:
									iAppGlobal.log.LogError("{0}({1}): Nicht unterstützes Element: {2}", iFile, reader.LineNumber, reader.Name);
									break;
							}
							break;
						case XmlNodeType.Comment:
						case XmlNodeType.Whitespace:
						case XmlNodeType.XmlDeclaration:
							continue;
					}
					if (parser_stack.Count > 0)
					{
						element_parser parser = ((element_parser)parser_stack[parser_stack.Count - 1]);

						try
						{
							parser.parse(reader, iAppGlobal);
						}
						finally
						{
							if ((reader.NodeType == XmlNodeType.EndElement || reader.IsEmptyElement) && reader.Name.Equals(parser.getName()))
							{
								parser_stack.RemoveAt(parser_stack.Count - 1);
							}
						}
					}
				}
			}
			catch (Exception e)
			{
				// just catch it ...
				iAppGlobal.log.LogError("{1} - parse error: {0}", e.Message, iFile);
			}
		}

		public void generate(CAppGlobal iAppGlobal)
		{
			foreach (classes.CGenerator aGenerator in GeneratorStack)
			{
//				iAppGlobal.log.LogInfo("generator: {0}:{1}-{2}", aGenerator.getName(), aGenerator.mode, aGenerator.target);
				aGenerator.generate(iAppGlobal);
			}
		}
	}
}
