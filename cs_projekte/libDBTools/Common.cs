using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace knk.DBTools
{
	/// <summary>
	/// abstract base class for table structures
	/// </summary>
	public class ATableStruct
	{
		string mTableName = null;

		public string TableName()
		{
			if (mTableName == null) mTableName = LookupRegistry.getTableNameFromAnnotations(this.GetType());
			return mTableName;
		}
	}
	
	/// <summary>
	/// attribute to mark/configure a tablestruct
	/// </summary>
	[AttributeUsage(AttributeTargets.Class)]
	public class SQLTABLE : Attribute
	{
		public string name = null;
	}

	/// <summary>
	/// attribute to mark/configure a table column
	/// </summary>
	[AttributeUsage(AttributeTargets.Property)]
	public class SQLCOLUMN : Attribute
	{
		public string name = null;
		public bool auto = false;
	}

}
