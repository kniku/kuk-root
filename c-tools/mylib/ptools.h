#ifndef PTOOLS_H
#define PTOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "tools.h"

#define CF_BUFSIZE 		5120	/* CryptFile buffersize */
#define GO_GOBUFSIZE	2000	/* GetOpts internal buffersize */
#define GO_NARGS_MAX	100		/* max number of args holded in nargs buffer */

/*
 * getopts.c
 */
/*
 * working structure for GetOpt..() routines 
 */
typedef struct _GetOptsRec {
	char os[GO_GOBUFSIZE+1];
	int nargs_cnt;
	char *nargs[GO_NARGS_MAX];
} GetOptsRec;

/*
 * cfgparse.c
 */
#define _CIR_InitSize	10	
#define _CIR_AddSize	5

#endif
