#ifndef TOOLS_H
#define TOOLS_H

#ifdef _WIN32
#	include <sys\types.h>
#	include <sys\stat.h>
#endif

/*
 * tools.c
 * ---------
 * generic tool functions
 */
#define HD_CTRL_Offs	0x1L	/* create offsets */
#define HD_CTRL_OffsHex	0x2L	/* .. as hex values */
#define HD_CTRL_OffsDec	0x4L	/* .. as dec values */
#define HD_CTRL_Ascii	0x8L	/* add ascii characters */

#define HD_HDBUFSIZE(bsize,lines)	((bsize/lines+1)*(12+4*lines+1+1))
	
void Bin2HexDump(void *buf,char *hd,long bufsize, int umbruch, long *offs, int ctrl);
void HexDumpFile(char *fn, int line, int ctrl);

int CryptFile(char *fn,char *cfn,char *pwd);

/*
 * getopts.c
 * ---------
 * parsing cmdl-options (simple flags like -cx
 * or with ONE following parameter like -f FILENAME
 * (see perl: getopts())
 */
typedef struct _GetOptsRec *GetOptsPtr;

GetOptsPtr GetOpts(char *olist,int argc,char **argv);
int GetOpt(GetOptsPtr go, char *opt,char*o,int ol);
char * GetArg(GetOptsPtr go, int i);
GetOptsPtr GetOptsDestroy(GetOptsPtr go);

/*
 * cfgparse.c
 * ---------
 * parsing config file
 * line syntax: key=value (numeric or string) or key="strvalue"
 * use '#' to comment out lines...
 */
typedef struct _CfgEntryRec {
	int type;
	void *data;
	char name[51];
} CfgEntryRec,*CfgEntry;

typedef struct _CfgInfoRec {
	int 		bufcnt;
	int 		bufmax;
	CfgEntry	buf;
#ifdef _WIN32
	struct _stat fstat;
#endif
	char		filename[256];
} CfgInfoRec,*CfgInfo;

#define CIT_Num			1
#define CIT_Str			2

CfgInfo CfgRead(char *filename,CfgInfo ci);
void CfgPrint(CfgInfo ci, char *filename);
double *CfgGetNum(CfgInfo ci,char *name);
char *CfgGetStr(CfgInfo ci,char *name);
CfgInfo CfgDestroy(CfgInfo ci);

/*
 * mystring.c
 * ---------
 * string tool functions
 */
void StrRemoveChar(char *s,char c);
char *StrTrimLeft(char *s,char c);
char *StrTrimRight(char *s,char c);
char *StrTrim(char *s,char c);
char *StrRemoveQuota(char *s);

/*
 * dynarr.c
 * ---------
 * simple dynamic array
 */
typedef struct _ItSysDynArrayRec *ItSysDynArray;
typedef int (*TDynArraySortCb)(const void *p1, const void *p2, const void *calldata);
ItSysDynArray DynArrayCreate(int cnt, int inccnt, int esize);
ItSysDynArray DynArrayDestroy(ItSysDynArray isd);
int DynArrayAppend(ItSysDynArray isd, void *data);
int DynArrayRemove(ItSysDynArray isd, int i);
void *DynArrayDirect(ItSysDynArray isd, int i);
void DynArrayQSort(ItSysDynArray isd, TDynArraySortCb cb, void *calldata);

#endif






