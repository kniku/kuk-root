all: obj\tools.obj obj\getopts.obj obj\cfgparse.obj obj\mystring.obj obj\dynarr.obj
	lib /OUT:obj\tools.lib obj\tools.obj obj\getopts.obj obj\cfgparse.obj \
		obj\mystring.obj obj\dynarr.obj

obj\tools.obj: tools.c tools.h ptools.h
	cl /Zi /c /Foobj\tools.obj tools.c
obj\getopts.obj: getopts.c tools.h ptools.h
	cl /Zi /c /Foobj\getopts.obj getopts.c 
obj\cfgparse.obj: cfgparse.c tools.h ptools.h
	cl /Zi /c /Foobj\cfgparse.obj cfgparse.c 
obj\mystring.obj: mystring.c tools.h ptools.h
	cl /Zi /c /Foobj\mystring.obj mystring.c 
obj\dynarr.obj: dynarr.c tools.h ptools.h
	cl /Zi /c /Foobj\dynarr.obj dynarr.c 



