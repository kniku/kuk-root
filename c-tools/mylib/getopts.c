#include "ptools.h"
#include <string.h>

#define GO_OPTSTART		'\1'
#define GO_OPTEND		'\2'
#define GO_OPTASSIGN	':'
#define GO_OPTIGNORE	'#'

GetOptsPtr GetOpts(char *olist,int argc,char **argv)
{
	GetOptsPtr 	rv=NULL;
	int			i,j,apos=0;
	char		*p;
	char		*_olist;

	if (olist&&argv) {
		rv=(GetOptsPtr)malloc(sizeof(GetOptsRec));
		_olist=(char *)malloc(strlen(olist)+1);
		if (rv&&_olist) {
			strcpy(_olist,olist);
			memset(rv,0,sizeof(GetOptsRec));
			for (i=1;i<argc&&argv[i];i++) {
				if (argv[i][0]=='-') {
					if (!argv[i][1]) continue;
					j=1;
					do {
						if ((p=strchr(_olist,argv[i][j]))) {
							sprintf(&(rv->os[apos]),"%c%c1%c",GO_OPTSTART,argv[i][j],GO_OPTASSIGN);
							apos+=4;
							*p=GO_OPTIGNORE;
							if (*(p+1)==GO_OPTASSIGN) {
								if (argv[i][j+1]) {
									strcpy(&(rv->os[apos]),&argv[i][j+1]);
									apos+=strlen(&argv[i][j+1]);
									rv->os[apos++]=GO_OPTEND;
									j=strlen(argv[i]);
									continue;
								} else {
									i++;
									j=0;
									if (argv[i]) {
										if (argv[i][j]!='-') {
											strcpy(&(rv->os[apos]),&argv[i][j]);
											apos+=strlen(&argv[i][j]);
											rv->os[apos++]=GO_OPTEND;
											j=strlen(argv[i]);
										} else {
											rv->os[apos++]=GO_OPTEND;
											j=1;
										}
										continue;
									} else {
										rv->os[apos++]=GO_OPTEND;
										break;
									}
								}
							} else {
								rv->os[apos++]=GO_OPTEND;
							}
						}
						j++;
					} while (argv[i][j]);
				} else {
					/*
					 * normaler parameter...
					 */
					if (rv->nargs_cnt<GO_NARGS_MAX-1) {
						rv->nargs[rv->nargs_cnt++]=argv[i];
					}
				}
			}
			for (i=0;i<(int )strlen(_olist);i++) {
				if (_olist[i]!=GO_OPTIGNORE&&_olist[i]!=GO_OPTASSIGN) {
					sprintf(&rv->os[apos],"%c%c0%c%c",GO_OPTSTART,_olist[i],GO_OPTASSIGN,GO_OPTEND);
					apos+=5;
				}
			}
			free(_olist);
		}
	}
	return rv;
}

int GetOpt(GetOptsPtr go, char *opt, char*o, int ol)
{
	int 	rv=0;
	char 	_s[10];
	char 	*apos;

	if (go&&opt) {
		if (o&&ol>0) memset(o,0,ol);
		sprintf(_s,"%c1%c",*opt,GO_OPTASSIGN);
		if ((apos=strstr(go->os,_s))) {
			if (o&&ol>0) {
				apos+=3;
				for (rv=0;rv<ol-1&&*apos!=GO_OPTEND;rv++,apos++) {
					o[rv]=*apos;
				}
			}
			rv=1;
		}
	}
	return rv;
}

char *GetArg(GetOptsPtr go, int i)
{
	char *rv=NULL;

	if (go) {
		if (i<GO_NARGS_MAX) rv=go->nargs[i];
	}
	return rv;
}

GetOptsPtr GetOptsDestroy(GetOptsPtr go)
{
	if (go) free(go);
	return NULL;
}















