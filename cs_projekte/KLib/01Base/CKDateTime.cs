using System;
using System.Globalization;

namespace KLib.Base
{
	/// <summary>
	/// static DateTime helper class
	/// </summary>
	public static class KDateTimeExt
    {
		/// <summary>
		/// Erzeugt einen Std. Zeitstempelstring
		/// </summary>
		public static string convert2DateTimeStr(DateTime Date)
		{
			return Date.ToString("yyyyMMddHHmmss");
		}
		public static bool parseDateTimeStr(string DateTimeStr, out DateTime Date)
		{
			return DateTime.TryParseExact(DateTimeStr, "yyyyMMddHHmmss", CultureInfo.CurrentCulture, DateTimeStyles.None, out Date);
		}
		/// <summary>
		/// Erzeugt einen Std. Datumstring
		/// </summary>
		public static string convert2DateStr(DateTime Date)
		{
			return Date.ToString("yyyyMMdd");
		}
		public static bool parseDateStr(string DateStr, out DateTime Date)
		{
			return DateTime.TryParseExact(DateStr, "yyyyMMdd", CultureInfo.CurrentCulture, DateTimeStyles.None, out Date);
		}
		/// <summary>
		/// Erzeugt einen Std. Zeitstring
		/// </summary>
		public static string convert2TimeStr(DateTime Date)
		{
			return Date.ToString("HHmmss");
		}
		public static bool parseTimeStr(string TimeStr, out DateTime Date)
		{
			return DateTime.TryParseExact(TimeStr, "HHmmss", CultureInfo.CurrentCulture, DateTimeStyles.None, out Date);
		}
	}
}
