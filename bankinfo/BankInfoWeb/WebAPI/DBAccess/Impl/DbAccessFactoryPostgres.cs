using Npgsql;
using System.Data.Common;

namespace WebAPI.DbAccess.Impl
{
	public class DbAccessFactoryPostgres : IDbAccessFactory
	{
		public DbConnection DbConnection => new NpgsqlConnection();
		public DbCommand DbCommand => new NpgsqlCommand();
	}
}
