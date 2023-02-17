namespace SimpleStarterGui;

public class ExecutionModel
{
    public string Executable { get; set; }
    public string? Arguments  { get; set; }
    public string? WorkingDirectory { get; set; }
    public bool UseShellExecute { get; set; }
}

public class StartInfoEntryModel
{
    public string Title { get; set; }
    public string? Description { get; set; }
    public ExecutionModel? Execution { get; set; }
    public string[]? Executions { get; set; }
}
public class StartInfoModel
{
    public string? Name { get; set; }
    public StartInfoEntryModel[]? StartInfos { get; set; }
    public string? FullConfigPath { get; set; }
}