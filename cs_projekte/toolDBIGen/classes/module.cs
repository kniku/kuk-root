using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace dbigen.classes
{
	class CModule : i_dbi_type
	{
		string name;

		public CModule(string iName)
		{
			name = iName;
		}

		#region i_dbi_type Member

		public dbi_type getDbiType()
		{
			return dbi_type.MODULE;
		}

		public string getName()
		{
			return name;
		}

		#endregion
	}
}
