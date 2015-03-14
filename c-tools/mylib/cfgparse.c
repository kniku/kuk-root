#include "ptools.h"

CfgInfo CfgRead(char *filename,CfgInfo ci)
{
	FILE 	*f;
	char	line[256];
	char	*pl,*pline;

	if (filename) {
		if (ci) {
			/*
			 * rereading. only if filestat indicates changes...
			 */
#ifdef _WIN32
			struct _stat	fstat;
		 	if (_stat(filename,&fstat)==0) {
				if (fstat.st_mtime==ci->fstat.st_mtime) return ci;
			}
#else
			return ci;
#endif
			ci=CfgDestroy(ci);
		}
		/*
		 * open file...
		 */
		if ((f=fopen(filename,"rt"))) {
			/*
			 * Ok. initialize memory structures...
			 */
			ci=malloc(sizeof(CfgInfoRec));
			if (ci) {
				memset(ci,0,sizeof(CfgInfoRec));
				ci->buf=malloc(_CIR_InitSize*sizeof(CfgEntryRec));
				if (!ci->buf) return NULL;
				memset(ci->buf,0,_CIR_InitSize*sizeof(CfgEntryRec));
				ci->bufmax=_CIR_InitSize;
				strncpy(ci->filename,filename,255);
#ifdef _WIN32
				_stat(filename,&ci->fstat);
#endif
				/*
				 * Ok. start reading...
				 */
				while (fgets(line,255,f)) {
					/*
					 * trim left/right
					 */
					pline=StrTrim(line,' ');
					if (pline&&*pline!='#') {
						pl=strchr(pline,'=');
						if (pl) {
							*pl=0;
							pl++;
							pl=StrTrimRight(pl,'\n');
							pl=StrTrim(pl,' ');
							pl=StrTrim(pl,'\t');
							pl=StrRemoveQuota(pl);
							if (!pl||!*pl) continue;
							/*
							 * new entry...
							 */
							pline=StrTrim(pline,' ');
							pline=StrTrim(pline,'\t');
							if (ci->bufcnt>=ci->bufmax) {
								ci->buf=realloc(ci->buf,(ci->bufmax+_CIR_AddSize)*sizeof(CfgEntryRec));
								if (!ci->buf) {
									fclose(f);
									return NULL;
								}
								ci->bufmax+=_CIR_AddSize;
							}
							if (ci->bufcnt<ci->bufmax) {
								strncpy(ci->buf[ci->bufcnt].name,pline,50);
								ci->buf[ci->bufcnt].name[50]=0;
								if (*pl>='0'&&*pl<='9') {
									ci->buf[ci->bufcnt].type=CIT_Num;
									ci->buf[ci->bufcnt].data=malloc(sizeof(double));
									*(double *)ci->buf[ci->bufcnt].data=atof(pl);
								} else {
									ci->buf[ci->bufcnt].type=CIT_Str;
									ci->buf[ci->bufcnt].data=malloc(strlen(pl)+1);
									strcpy((char *)ci->buf[ci->bufcnt].data,pl);
								}
								ci->bufcnt++;
							}
						}
					}
				}
			}
			fclose(f);
		}
	}
	return ci;
}

void CfgPrint(CfgInfo ci,char *filename)
{
	int i;
	FILE *f;

	if ((f=fopen(filename,"wt"))) {
		fprintf(f,"current configuration:\n");
		if (ci) {
			for (i=0;i<ci->bufcnt;i++) {
				if (ci->buf[i].data) {
					fprintf(f,"CI %3d: %s=",i,ci->buf[i].name);
					if (ci->buf[i].type==CIT_Num) {
						fprintf(f,"%f (numeric)\n",*(double *)ci->buf[i].data);
					} else if (ci->buf[i].type==CIT_Str) {
						fprintf(f,"\"%s\" (string)\n",(char *)ci->buf[i].data);
					} else {
						fprintf(f,"???\n");
					}
				}
			}
		}
		fclose(f);
	}
}

double *CfgGetNum(CfgInfo ci,char *name)
{
	int i;
	double *rv=NULL;

	if (ci) {
		ci=CfgRead(ci->filename,ci);
		for (i=0;i<ci->bufcnt;i++) {
			if (!strncmp(ci->buf[i].name,name,50)&&
				ci->buf[i].type==CIT_Num) {
				rv=(double *)ci->buf[i].data;
			}
		}
	}
	return rv;
}

char *CfgGetStr(CfgInfo ci,char *name)
{
	int i;
	char *rv=NULL;

	if (ci) {
		ci=CfgRead(ci->filename,ci);
		for (i=0;i<ci->bufcnt;i++) {
			if (!strncmp(ci->buf[i].name,name,50)&&
				ci->buf[i].type==CIT_Str) {
				rv=(char *)ci->buf[i].data;
			}
		}
	}
	return rv;
}

CfgInfo CfgDestroy(CfgInfo ci)
{
	int i;

	if (ci) {
		if (ci->buf) {
			for (i=0;i<ci->bufcnt;i++) {
				if (ci->buf[i].data) {
					free(ci->buf[i].data);
					ci->buf[i].data=NULL;
				}
			}
			free(ci->buf);
			ci->buf=NULL;
		}
		ci->bufcnt=0;
		free(ci);
	}
	return NULL;
}
