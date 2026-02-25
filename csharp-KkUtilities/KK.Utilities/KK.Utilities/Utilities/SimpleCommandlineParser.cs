namespace KK.Utilities.Utilities;

public class SimpleCommandlineParser(string[]? args)
{
    private readonly string[] _args = args ?? [];

    public string? GetArgument(string name, string? defaultValue = null)
    {
        var x = _args.FirstOrDefault(a => a.ToLower().Contains(name.ToLower()));
        if (x == null) return null;
        var xx = x.Split(':', '=');
        return xx.Length > 1 ? xx[1].Trim() : defaultValue;
    }

    public bool GetFlag(string name, bool defaultValue = false)
    {
        var x = _args.FirstOrDefault(a => a.ToLower().Contains(name.ToLower()));
        if (x == null) return defaultValue;
        return true;
    }

    public T? GetArgument<T>(string name, T? defaultValue = default)
    {
        var x = GetArgument(name, null);
        if (x == null) return defaultValue;
        return (T) Convert.ChangeType(x, typeof(T));
    }
}