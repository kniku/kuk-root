@rem = '--*-Perl-*--
@echo off
if "%OS%" == "Windows_NT" goto WinNT
wperl -x -S "%0" %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
:WinNT
start wperl -x -S %0 %*
if NOT "%COMSPEC%" == "%SystemRoot%\system32\cmd.exe" goto endofperl
if %errorlevel% == 9009 echo You do not have Perl in your PATH.
if errorlevel 1 goto script_failed_so_exit_with_non_zero_val 2>nul
goto endofperl
@rem ';
#!/usr/local/bin/perl
#line 15

use Tk;

print "tk version=$Tk::VERSION\n";
$top=MainWindow->new();
$but=$top->Button(-text=>"Hello World",-command=>sub {exit 0});
$but->pack();
MainLoop;

__END__
:endofperl
