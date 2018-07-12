using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.Logging;

namespace WebAPI
{
	public static class AppGlobal
    {
	    public static IApplicationBuilder Application { get; set; }

	    public static class Logging
	    {
		    public static ILoggerFactory LoggerFactory { get; set; }
		    public static ILogger CreateLogger<T>() => LoggerFactory.CreateLogger<T>();        
		    public static ILogger CreateLogger(string categoryName) => LoggerFactory.CreateLogger(categoryName);
	    }

	    public static DbAccess.DbAccess DatabaseAccess { get; set; }
    }
}
