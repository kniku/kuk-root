namespace SimpleStarterGui;

public class CommandlineParser
{
    private IDictionary<string, string?> _options;

    public CommandlineParser(IEnumerable<string> iCommandlineArgs)
    {
        var args = iCommandlineArgs.ToArray();
        DebugLoggerFactory.GetOrCreate().Debug($"CommandLine: {string.Join("|", args)}");

        _options = new Dictionary<string, string?>();
        foreach (var option in args)
        {
            var tokens = option.Split(new[] { '=' });
            var param = tokens[0].Trim('/', ' ', '\t').ToLower();
            string? paramValue = null;
            if (tokens.Length > 1)
                paramValue = tokens[1].Trim().Trim('"', '\'');
            DebugLoggerFactory.GetOrCreate().Debug($"Option: [{param}]=[{paramValue}]");
            _options.Add(param, paramValue);
        }
    }

    public string? FindOption(string iOption, string? iDefault = null)
    {
        return _options.TryGetValue(iOption.ToLower(), out var optionValue) ? optionValue : iDefault;
    }
}
