using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;

namespace dbigen
{
	class CLookup
	{
		Hashtable hashTypes = new Hashtable();
		Hashtable hashStructs = new Hashtable();
		Hashtable hashTables = new Hashtable();

		Hashtable hash4type(classes.dbi_type iDbiType)
		{
			Hashtable r = null;

			switch (iDbiType)
			{
				case classes.dbi_type.TYPE:
					r = hashTypes;
					break;
				case classes.dbi_type.STRUCT:
					r = hashStructs;
					break;
				case classes.dbi_type.TABLE:
					r = hashTables;
					break;
			}
			return r;
		}

		public Hashtable getDbiTypeHash(classes.dbi_type iDbiType)
		{
			Hashtable hash = hash4type(iDbiType);
			return hash;
		}

		public classes.i_dbi_type lookupDbiType(classes.dbi_type iDbiType, string iName)
		{
			classes.i_dbi_type r = null;
			Hashtable hash = hash4type(iDbiType);
			if (hash != null)
			{
				r = (classes.i_dbi_type)hash[iName];
			}
			return r;
		}

		public bool addDbiType(classes.i_dbi_type iType)
		{
			bool r = false;
			Hashtable hash = hash4type(iType.getDbiType());

			if (hash != null)
			{
				if (!hash.ContainsKey(iType.getName())) {
					hash.Add(iType.getName(), iType);
					r = true;
				}
			}
	
			return r;
		}

	}
}
