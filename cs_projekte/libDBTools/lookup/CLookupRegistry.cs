using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

namespace knk.DBTools.lookup
{
	static class CLookupRegistry
	{
		static Hashtable hashStructures = new Hashtable();
		static Hashtable hashTables = new Hashtable();

		static Hashtable getHash4Type(types.DBI_BaseTypes iType)
		{
			Hashtable r = null;
			switch (iType)
			{
				case types.DBI_BaseTypes.STRUCTURE:
					r = hashStructures;
					break;
				case types.DBI_BaseTypes.TABLE:
					r = hashTables;
					break;
			}
			return r;
		}

		public static bool register(types.IBaseType iType)
		{
			bool r = false;
			Hashtable hash = getHash4Type(iType.getBaseType());

			if (hash != null)
			{
				if (!hash.ContainsKey(iType.getName()))
				{
					hash.Add(iType.getName(), iType);
					r = true;
				}
			}
			
			return r;
		}

		public static types.IBaseType lookupType(types.DBI_BaseTypes iBaseType, string iName)
		{
			types.IBaseType r = null;
			Hashtable hash = getHash4Type(iBaseType);

			if (hash != null)
			{
				r = (types.IBaseType)hash[iName];
			}
			return r;
		}
	}
}
