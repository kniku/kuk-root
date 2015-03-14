using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace dbigen.classes
{
	enum dbi_type
	{
		NONE,
		MODULE,
		GENERATOR,
		TYPE,
		STRUCT,
		TABLE
	}

	interface i_dbi_type
	{
		dbi_type getDbiType();
		string getName();
	}
}
