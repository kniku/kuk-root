using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading;
using Knk.Base.Logging;

namespace Knk.Base.File
{
    public class WatchedElementChangedEventArgs : EventArgs
    {
        public WatcherChangeTypes ChangeType { get; set; }
        public string FullPath { get; set; }
        public string FullPathOld { get; set; }
    }

    public delegate void WatchedElementChangedEventHandler(object sender, WatchedElementChangedEventArgs e);

    public class DirectoryWatcher : IDisposable
    {
        public string Path { get; }
        public bool Recursive { get; }
        public bool WatchChanges { get; }
        public bool Started { get; private set; }

        private FileSystemWatcher Watcher { get; set; }

        public event WatchedElementChangedEventHandler WatchedElementChanged;
        void NotifyWatchedElementChanged(WatcherChangeTypes changeType, string fullPath, string fullPathOld)
        {
            WatchedElementChanged?.Invoke(this, new WatchedElementChangedEventArgs() {ChangeType = changeType, FullPath = fullPath, FullPathOld = fullPathOld});
        }

        /// <summary>
        /// directory watcher: tracking changes of directory provided (delete, create, changes of files/directories)
        /// attach to WatchedElementChanged event handler to receive changing events
        /// </summary>
        /// <param name="path">path of directory to watch</param>
        /// <param name="watchChanges">if notifications about changes of existing files/dirs wanted</param>
        /// <param name="recursive">recurse subdirectories</param>
        public DirectoryWatcher(string path, bool watchChanges = false, bool recursive = false)
        {
            Path = path;
            Recursive = recursive;
            WatchChanges = watchChanges;
        }

        public void Start()
        {
            if (Started)
                return;

            Started = true;

            Watcher = new FileSystemWatcher();
            Watcher.Path = Path;
            Watcher.IncludeSubdirectories = Recursive;
            Watcher.NotifyFilter = NotifyFilters.FileName | NotifyFilters.DirectoryName;
            if (WatchChanges)
                Watcher.NotifyFilter |= NotifyFilters.LastWrite;

            Watcher.Changed += OnChanged;
            Watcher.Created += OnChanged;
            Watcher.Deleted += OnChanged;
            Watcher.Renamed += OnRenamed;
            Watcher.EnableRaisingEvents = true;
        }

        private void OnChanged(object source, FileSystemEventArgs e)
        {
            NotifyWatchedElementChanged(e.ChangeType, e.FullPath, null);
        }

        private void OnRenamed(object source, RenamedEventArgs e)
        {
            NotifyWatchedElementChanged(e.ChangeType, e.FullPath, e.OldFullPath);
        }

        public void Stop()
        {
            if (!Started)
                return;

            Started = false;

            Watcher.EnableRaisingEvents = false;
            Watcher.Dispose();
            Watcher = null;
        }

        #region IDisposable Support
        public void Dispose()
        {
            Stop();
        }
        #endregion
    }

}
