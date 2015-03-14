using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

namespace dbigen.classes
{
	class CStructMember
	{
		public string name;
		public string descr;
		public string reftype;
		public bool isNull;
		public string sql_default;
		public CStructMember(string iName)
		{
			name = iName;
		}
	}

	class CStructure : i_dbi_type
	{
		string name;
		public string descr;
		public ArrayList arrayMember { get; private set; }
		public Hashtable hashMember { get; private set; }	// für schnellen zugriff per namen

		public CStructure(string iName)
		{
			name = iName;
			arrayMember = new ArrayList();
			hashMember = new Hashtable();
		}

		public bool addMember(CStructMember iMember)
		{
			bool r = false;

			if (!hashMember.ContainsKey(iMember.name))
			{
				hashMember.Add(iMember.name, arrayMember.Add(iMember));
			}
			return r;
		}

		#region i_dbi_type Member

		public dbi_type getDbiType()
		{
			return dbigen.classes.dbi_type.STRUCT;
		}

		public string getName()
		{
			return name;
		}

		#endregion
	}
}
