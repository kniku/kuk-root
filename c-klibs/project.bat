set _PRJROOT=%CD%
set PATH=%PATH%;%_PRJROOT%\base\debug
set PATH=%PATH%;%_PRJROOT%\dbi\debug
set PATH=%PATH%;%_PRJROOT%\dbi\drivers\mysql5\lib\win32
doskey prjroot=cd /d %_PRJROOT%
title WORK: klibs
color 17