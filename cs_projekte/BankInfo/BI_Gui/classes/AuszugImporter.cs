using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;
using knk.DBTools;

namespace BI_Gui.classes
{

	class ImportStatus
	{
		public int cnt_positionen;
		public int cnt_err_positionen;
		public int cnt_imports;
		public int cnt_err_imports;
		private string _first_error;
		public string first_error { set { if (_first_error == null) _first_error = value; } get { return _first_error; } }
	}

	class AuszugImporter
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

		static void parseDatum(string DTstring, out DateTime DT)
		{
			if (!DateTime.TryParseExact(DTstring, "dd/MM/yyyy", CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out DT))
				if (!DateTime.TryParseExact(DTstring, "dd.MM.yyyy", CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out DT))
					throw new Exception(string.Format("Ungültiges Datum: {0}", DTstring));
		}

		static void handlePosition(Connection iConn, ImportStatus iStatus, int iKontoID, Position iPos)
		{
			//			ag.Log.LogDebug("POS {0}#{1}: {2} {3}", iPos.Buchungsdatum, iPos.Valutadatum, iPos.Waehrung, iPos.Betrag);

			DateTime effdt, posteddt;
			parseDatum(iPos.Valutadatum, out effdt);
			parseDatum(iPos.Buchungsdatum, out posteddt);
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

		public static ImportStatus import_file(Connection iConn, string iFilename, int iKontoID)
		{
			ImportStatus r = new ImportStatus();
			string[] lines = System.IO.File.ReadAllLines(iFilename, Encoding.Default);
			String workingLine = "";
			Position aPos = new Position();

			bool inComment = false;	// Achtung: Zeilenübergreifende Kommentare!!

			foreach (string aLine in lines.Skip(1))
			{
				//workingLine += aLine;

				for (int i = 0; i < aLine.Length; i++)
				{
					Char c = aLine[i];
					if (c == '"')
					{
						inComment = inComment ? false : true;
					}
					if (c == ';' && !inComment)
					{
						c = (char)1;
					}
					workingLine += c;
				}

				if (workingLine.Count(c => c == (char)1) == 15)
				{
					string[] cols = workingLine.Split((char)1);
					try
					{
						r.cnt_positionen++;
						aPos.Buchungsdatum = cols[0];
						aPos.Valutadatum = cols[1];

						byte[] myASCIIBytes = Encoding.Default.GetBytes(cols[2].Trim('"'));
						byte[] myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Verwendung = Encoding.Unicode.GetString(myUTF8Bytes);
						//aPos.Verwendung = cols[2];
						myASCIIBytes = Encoding.Default.GetBytes(cols[3].Trim('"'));
						myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Notiz = Encoding.Unicode.GetString(myUTF8Bytes);
						//aPos.Notiz = cols[3];
						aPos.Waehrung = cols[4];
						aPos.Betrag = cols[5];
						myASCIIBytes = Encoding.Default.GetBytes(cols[6].Trim('"'));
						myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Beleginfo = Encoding.Unicode.GetString(myUTF8Bytes);
						//						aPos.Beleginfo = cols[6];
						handlePosition(iConn, r, iKontoID, aPos);
					}
					catch (Exception)
					{
						r.cnt_err_positionen++;
						r.first_error = string.Format("POSITION {1}: col count == {0}\nLine=[{2}]", cols.Count(), r.cnt_positionen, workingLine.Replace((char)1, ';'));
						return r;
					}
					workingLine = "";
					inComment = false;
				}
			}
			return r;
		}
#if false
		public static ImportStatus import_file(Connection iConn, string iFilename, int iKontoID)
		{
			ImportStatus r = new ImportStatus();
			string[] lines = System.IO.File.ReadAllLines(iFilename, Encoding.Default);
			String workingLine = "";
			Position aPos = new Position();

			foreach (string aLine in lines.Skip(1))
			{
				workingLine += aLine;
				if (workingLine.EndsWith("\";"))
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

						byte[] myASCIIBytes = Encoding.Default.GetBytes(cols[2].Trim('"'));
						byte[] myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Verwendung = Encoding.Unicode.GetString(myUTF8Bytes);
						//aPos.Verwendung = cols[2];
						myASCIIBytes = Encoding.Default.GetBytes(cols[3].Trim('"'));
						myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Notiz = Encoding.Unicode.GetString(myUTF8Bytes);
						//aPos.Notiz = cols[3];
						aPos.Waehrung = cols[4];
						aPos.Betrag = cols[5];
						myASCIIBytes = Encoding.Default.GetBytes(cols[6].Trim('"'));
						myUTF8Bytes = ASCIIEncoding.Convert(Encoding.Default, Encoding.Unicode, myASCIIBytes);
						aPos.Beleginfo = Encoding.Unicode.GetString(myUTF8Bytes);
						//						aPos.Beleginfo = cols[6];
						handlePosition(iConn, r, iKontoID, aPos);
					}
					workingLine = "";
				}
				else
				{
					workingLine += " ";// Environment.NewLine;
				}
			}
			return r;
		}
#endif
	}
}

