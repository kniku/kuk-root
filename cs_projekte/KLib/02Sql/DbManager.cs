using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KLib.Sql
{
	/// <summary>
	/// Verwaltet alle Verbindungsmanager einer Anwendung (Verbindung zu verschiedenen datenbanken)
	/// </summary>
	public static class DbManager
	{
		/// <summary>
		/// Internes Dictionary für die Verbindungsmanager
		/// </summary>
		static Dictionary<string, DbConnectionManager> mDict = new Dictionary<string,DbConnectionManager>();

		/// <summary>
		/// Neuen Verbindungsmanager einfügen. Im Falle von Duplikaten wird eine Exception geworfen!
		/// </summary>
		/// <param name="iName">Name der Datenbankverbindung</param>
		/// <param name="iManager">Der zugehörige Manager</param>
		public static void addConnectionManager(string iName, DbConnectionManager iManager)
		{
			mDict.Add(iName, iManager);
		}

		/// <summary>
		/// Einen verbindungsmanager über seinen Namen anfordern
		/// </summary>
		/// <param name="iName">Name des Managers / der Verbindung</param>
		/// <returns>Den Manager</returns>
		public static DbConnectionManager getConnectionManager(string iName)
		{
			return mDict[iName];
		}
	}
}
