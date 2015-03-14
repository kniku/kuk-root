#include "ptools.h"

typedef struct _ItSysDynArrayEntryRec {
	void *elem;	/* data buffer */
	long flags;
	struct _ItSysDynArrayRec *pArray;
} ItSysDynArrayEntryRec,*ItSysDynArrayEntry;

/* simple dynamic array ...*/
typedef struct _ItSysDynArrayRec {
	int cnt;	/* number of elements in buf */
	int max;	/* max number possible in current buf */
	int inccnt;	/* growing rate... */
	int esize;	/* size of an element */
	ItSysDynArrayEntry buf;	/* data buffer */
	/* tmp stuff */
	TDynArraySortCb cbcmp;
	void *calldata;	/* current calldata: sorting,....*/
} ItSysDynArrayRec;

ItSysDynArray DynArrayCreate(int cnt, int inccnt, int esize)
{
	ItSysDynArray rv=NULL;

	if (cnt>0&&inccnt>0&&esize>0) {
		rv=(ItSysDynArray)malloc(sizeof(ItSysDynArrayRec));
		if (rv) {
			memset(rv,0,sizeof(ItSysDynArrayRec));
			rv->esize=esize;
			rv->inccnt=inccnt;
			rv->max=cnt;
			rv->buf=(ItSysDynArrayEntry)malloc(cnt*sizeof(ItSysDynArrayEntryRec));
			if (!rv->buf) {
				free(rv);
				rv=NULL;
			} else {
				memset(rv->buf,0,cnt*sizeof(ItSysDynArrayEntryRec));
			}
		}
	}
	return rv;
}

ItSysDynArray DynArrayDestroy(ItSysDynArray isd)
{
	int i;
	
	if (isd) {
		if (isd->buf) {
			for (i=0;i<isd->cnt;i++) {
				if (isd->buf[i].elem) {
					free(isd->buf[i].elem);
					isd->buf[i].elem=NULL;
				}
			}
			free(isd->buf);
		}
		memset(isd,0,sizeof(ItSysDynArrayRec));
		free(isd);
	}
	return NULL;
}

int DynArrayAppend(ItSysDynArray isd, void *data)
{
	int rv=0;

	if (isd&&data&&isd->buf) {
		if (isd->max<=isd->cnt) {
			isd->buf=realloc(isd->buf,(isd->max+isd->inccnt)*sizeof(ItSysDynArrayEntryRec));
			isd->max+=isd->inccnt;
		}
		if (isd->buf) {
			isd->buf[isd->cnt].elem=malloc(isd->esize);
			if (isd->buf[isd->cnt].elem) {
				memset(isd->buf[isd->cnt].elem,0,isd->esize);
				isd->buf[isd->cnt].pArray=isd;
				memcpy(isd->buf[isd->cnt++].elem,data,isd->esize);

				rv=1;
			}
		}
	}
	return rv;
}

static void _da_moveup(ItSysDynArray isd,int i,int cnt)
{
	int bottom,idx;

	if (isd&&isd->buf&&isd->cnt>=i) {
		bottom=max(0,i-cnt);
		/* free the elements getting lost... */
		for (idx=i-1;idx>bottom;idx--) {
			if (isd->buf[idx].elem) {
				free(isd->buf[idx].elem);
			}
		}
		memmove(&isd->buf[bottom],&isd->buf[i],(isd->cnt-i)*sizeof(ItSysDynArrayEntryRec));
		isd->cnt-=(i-bottom);
	}
}

int DynArrayRemove(ItSysDynArray isd, int i)
{
	int rv=0;

	if (isd&&isd->buf&&isd->cnt>i) {
		_da_moveup(isd,i+1,1);
		rv=1;
	}
	return rv;
}

void *DynArrayDirect(ItSysDynArray isd, int i)
{
	char *rv=NULL;

	if (isd&&i>=0&&i<isd->cnt) {
		rv=isd->buf[i].elem;
	}
	return (void *)rv;
}

static int _qsort_cmp(const void *p1,const void *p2)
{
	int rv=1;

	ItSysDynArrayEntry e1=(ItSysDynArrayEntry)p1,e2=(ItSysDynArrayEntry)p2;
	if (e1&&e1->pArray&&e1->pArray->cbcmp) {
		rv=e1->pArray->cbcmp(p1,p2,e1->pArray->calldata);
	}
	return rv;
}

void DynArrayQSort(ItSysDynArray isd, TDynArraySortCb cb, void *calldata)
{
	if (isd&&cb) {
		isd->calldata=calldata;
		isd->cbcmp=cb;
		qsort(isd->buf,isd->cnt,sizeof(ItSysDynArrayEntryRec),_qsort_cmp);
		isd->calldata=NULL;
		isd->cbcmp=NULL;
	}
}
