using System;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using System.Data.Common;
using System.Text.RegularExpressions;
using Microsoft.AspNetCore.Razor.Language.Intermediate;
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
			switch (Configuration.GetSection("Application")["DBMSType"])
	        {
		        case "Postgres":
			        services.AddSingleton<IDbAccessFactory , DbAccessFactoryPostgres>();
			        break;
				default:
					services.AddSingleton<IDbAccessFactory , DbAccessFactorySqlServer>();
					break;
	        }
	        services.AddSingleton<IKontoService, KontoService>();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env, ILoggerFactory loggerFactory)
        {
	        AppGlobal.Logging.LoggerFactory = loggerFactory;
	        AppGlobal.Application = app;

            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

	        AppGlobal.DatabaseAccess = new DbAccess.DbAccess(Configuration.GetConnectionString(Configuration.GetSection("Application")["DBMSConnection"]));
	        dbtest();

	        app.UseCors(config => config.AllowAnyOrigin());
            app.UseMvc();
        }


	    void dbtest()
	    {
		    ILogger logger = AppGlobal.Logging.CreateLogger<Startup>() /* "WebAPI.Controllers"*/;
		    string csWithoutPassword = Regex.Replace(AppGlobal.DatabaseAccess.ConnectionString, @"(password\s*=\s*)(\w+)",
			    "$1*hidden*", RegexOptions.IgnoreCase);

		    logger.LogDebug($"Checking database connection [{csWithoutPassword}]...");

		    var x = AppGlobal.DatabaseAccess.CheckConnection();
			if (x != null)
			{
				logger.LogCritical($"Database connection error!\nConnection string=[{csWithoutPassword}]\nException: {x.Message}");
		    }
			else
			{
				logger.LogDebug("Database connection Ok.");
			}
		}
    }
}
