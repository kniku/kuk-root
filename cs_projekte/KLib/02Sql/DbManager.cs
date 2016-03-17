using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace KLib.Sql
{
	// Konfigurieren in der Anwendung:
	//	[assembly: ASDShared.Sql.XmlConfigurator(ConfigFile = "../conf/Database.config")]

	[AttributeUsage(AttributeTargets.Assembly)]
	public class XmlConfiguratorAttribute : Attribute
	{
		public string ConfigFile { get; set; }
	}

	/// <summary>
	/// Verwaltet alle Verbindungsmanager einer Anwendung (Verbindung zu verschiedenen Datenbanken)
	/// </summary>
	public static class DbManager
	{
		static readonly log4net.ILog Logger = log4net.LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
		static string DB_CONFIG_FILENAME = "Database.config";	// Default configuration file

		/// <summary>
		/// Internes Dictionary für die Verbindungsmanager
		/// </summary>
		static Dictionary<string, DbConnectionManager> mDict = new Dictionary<string,DbConnectionManager>();

		static DbManager()
		{
			bool inDatabase = false;
			string dbName = null;
			string dbType = null;

			// Mal schauen, ob es ein Assembly Attribute mit einer Konfiguration gibt.
			// Ggf. damit den default-Konfigfilenamen übersteuern
			System.Reflection.Assembly assembly = System.Reflection.Assembly.GetEntryAssembly();
			System.Attribute[] attrs = System.Attribute.GetCustomAttributes(assembly);
			foreach (Attribute attr in attrs)
			{
				if (attr.GetType() == typeof(XmlConfiguratorAttribute))
				{
					DB_CONFIG_FILENAME = (attr as XmlConfiguratorAttribute).ConfigFile;
					break;
				}
			}

			// Wenn Pfad zur Konfig relativ, den exe-Pfad dazuhängen und Gesamtpfad sauber auflösen...
			if (!Path.IsPathRooted(DB_CONFIG_FILENAME))
			{
				DB_CONFIG_FILENAME = Path.GetDirectoryName(assembly.Location) + "\\" + DB_CONFIG_FILENAME;
			}
			DB_CONFIG_FILENAME = Path.GetFullPath(DB_CONFIG_FILENAME);

			// Konfiguration einlesen
			if (File.Exists(DB_CONFIG_FILENAME))
			{
				XmlTextReader xmlReader = new XmlTextReader(DB_CONFIG_FILENAME);
				while (xmlReader.Read())
				{
					switch (xmlReader.NodeType)
					{
						case XmlNodeType.Element:
							Logger.Debug("<" + xmlReader.Name + ">");
							if (xmlReader.Name.Equals("database"))
							{
								dbName = xmlReader.GetAttribute("name");
								dbType = xmlReader.GetAttribute("type");
								inDatabase = true;
							}
							break;
						case XmlNodeType.Text:
							Logger.Debug("text=" + xmlReader.Value.Trim());
							if (inDatabase)
							{
								Logger.InfoFormat("DATABASE FOUND: {0}:{1}", dbName, dbType);

								DbConnectionManager.ProviderType nativeType;
								if (Enum.TryParse(dbType, true, out nativeType))
								{
									addConnectionManager(dbName, new DbConnectionManager(nativeType, xmlReader.Value.Trim()));
								}
								else
								{
									Logger.ErrorFormat("invalid database type: {0}", dbType);
								}
								//DbConnectionManager.ProviderType nativeType = (DbConnectionManager.ProviderType)Enum.Parse(typeof(DbConnectionManager.ProviderType), dbType);

							}
							break;
						case XmlNodeType.EndElement:
							if (xmlReader.Name.Equals("database"))
							{
								inDatabase = false;
							}
							break;
					}
				}
			}
			else
			{
				Logger.ErrorFormat("database configuration not found: {0}", DB_CONFIG_FILENAME);
			}
		}

		/// <summary>
		/// Neuen Verbindungsmanager einfügen. Im Falle von Duplikaten wird eine Exception geworfen!
		/// </summary>
		/// <param name="iName">Name der Datenbankverbindung</param>
		/// <param name="iManager">Der zugehörige Manager</param>
		public static void addConnectionManager(string iName, DbConnectionManager iManager)
		{
			if (!mDict.ContainsKey(iName)) mDict.Add(iName, iManager);
		}

		/// <summary>
		/// Einen Verbindungsmanager über seinen Namen anfordern
		/// </summary>
		/// <param name="iName">Name des Managers / der Verbindung</param>
		/// <returns>Den Manager</returns>
		public static DbConnectionManager getConnectionManager(string iName)
		{
			return mDict.ContainsKey(iName) ? mDict[iName] : null;
		}
		/// <summary>
		/// Den Verbindungsmanager anfordern.
		/// Funktioniert nur, wenn genau eine Verbindung definiert wurde, sonst null!
		/// </summary>
		/// <returns>Den Manager</returns>
		public static DbConnectionManager getConnectionManager()
		{
			return mDict.Count() == 1 ? mDict.Values.ElementAt(0) : null;
		}
	}
}
