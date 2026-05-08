using System.Collections.Concurrent;

namespace AsyncDeletionMonitor;

public class Worker : BackgroundService
{
    // private const string SearchPattern = "2del*del2";
    private const string SafetyFileName = "2del##del2.txt";
    private bool _whatIfOnly;
    private readonly ILogger<Worker> _logger;
    private readonly List<FileSystemWatcher> _watchers = [];
    private readonly BlockingCollection<FileSystemInfo> _objects2Delete = new ();
    private readonly ConcurrentDictionary<string, int> _errorIndex = new ();

    public Worker(ILogger<Worker> logger, IConfiguration configuration)
    {
        _logger = logger;
        var dirs = configuration.GetSection("Directories").GetChildren().Select(c => c.Value);
        _whatIfOnly = configuration.GetValue("WhatIfOnly", true);
        foreach (var path in dirs)
        {
            var dir = string.IsNullOrEmpty(path) ? null : new DirectoryInfo(path);
            if (dir?.Exists == true && File.Exists(Path.Combine(dir.FullName, SafetyFileName)))
            {
                _logger.LogInformation("Monitoring: {Dir}", dir.FullName);
                InitialScanForDeletion(dir);
                StartWatch(dir);
            }
            else
            {
                _logger.LogError(
                    "Invalid directory: \"{Dir}\" - Ensure directory exists and contains safety file \"{SafetyFile}\"", path,
                    SafetyFileName);
                Environment.Exit(-1);
            }
        }
    }

    private void InitialScanForDeletion(DirectoryInfo directory)
    {
        foreach (var info in directory.EnumerateFileSystemInfos("*", SearchOption.AllDirectories))
            _objects2Delete.Add(info);
    }
    
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
        // watcher.Filter = SearchPattern;
        watcher.IncludeSubdirectories = true;
        watcher.EnableRaisingEvents = true;        
        _watchers.Add(watcher);
    }
    
    private void OnChanged(object source, FileSystemEventArgs e)
    {
        _logger.LogDebug("OnChanged received: {Type} - {Path}", e.ChangeType, e.FullPath);

        if(File.Exists(e.FullPath))
        {
            _objects2Delete.Add(new FileInfo(e.FullPath));
        }
        else if(Directory.Exists(e.FullPath))
        {
            _objects2Delete.Add(new DirectoryInfo(e.FullPath));
        }
        else
        {
            _logger.LogWarning("Invalid path detected: {Path}", e.FullPath);
        }
    }

    private bool SafeDeleteObject(FileSystemInfo fileSystemInfo)
    {
        var r = true;
        try
        {
            if (fileSystemInfo.Exists)
            {
                if (fileSystemInfo is DirectoryInfo directoryInfo)
                {
                    _logger.LogDebug("Deleting directory: {ObjectName}", directoryInfo.FullName);
                    if (!_whatIfOnly)
                        directoryInfo.Delete(true);
                }
                else if (fileSystemInfo is FileInfo fileInfo && fileInfo.Name != SafetyFileName)
                {
                    _logger.LogDebug("Deleting file: {ObjectName}", fileInfo.FullName);
                    if (!_whatIfOnly)
                        fileInfo.Delete();
                }
                else
                {
                    _logger.LogDebug("Skipping file or directory: {ObjectName}", fileSystemInfo.FullName);
                }
            }
        }
        catch (DirectoryNotFoundException)
        {
            // directory has gone somehow
            r = true;
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
                _objects2Delete.Add(objectInfo);
            });
        } else {
            _logger.LogError("Error deleting file after {N} attempts: {ObjectName}", errCount, objectInfo.FullName);
        }
    }
    
    protected override async Task ExecuteAsync(CancellationToken stoppingToken)
    {
        while (!stoppingToken.IsCancellationRequested)
        {
            var objectToDelete = _objects2Delete.Take(stoppingToken);
            if (!SafeDeleteObject(objectToDelete))
                PushToErrorIndex(objectToDelete);

            await Task.Delay(500, stoppingToken);
        }
    }
}