namespace DirectoryMonitor;

public class Worker : BackgroundService
{
    // private const string SearchPattern = "2del*del2";
    private readonly ILogger<Worker> _logger;
    // private readonly List<FileSystemWatcher> _watchers = [];
    // private readonly BlockingCollection<FileSystemInfo> _objects2delete = new ();
    // private readonly ConcurrentDictionary<string, int> _errorIndex = new ();

    public Worker(ILogger<Worker> logger, IConfiguration configuration)
    {
        _logger = logger;
        var dirs = configuration.GetSection("Directories").GetChildren().Select(c => c.Value);
        foreach (var path in dirs)
        {
            var dir = string.IsNullOrEmpty(path) ? null : new DirectoryInfo(path);
            if (dir?.Exists == true)
            {
                _logger.LogInformation("Monitoring: {Dir}", dir.FullName);
                // InitialScanForDeletion(dir);
                StartWatch(dir);
            }
            else
            {
                _logger.LogWarning("Invalid directory: {Dir}", path);
            }
        }
    }

    // private void InitialScanForDeletion(DirectoryInfo directory)
    // {
    //     foreach (var info in directory.EnumerateFileSystemInfos(SearchPattern, SearchOption.AllDirectories))
    //         _objects2delete.Add(info);
    // }
    
    private void StartWatch(DirectoryInfo directory)
    {
        var watcher = new FileSystemWatcher();
        watcher.Path = directory.FullName;
        // watcher.NotifyFilter = NotifyFilters.Attributes
        //                        | NotifyFilters.CreationTime
        //                        | NotifyFilters.DirectoryName
        //                        | NotifyFilters.FileName
        //                        | NotifyFilters.LastAccess
        //                        | NotifyFilters.LastWrite
        //                        | NotifyFilters.Security
        //                        | NotifyFilters.Size;
        watcher.NotifyFilter = NotifyFilters.CreationTime
                              | NotifyFilters.DirectoryName
                              | NotifyFilters.FileName;
        watcher.Created += OnChanged;
        watcher.Renamed += OnChanged;
        watcher.Deleted += OnChanged;
        // watcher.Filter = SearchPattern;
        watcher.IncludeSubdirectories = true;
        watcher.EnableRaisingEvents = true;        
        // _watchers.Add(watcher);
    }
    
    private void OnChanged(object source, FileSystemEventArgs e)
    {
        _logger.LogInformation("OnChanged received: {Type} - {Path}", e.ChangeType, e.FullPath);
    }
#if false
    private bool SafeDeleteObject(FileSystemInfo fileSystemInfo)
    {
        var r = true;
        try
        {
            _logger.LogInformation("Deleting file or directory: {ObjectName}", fileSystemInfo.FullName);
            if (fileSystemInfo.Exists)
            {
                if (fileSystemInfo is DirectoryInfo directoryInfo)
                {
                    directoryInfo.Delete(true);
                }
                else
                {
                    fileSystemInfo.Delete();
                }
            }
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error deleting file or directory: {DirName}", fileSystemInfo.FullName);
            r = false;
        }

        return r;
    }

    private void PushToErrorIndex(FileSystemInfo objectInfo)
    {
        var errCount = _errorIndex.GetOrAdd(objectInfo.FullName, 0);
        if (errCount < 10)
        {
            // try it again later
            _ = Task.Run(async () =>
            {
                await Task.Delay(TimeSpan.FromMinutes(.1));
                _objects2delete.Add(objectInfo);
            });
        } else {
            _logger.LogError("Error deleting file after {N} attempts: {ObjectName}", errCount, objectInfo.FullName);
        }
    }
    
#endif
    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        while (!stoppingToken.IsCancellationRequested)
        {
            // var objectToDelete = _objects2delete.Take(stoppingToken);
            // if (!SafeDeleteObject(objectToDelete))
            //     PushToErrorIndex(objectToDelete);

            await Task.Delay(500, stoppingToken);
        }
    }
}