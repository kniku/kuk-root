using System.Data.Common;
using System.Data.SqlClient;

namespace WebAPI.DbAccess.Impl
{
	public class DbAccessFactorySqlServer : IDbAccessFactory
	{
		//DbConnectionStringBuilder IDbAccessFactory.DbConnectionStringBuilder => new NpgsqlConnectionStringBuilder();
		DbConnection IDbAccessFactory.DbConnection => new SqlConnection();
		DbCommand IDbAccessFactory.DbCommand => new SqlCommand();
	}
}
