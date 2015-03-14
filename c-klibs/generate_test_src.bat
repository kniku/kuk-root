echo off
if "%_PRJROOT%" == "" (
	echo _PRJROOT not properly set - bye.
	goto :eof
)

pushd %_PRJROOT%\test
python ..\dbi\dbigen\dbigen.py
popd
