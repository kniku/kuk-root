using System.Data.Common;

namespace WebAPI.DbAccess
{
	public interface IDbAccessFactory
	{
		//DbConnectionStringBuilder DbConnectionStringBuilder { get; }
		DbConnection DbConnection { get; }
		DbCommand DbCommand { get; }
	}
}
