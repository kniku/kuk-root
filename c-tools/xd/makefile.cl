TOOLDIR=..\mylib

all: obj\xd.exe

obj\xd.exe: xd.c
	cl /I$(TOOLDIR) /Foobj\xd.obj xd.c /link $(TOOLDIR)\obj\tools.lib /out:obj\xd.exe




