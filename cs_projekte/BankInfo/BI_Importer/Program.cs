using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Globalization;
using knk.shared;
using knk.DBTools;

namespace BI_Importer
{
	class Position
	{
		public string Buchungsdatum;
		public string Valutadatum;
		public string Verwendung;
		public string Notiz;
		public string Beleginfo;
		public string Betrag;
		public string Waehrung;
	}

	class ImportStatus
	{
		public int cnt_positionen;
		public int cnt_err_positionen;
		public int cnt_imports;
		public int cnt_err_imports;
		private string _first_error;
		public string first_error { set { if (_first_error == null) _first_error = value; } get { return _first_error; } }
	}

    class Program
    {
		static AppGlobalSimple ag = AppGlobalSimple.createInstance() as AppGlobalSimple;

		void handlePosition(Connection iConn, ImportStatus iStatus, int iKontoID, Position iPos)
		{
//			ag.Log.LogDebug("POS {0}#{1}: {2} {3}", iPos.Buchungsdatum, iPos.Valutadatum, iPos.Waehrung, iPos.Betrag);

			DateTime effdt = DateTime.ParseExact(iPos.Valutadatum, "dd/MM/yyyy", CultureInfo.InvariantCulture);
			DateTime posteddt = DateTime.ParseExact(iPos.Buchungsdatum, "dd/MM/yyyy", CultureInfo.InvariantCulture);
			string vks = iPos.Betrag.Substring(0, iPos.Betrag.Length - 2);
			string nks = iPos.Betrag.Substring(iPos.Betrag.Length - 2);
			vks = vks.Replace(".", "");
			vks = vks.Replace(",", "");

			double Betrag = double.Parse(vks + "." + nks, CultureInfo.InvariantCulture);
			try
			{
				iConn.execSQL("insert into positionen (effdt,posteddt,curcode,amt,memo,ktoid,voucher) values (:1,:2,:3,:4,:5,:6,:7)",
					effdt, posteddt, iPos.Waehrung, Betrag, iPos.Verwendung, iKontoID, iPos.Beleginfo);
				iStatus.cnt_imports++;
			}
			catch (Exception e)
			{
			//	ag.Log.LogError(e.Message);
				iStatus.first_error = e.Message;
				iStatus.cnt_err_imports++;
			}
		}

		ImportStatus import_file(Connection iConn, string iFilename, int iKontoID)
		{
			ImportStatus r = new ImportStatus();
			string[] lines = System.IO.File.ReadAllLines(iFilename, Encoding.Default);
			String workingLine = "";
			Position aPos = new Position();

			foreach (string aLine in lines.Skip(1))
			{
				workingLine += aLine;
				if (workingLine.Contains("\";"))
				{
					string myLine = "";
					bool inComment = false;
					for (int i = 0; i < workingLine.Length; i++)
					{
						Char c = workingLine[i];
						if (c == '"')
						{
							inComment = inComment ? false : true;
						}
						if (c == ';' && !inComment)
						{
							c = (char)1;
						}
						myLine += c;
					}

					string[] cols = myLine.Split((char)1);
					if (cols.Count() != 8)
					{
						r.first_error = string.Format("POSITION {1}: col count == {0}", cols.Count(), r.cnt_positionen);
						r.cnt_err_positionen++;
					}
					else
					{
						r.cnt_positionen++;
						aPos.Buchungsdatum = cols[0];
						aPos.Valutadatum = cols[1];

						byte[] myASCIIBytes = Encoding.Default.GetBytes(cols[2]);
						byte[] myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Verwendung = Encoding.Unicode.GetString(myUTF8Bytes);
						//aPos.Verwendung = cols[2];
						myASCIIBytes = Encoding.Default.GetBytes(cols[3]);
						myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Notiz = Encoding.Unicode.GetString(myUTF8Bytes);
						//aPos.Notiz = cols[3];
						aPos.Waehrung= cols[4];
						aPos.Betrag = cols[5];
						myASCIIBytes = Encoding.Default.GetBytes(cols[6].Trim('"'));
						myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Beleginfo = Encoding.Unicode.GetString(myUTF8Bytes);
//						aPos.Beleginfo = cols[6];
						handlePosition(iConn, r, iKontoID, aPos);
					}
					workingLine = "";
				}
			}
			return r;
		}

        static void Main(string[] args)
        {
			Program prog = new Program();

			ag.Log.LogInfo("startup");

			if (args.Length < 1)
			{
				ag.Log.LogWarning("Aufruf: {0} [FILE]", "BI_Importer");
				Environment.Exit(0);
			}

			string db_host = "localhost";
			string db_user = "kuk";
			string db_pwd = "kuk";
			string db_database = "testdb";

			ag.Log.LogInfo("Datenbankverbindung: {0}@{1} als User {2}", db_database, db_host, db_user);
			ConnectionPool DBPool = new ConnectionPool(ConnectionPool.ProviderType.Postgres, db_host, db_database, db_user, db_pwd, 0);
			if (!DBPool.checkConnection(ag.Log))
			{
				ag.Log.LogError("Datenbank nicht erreichbar!");
				Environment.Exit(0);
			}
			else
			{
				ag.Log.LogInfo("Datenbankverbindung ok.");
			}

			try
			{
				Connection conn = DBPool.getConnection();

				ArrayList arrID = new ArrayList();
				ArrayList arrNR = new ArrayList();
				ArrayList arrName = new ArrayList();
				conn.execSQL_select("select id,ktonr,ktoname from konten", arrID, arrNR, arrName);
				Console.WriteLine("================");
				for (int i = 0; i < arrID.Count; i++)
				{
					Console.WriteLine("{0}: {1} - {2}", arrID[i], arrNR[i], arrName[i]);
				}
				Console.Write("Kontenauswahl: ");
				int answer = int.Parse(Console.ReadLine());
				bool found = false;
				for (int i = 0; i < arrID.Count; i++)
				{
					if (answer == (int)arrID[i])
					{
						found = true;
						ag.Log.LogInfo("Verwende Konto {0}: {1}", arrID[i], arrNR[i]);
						break;
					}
				}

				if (found)
				{
//					ImportStatus erg = prog.import_file(conn, "../../IKS-01-201211.csv", answer);
					ImportStatus erg = prog.import_file(conn, args[0], answer);
					ag.Log.LogInfo("Ergebnis: Positionen: {0}, Importiert: {1}", erg.cnt_positionen, erg.cnt_imports);
					if ((erg.cnt_err_imports + erg.cnt_err_positionen) > 0)
					{
						ag.Log.LogError("Fehler: Positionen: {0}, Import: {1} : {2}", erg.cnt_err_positionen, erg.cnt_err_imports, erg.first_error);
					}
				}
				else
				{
					ag.Log.LogError("Ungültige Auswahl!");
				}

				conn = DBPool.releaseConnection(conn);
			}
			catch (Exception e)
			{
				ag.Log.LogError("{0}", e.Message);
			}

			ag.Log.LogInfo("shutdown");
		}
    }
}
