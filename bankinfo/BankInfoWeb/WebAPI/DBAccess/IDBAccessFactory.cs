using System.Data.Common;

namespace WebAPI.DbAccess
{
	public interface IDbAccessFactory
	{
		DbConnection DbConnection { get; }
		DbCommand DbCommand { get; }
	}
}
