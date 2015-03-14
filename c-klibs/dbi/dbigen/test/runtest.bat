@echo off
..\dbigen.py -v>generated\.stdout
diff generated\.stdout reference\.stdout
if not "%errorlevel%" == "0" (
	echo ### TEST FAILED: stdout
) else (
	echo OK: stdout
)
diff generated\c.h reference\c.h
if not "%errorlevel%" == "0" (
	echo ### TEST FAILED: GenCDecl(header)
) else (
	echo OK: GenCDecl(header)
)
diff generated\c.cpp reference\c.cpp
if not "%errorlevel%" == "0" (
	echo ### TEST FAILED: GenCDecl(source)
) else (
	echo OK: GenCDecl(source)
)
diff generated\c_table_mysql.sql reference\c_table_mysql.sql
if not "%errorlevel%" == "0" (
	echo ### TEST FAILED: GenSql(mysql5)
) else (
	echo OK: GenSql(mysql5)
)
diff generated\c_table_oracle.sql reference\c_table_oracle.sql
if not "%errorlevel%" == "0" (
	echo ### TEST FAILED: GenSql(oracle9)
) else (
	echo OK: GenSql(oracle9)
)

:ende