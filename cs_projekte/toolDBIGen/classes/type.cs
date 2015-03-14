using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace dbigen.classes
{
	class CType : i_dbi_type
	{
		public enum basetype
		{
			NONE,
			NUMERIC,
			STRING,
			DATETIME,
			AUTO,
			GUID
		}
		string name;
		public string descr;
		public int precision, scale;
		public basetype type;
		public string sql_default;

		public CType(string iName)
		{
			name = iName;
		}

		#region i_dbi_type Member

		public dbi_type getDbiType()
		{
			return dbigen.classes.dbi_type.TYPE;
		}

		public string getName()
		{
			return name;
		}

		#endregion
	}
}
