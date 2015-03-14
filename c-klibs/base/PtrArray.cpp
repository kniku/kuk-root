#include "pklibbase.h"
#include <stdlib.h>
#include <string.h>

namespace klib {
namespace base {

#define BLOCK_CNT	10

struct PtrArrayRec {
	unsigned int cnt;
	unsigned int alloc_cnt;
	void **pData;
};

PtrArray PtrArrayCreate(void)
{
	PtrArray r = new PtrArrayRec;
	memset(r, 0, sizeof(PtrArrayRec));
	r->pData = (void **)malloc(BLOCK_CNT * sizeof(void *));
	r->alloc_cnt = BLOCK_CNT;
	return r;
}

/*
PtrArray PtrArrayDestroyWithDelete(PtrArray iArray)
{
	unsigned int i;
	if (iArray) {
		if (iArray->pData) {
			for (i = 0; i < iArray->cnt; i++) {
				if (iArray->pData[i]) delete iArray->pData[i];
			}
		}
	}
	return PtrArrayDestroy(iArray);
}
*/

PtrArray PtrArrayDestroy(PtrArray iArray, BArray_cbDestroy iDestroyer)
{
	unsigned int i;
	if (iArray) {
		if (iArray->pData) {
  			if (iDestroyer) {
				for (i = 0; i < iArray->cnt; i++) {
					iDestroyer(iArray->pData[i]);
				}
			}
			delete iArray->pData;
		}
		delete iArray;
	}
	return 0;
}

PtrArray PtrArrayAdd(PtrArray iArray, void *iPtr)
{
	if (iArray == 0) iArray = PtrArrayCreate();
	if (iArray) {
		if (iArray->cnt == iArray->alloc_cnt) {
			iArray->pData = (void **)realloc(iArray->pData, (iArray->alloc_cnt + BLOCK_CNT) * sizeof(void *));
			iArray->alloc_cnt += BLOCK_CNT;
		}
		iArray->pData[iArray->cnt] = iPtr;
		iArray->cnt++;
	}
	return iArray;
}

// remove iPtr if found in iArray
// returns ipPtr if found (and removed); 0 otherwise
void *PtrArrayRemove(PtrArray iArray, void *iPtr)
{
	void *r = 0;
	unsigned int i;

	if (iArray) {
		for (i = 0; i < iArray->cnt; i++) {
			if (iArray->pData[i] == iPtr) {
				r = iPtr;
				if (i < iArray->cnt) {
					memmove(iArray->pData[i], iArray->pData[i + 1], (iArray->cnt - i) * sizeof(void *));
				}
				iArray->cnt--;
				break;
			}
		}
	}
	return r;
}

// returns pointer at position iIndex or 0 if iIndex is out of range
void *PtrArrayGetAt(PtrArray iArray, const unsigned int iIndex)
{
	void *r = 0;

	if (iArray && iIndex < iArray->cnt) {
		r = iArray->pData[iIndex];
	}
	return r;
}

unsigned int PtrArrayGetCount(PtrArray iArray)
{
	return iArray ? iArray->cnt : 0;
}

} // namespace
} // namespace
// eof
