using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ClientServer
{
	public class CConstants
	{
		// well known commands
		/// <summary>
		/// Hilfetext anfordern
		/// Parameter: Keine
		/// </summary>
		public const string CMD_HELP = "help";
		/// <summary>
		/// Prüfen, ob Datei existiert (am Server)
		/// Parameter: Dateipfad
		/// </summary>
		public const string CMD_FILE_EXIST = "finfo";
		/// <summary>
		/// Abrufen/Setzen des aktuellen Arbeitsverzeichnisses
		/// Parameter: Keine (Abrufen) / Pfad (Setzen)
		/// </summary>
		public const string CMD_CURRENT_DIRECTORY = "cwd";
		/// <summary>
		/// Holt eine Datei vom Server
		/// Aufruf: fget|FILENAME
		/// </summary>
		public const string CMD_GETFILE = "fget";

		public const string CONST_BOOLSTR_TRUE = "t";
		public const string CONST_BOOLSTR_FALSE = "f";

		public const string CONST_FILETYPE_FILE = "F";
		public const string CONST_FILETYPE_DIRECTORY = "D";

		/// <summary>
		/// Well known error codes
		/// </summary>
		public enum ERRORCODE
		{
			OK = 0,				// kein Fehler
			NotExisting,		// "irgendwas" benötigtes nicht existent (Datei, ...)
			Unspecified = 999
		}

	}
}
