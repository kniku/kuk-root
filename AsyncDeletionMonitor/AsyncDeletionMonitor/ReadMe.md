Überwacht Verzeichnisse auf löschbare Objekte (Name=2DEL*DEL2)

Als Service installieren:
```
sc.exe create "AsyncDeletionMonitor" binPath= "C:\tmp\xxx\AsyncDeletionMonitor.exe" DisplayName= "Asynchroneous Deletion Monitor (@Kurt)" start=demand
```
Deinstallieren:
```
sc.exe remove "AsyncDeletionMonitor"
```
