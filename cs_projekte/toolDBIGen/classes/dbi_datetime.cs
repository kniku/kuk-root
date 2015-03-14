using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace dbigen.classes
{
	// normalized timestamp (NTS): CCYYMMDDhhmmss
	//                             <date  ><time>

	interface dbi_nts_tool
	{
		// konvertiert NTS in SQL Format
		string NTS2SQL(string iNormalizedTimestamp);
	}

	class dbi_nts_postgres : dbi_nts_tool
	{
		public string NTS2SQL(string iNormalizedTimestamp)
		{
			string r = null;
			if (iNormalizedTimestamp.Length == 8 || iNormalizedTimestamp.Length == 14)
			{
				r = string.Format("{0}-{1}-{2}",
						iNormalizedTimestamp.Substring(0, 4),	// year
						iNormalizedTimestamp.Substring(4, 2),	// month
						iNormalizedTimestamp.Substring(6,2));	// day
				if (iNormalizedTimestamp.Length == 14)
				{
					r = r + string.Format(" {0}:{1}:{2}",
							iNormalizedTimestamp.Substring(8, 2),	// hour
							iNormalizedTimestamp.Substring(10, 2),	// minutes
							iNormalizedTimestamp.Substring(12, 2));	// seconds
				}
				else
				{
					r = r + " 00:00:00";
				}
			}
			return r;
		}
	}

	class dbi_nts_mssql : dbi_nts_tool
	{
		public string NTS2SQL(string iNormalizedTimestamp)
		{
			//2004-05-23T14:25:10
			string r = null;
			if (iNormalizedTimestamp.Length == 8 || iNormalizedTimestamp.Length == 14)
			{
				r = string.Format("{0}{1}{2}",
						iNormalizedTimestamp.Substring(0, 4),	// year
						iNormalizedTimestamp.Substring(4, 2),	// month
						iNormalizedTimestamp.Substring(6, 2));	// day
				if (iNormalizedTimestamp.Length == 14)
				{
					r = r + string.Format("{0}:{1}:{2}",
							iNormalizedTimestamp.Substring(8, 2),	// hour
							iNormalizedTimestamp.Substring(10, 2),	// minutes
							iNormalizedTimestamp.Substring(12, 2));	// seconds
				}
				else
				{
					r = r + "00:00:00";
				}
			}
			return r;
		}
	}
}
