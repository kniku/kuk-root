using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Options;
using WebAPI.Services;
using WebAPI.Services.Impl;

namespace WebAPI
{
	internal static class ApplicationLogging
	{
		internal static ILoggerFactory LoggerFactory { get; set; }
		internal static ILogger CreateLogger<T>() => LoggerFactory.CreateLogger<T>();        
		internal static ILogger CreateLogger(string categoryName) => LoggerFactory.CreateLogger(categoryName);
	}

    public class Startup
    {
        public Startup(IConfiguration configuration, ILoggerFactory loggerFactory)
        {
            Configuration = configuration;
	        ApplicationLogging.LoggerFactory = loggerFactory;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddMvc();
	        services.AddCors();

			// Register application services.
	        services.AddSingleton<IKontoService, KontoService>();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

	        app.UseCors(config => config.AllowAnyOrigin());
            app.UseMvc();
        }
    }
}
