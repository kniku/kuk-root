using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using System.Data.Common;
using WebAPI.DbAccess;
using WebAPI.DbAccess.Impl;
using WebAPI.Services;
using WebAPI.Services.Impl;

namespace WebAPI
{
	public class Startup
    {
        public Startup(IConfiguration configuration, ILoggerFactory loggerFactory)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddMvc();
	        services.AddCors();

			// Register application services.
	        services.AddSingleton<IDbAccessFactory , DbAccessFactoryPostgres>();
	        services.AddSingleton<IKontoService, KontoService>();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env, ILoggerFactory loggerFactory)
        {
	        AppGlobal.Logging.LoggerFactory = loggerFactory;
	        AppGlobal.Application = app;

            if (env.IsDevelopment())
            {
	            AppGlobal.DatabaseAccess = new DbAccess.DbAccess(Configuration.GetConnectionString("BankInfo_test"));
                app.UseDeveloperExceptionPage();
	            dbtest();
            }
            else
            {
	            AppGlobal.DatabaseAccess = new DbAccess.DbAccess(Configuration.GetConnectionString("BankInfo_prod"));
            }

	        app.UseCors(config => config.AllowAnyOrigin());
            app.UseMvc();
        }

	    void dbtest()
	    {
		    ILogger logger = AppGlobal.Logging.CreateLogger("WebAPI.Controllers");
			logger.LogWarning("STARTUP: DB TEST");
		    logger.LogWarning($"connStr=[{AppGlobal.DatabaseAccess.ConnectionString}]");

		    DbCommand cmd = AppGlobal.DatabaseAccess.GetCommand("select count(*) from konten");
			var result = cmd.ExecuteScalar();
			cmd.Connection.Close();
		    
		    logger.LogWarning($"result={result}");
		    logger.LogWarning("STARTUP: DB TEST - done.");

		}
    }
}
