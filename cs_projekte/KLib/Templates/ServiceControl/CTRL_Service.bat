@echo off
setlocal

set SERVICE_EXE=ASDService.exe
set SERVICE_NAME=BTA ASD Services
set INSTALLUTIL=installutil.EXE

if "%1" == "install" (
	%INSTALLUTIL% %SERVICE_EXE%
	goto ende
)

if "%1" == "uninstall" (
	%INSTALLUTIL% /u %SERVICE_EXE%
	goto ende
)

if "%1" == "start" (
	net start "%SERVICE_NAME%"
	goto ende
)

if "%1" == "stop" (
	net stop "%SERVICE_NAME%"
	goto ende
)

echo.
echo %0: DCS Service Controller
echo usage: %0 "install|uninstall|start|stop"
echo.

:ende
