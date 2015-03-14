using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Globalization;
using System.IO;
using log4net;

namespace ClientServer
{
	public class CTools
	{
		static readonly ILog log = LogManager.GetLogger(typeof(CTools));

		/// <summary>
		/// Erzeugt einen Prefix einer Serverantwort
		/// </summary>
		/// <param name="Command"></param>
		/// <param name="Code"></param>
		/// <returns></returns>
		public static string createReturnMessagePrefix(string Command, CConstants.ERRORCODE Code)
		{
			return string.Format("{0}|{1}", Command, (int )Code);
		}

		/// <summary>
		/// Allgemeine Fehlermeldung senden
		/// </summary>
		/// <param name="Stream"></param>
		/// <param name="Code"></param>
		/// <param name="Msg"></param>
		public static void sendError(NetworkStream Stream, CConstants.ERRORCODE Code, string Msg)
		{
			CTools.sendString(Stream, string.Format("{0}|{1}", createReturnMessagePrefix("ERR", Code), Msg));
		}

		/// <summary>
		/// Schickt einen string über den Stream
		/// </summary>
		/// <param name="Stream"></param>
		/// <param name="msg"></param>
		public static void sendString(NetworkStream Stream, string msg)
		{
			if (Stream != null && Stream.CanWrite)
			{
				try
				{
					ASCIIEncoding encoder = new ASCIIEncoding();
					byte[] buffer = encoder.GetBytes(msg);
					Stream.Write(buffer, 0, buffer.Length);
					Stream.Flush();
				}
				catch (Exception e)
				{
					log.Error(e);
				}
			}
		}

		/// <summary>
		/// Liest einen string aus dem Stream
		/// </summary>
		/// <param name="Stream"></param>
		/// <returns></returns>
		public static string receiveString(NetworkStream Stream)
		{
			string r = null;
			
			if (Stream != null && Stream.CanRead)
			{
				byte[] message = new byte[4096];
				int bytesRead;

				try
				{
					//blocks until a client sends a message
					bytesRead = Stream.Read(message, 0, 4096);
					ASCIIEncoding encoder = new ASCIIEncoding();
					r = encoder.GetString(message, 0, bytesRead);
				}
				catch (Exception e)
				{
					log.Error(e);
				}
			}
			return r;
		}

		/// <summary>
		/// Zerlegen einer Nachricht (Client/Server) in ein string array
		/// </summary>
		/// <param name="Message"></param>
		/// <returns></returns>
		public static string[] getMessageArray(string Message)
		{
			string[] r = null;
			if (Message != null) r = Message.Split('|');
			return r;
		}

		/// <summary>
		/// Extrahieren eines integers aus einem Message Array
		/// </summary>
		/// <param name="Array">Das Array</param>
		/// <param name="Index">Der Zugriffsindex</param>
		/// <returns>Wert an der Stelle [Index] als Integer</returns>
		public static int getIntFromMessageArray(string[] Array, int Index)
		{
			int r = -1;
			if (Array != null && Array.Count() > Index)
			{
				int.TryParse(Array[Index], out r);
			}

			return r;
		}

		public static string convertBoolToStrBool(bool iBool)
		{
			return iBool ? "t" : "f";
		}

		public static bool convertBoolStrToBool(string StrBool)
		{
			bool r = false;
			if (StrBool != null)
			{
				r = StrBool.Equals("t") ? true : false;
			}
			return r;
		}

		public static string convertDateTimeToStr(DateTime DT)
		{
			string r = null;
			if (DT != null) r = DT.ToString("yyyyMMddHHmmss", CultureInfo.InvariantCulture);
			return r;
		}

		public static DateTime convertStrToDateTime(string Str)
		{
			DateTime r = DateTime.MinValue;

			if (Str != null && Str.Length == 14)
			{
				DateTime.TryParseExact(Str, "yyyyMMddHHmmss", null, DateTimeStyles.None, out r);
			}
			return r;
		}

	}
}
