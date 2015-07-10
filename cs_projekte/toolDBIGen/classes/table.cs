using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

namespace dbigen.classes
{

	public class CForKey
	{
		public string cols, for_cols, for_table;
	}

	class CTable : i_dbi_type
	{
		string name;
		public string descr;
		public string reftype;
		public ArrayList arrayKeyColumns { get; private set; }
		public List<CForKey> arrayForKeys { get; private set; }


		public CTable(string iName)
		{
			name = iName;
			arrayKeyColumns = new ArrayList();
			arrayForKeys = new List<CForKey>();
		}

		#region i_dbi_type Member

		public dbi_type getDbiType()
		{
			return dbigen.classes.dbi_type.TABLE;
		}

		public string getName()
		{
			return name;
		}

		#endregion
	}
}
