TOOLDIR=..\mylib

all: obj\crypt.exe

obj\crypt.exe: crypt.c
	cl /I$(TOOLDIR) /Foobj\crypt.obj crypt.c /link $(TOOLDIR)\obj\tools.lib /out:obj\crypt.exe


