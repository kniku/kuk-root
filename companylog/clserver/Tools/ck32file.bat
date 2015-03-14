@echo off

setlocal

set python_call=c:\kuk\tools\Python26\python.exe

rem set CK3PATH=\\nt02\vol_i\ecoserv\ck3
rem set CK3PATH=%ROOT_ECOTOOLS%\ck3_test

if "%ROOT_ECOTOOLS%" equ "" (
	concolor -println "FEHLER: ROOT_ECOTOOLS nicht gesetzt" red
	goto aufruf
)
if "%1" equ "" goto aufruf
if "%2" equ "" goto aufruf
if not "%3" equ "" (
	echo wechsle nach %3...
	pushd %3
)

set CK3PATH=%2

echo generiere summary (html)...
%python_call% %ROOT_ECOTOOLS%\Tools\ck32file.py --ck3path=%CK3PATH% --summary --output=html --summary_cols=6 --filter=%1>%1_Summary.html
echo generiere summary (csv)...
%python_call% %ROOT_ECOTOOLS%\Tools\ck32file.py --ck3path=%CK3PATH% --summary --output=csv --filter=%1
echo generiere details (html)...
%python_call% %ROOT_ECOTOOLS%\Tools\ck32file.py --ck3path=%CK3PATH% --details --output=html --filter=%1>%1_Details.html

if not "%3" equ "" (
	c:\kuk\tools\scite\scite.exe "-check.if.already.open=0" index.html
	popd
)

goto ende

:aufruf
concolor -println "Aufruf: ck32file.bat [Filter] [ck3dir] [install_dir]" green
echo Beispiel: ck32file.bat 200811 %ROOT_ECOTOOLS%\ecoserv_auswertungen\ck3 h:\local\bin\tomcat\webapps\ecoserv_auswertungen
echo ACHTUNG: Einhaengen in index.htm nicht vergessen!
goto ende

:ende