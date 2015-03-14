@echo off
setlocal enabledelayedexpansion

if "%QTDIR%" equ "" (
	echo QTDIR nicht gesetzt.
	echo QT Installationsverzeichnis angeben:
	set /p QTDIR=QTDIR=
)
if not exist %QTDIR%\bin\nul (
	echo QTDIR ungueltig: %QTDIR%
	goto ende
)

del companylog_dist.zip
md dist_tmp
cd dist_tmp
xcopy ..\bin\*.* /e
copy %qtdir%\bin\QtCore*4.dll
copy %qtdir%\bin\QtGui*4.dll
copy %qtdir%\bin\QtNetwork*4.dll
copy %qtdir%\bin\QtXml*4.dll
md imageformats
copy %qtdir%\plugins\imageformats\qjpeg*4.dll imageformats\

zip -r ..\companylog_dist.zip *

cd ..
rd /s /q dist_tmp

:ende
