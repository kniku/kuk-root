rd /s /s testversion
mkdir testversion
cd testversion
copy ..\gui\debug\gui.exe
copy ..\xml2db\debug\xml2db.exe
copy %rootkuk%\programming\work\klibs\base\debug\*.dll
copy %rootkuk%\programming\work\klibs\dbi\debug\*.dll
copy %rootkuk%\programming\extlibs\xerces\bin\*3_0.dll
copy %rootkuk%\programming\work\klibs\dbi\drivers\mysql5\lib\win32\*.dll
copy %rootkuk%\programming\ExtLibs\xtreme\bin\vc90\ToolkitPro1202vc90D.dll
cd..
