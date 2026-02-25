Überwacht Verzeichnisse auf löschbare Objekte (Name=2DEL*DEL2)

Als Service installieren (zB pwsh als Admin):
```
sc.exe create "AsyncDeletionMonitor" binPath= "C:\Views\kuk\Repos\kuk-root\AsyncDeletionMonitor\AsyncDeletionMonitor\bin\Release\net9.0\AsyncDeletionMonitor.exe" DisplayName= "Asynchroneous Deletion Monitor (@Kurt)" start=demand
```
Deinstallieren (zB pwsh als Admin):
```
sc.exe delete "AsyncDeletionMonitor"
```
