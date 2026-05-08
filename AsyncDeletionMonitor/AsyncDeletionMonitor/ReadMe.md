Überwacht konfigurierte Verzeichnisse und löscht alles, was reinkommt 

Als Service installieren (zB pwsh als Admin):
```
sc.exe create "AsyncDeletionMonitor" binPath= "C:\Views\kuk\Repos\kuk-root\AsyncDeletionMonitor\AsyncDeletionMonitor\bin\Release\net10.0\AsyncDeletionMonitor.exe" DisplayName= "Asynchroneous Deletion Monitor (@Kurt)" start=delayed-auto
```
Status abfragen:
```
sc.exe query AsyncDeletionMonitor| findstr /i state
```
Deinstallieren (zB pwsh als Admin):
```
sc.exe delete "AsyncDeletionMonitor"
```
