using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace knk.DBTools.lookup.types
{
	public enum DBI_BaseTypes
	{
		NONE,
		STRUCTURE,
		TABLE
	}

	interface IBaseType
	{
		string getName();
		DBI_BaseTypes getBaseType();
	}
}
