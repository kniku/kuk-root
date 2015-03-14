
namespace knk.DBTools.ProviderImpl
{
	class ProviderFactory
	{
		public static IConnectionProvider createProvider(ConnectionPool.ProviderType iProvider)
		{
			IConnectionProvider r = null;
			switch (iProvider)
			{
				case ConnectionPool.ProviderType.Postgres:
					r = new ProviderImpl.Postgres.ConnectionProvider();
					break;
				case ConnectionPool.ProviderType.SQLite:
					r = new ProviderImpl.SQLite.ConnectionProvider();
					break;
				case ConnectionPool.ProviderType.SqlServer:
					r = new ProviderImpl.SqlServer.ConnectionProvider();
					break;
				default:
					break;
			}
			return r;
		}
	}
}
