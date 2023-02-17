using System.Reflection;
using System.Text.Json;

namespace SimpleStarterGui;

public static class Utils
{
    private static string? _executingAssemblyPath;

    private static string GetFullPathOfExecutingAssembly()
    {
        _executingAssemblyPath ??= Path.GetFullPath(Assembly.GetEntryAssembly()!.Location);
        return _executingAssemblyPath;
    }

    public static string GetFullPathOfExecutingAssemblyWithNewExtension(string iNewExtension, bool appendExtension = false)
    {
        var path = GetFullPathOfExecutingAssembly();
        if (appendExtension)
            return path + $".{iNewExtension}";
        return Path.ChangeExtension(path, iNewExtension);
    }
    
    public static StartInfoModel? ReadStartInfo()
    {
        string? configFile = null;

        // 1st - from commandline
        if (new CommandlineParser(Environment.GetCommandLineArgs().Skip(1)).FindOption("config") is { } cfConfig &&
            File.Exists(cfConfig))
            configFile = cfConfig;

        // 2nd - config from working dir
        if (configFile == null && Path.GetFileName(GetFullPathOfExecutingAssemblyWithNewExtension("config.json")) is
                { } cfWorkingDir && File.Exists(cfWorkingDir))
                configFile = cfWorkingDir;

        // 3rd - nearby executing assembly
        if (configFile == null && GetFullPathOfExecutingAssemblyWithNewExtension("config.json") is { } cf &&
            File.Exists(cf))
            configFile = cf;

        DebugLoggerFactory.GetOrCreate().Information($"Configuration: {configFile}");

        if (configFile == null)
            return null;

        try
        {
            var content = File.ReadAllText(configFile);
            var r = JsonSerializer.Deserialize<StartInfoModel>(content);
            if (r != null)
                r.FullConfigPath = configFile;
            return r;
        }
        catch (Exception ex)
        {
            DebugLoggerFactory.GetOrCreate().Error(ex);
            return null;
        }
    }
}