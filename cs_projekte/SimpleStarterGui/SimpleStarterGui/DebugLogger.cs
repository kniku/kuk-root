namespace SimpleStarterGui;

public interface IDebugLogger
{
    void Debug(string iMsg);
    void Information(string iMsg);
    void Warning(string iMsg);
    void Error(Exception iException, string? iMsg = null);
    void Error(string iMsg);
}

public static class DebugLoggerFactory
{
    private static IDebugLogger? _logger;
    public static IDebugLogger GetOrCreate()
    {
        return _logger ??= new DebugLogger();
    }
}

public class DebugLogger: IDebugLogger
{
    public string LogFullPath { get; }

    public DebugLogger()
    {
        LogFullPath = Utils.GetFullPathOfExecutingAssemblyWithNewExtension("log", true);
        File.Delete(LogFullPath);
        Information("Startup");
    }

    private void Log(string iPrefix, string iMsg)
    {
        var msg = $"{DateTime.Now} {iPrefix}: {iMsg}";
        System.Diagnostics.Debug.WriteLine($"##### {msg}");
        File.AppendAllText(LogFullPath, msg + Environment.NewLine);
    }

    public void Debug(string iMsg)
    {
        Log("DBG", iMsg);
    }

    public void Information(string iMsg)
    {
        Log("INF", iMsg);
    }

    public void Warning(string iMsg)
    {
        Log("WRN", iMsg);
    }

    public void Error(Exception iException, string? iMsg = null)
    {
        Error(iMsg == null ? $"EXCEPTION: {iException}" : $"{iMsg}, EXCEPTION: {iException}");
    }
    
    public void Error(string iMsg)
    {
        Log("ERR", iMsg);
    }
}
