using Npgsql;
using System.Data.Common;

namespace WebAPI.DbAccess.Impl
{
	public class DbAccessFactoryPostgres : IDbAccessFactory
	{
		//DbConnectionStringBuilder IDbAccessFactory.DbConnectionStringBuilder => new NpgsqlConnectionStringBuilder();
		DbConnection IDbAccessFactory.DbConnection => new NpgsqlConnection();
		DbCommand IDbAccessFactory.DbCommand => new NpgsqlCommand();
	}
}
