@echo off
color 1f
set ROOTPRJ=%cd%
doskey prj=cd /d %ROOTPRJ%

set PGSQLDATA=%ROOTPRJ%\pgDatabase
