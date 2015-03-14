@echo off
if "%1"=="" goto err
call \\nt03\d_drive\nt-tools\perl\bin\perl.exe \\nt03\vol_i\toolsnt\ecoreg.pl -mqmi %*
goto ende

:err
echo.
echo Schneller Zugriff auf QM Datenbank. (kuk, 2002...)
echo.
echo usage: %0 [option] pattern
echo.
echo mit: -i ... nur intern (Mitarbeiter)
echo      -e ... nur extern (Kunden)
echo      -k ... nur intern (Loginkuerzel)
echo      -f ... extern, fuer bestimmte Firma
echo.
echo Default: Suche ueber alles.
echo Das pattern ist wildcardfaehig.
echo Gesucht wird jeweils ueber Vorname, Nachname und Login (bei Mitarbeitern...)
echo.
echo Beispiele: %0 *stef*
echo            %0 -i *stef*
echo            %0 -k k*k
echo            %0 -f kirch *rad*
echo.
:ende
