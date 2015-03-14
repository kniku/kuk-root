#include "pklibbase.h"
#include <stdlib.h>
#include <string.h>

namespace klib {
namespace base {

struct BArray_Internals {
	unsigned long m_ElemSize;
	unsigned long m_ElemCount;
	unsigned long m_AllocCount;
	char *m_Buffer;
	BArray_cbDestroy m_cbDestroy;
	BArray_cbInit m_cbInit;
	BArray_cbCompare m_cbCompare;
};

#define MAXI(a,b) (a>b?a:b)
#define CHECK_INDEX(internals,idx) ((internals) && (internals)->m_ElemCount>(idx))
#define REALLOC(internals, elems_new) \
{ \
	if ((internals) && (elems_new > 0) && ((internals)->m_Buffer == 0) || (internals)->m_AllocCount < ((internals)->m_ElemCount + elems_new)) { \
		(internals)->m_AllocCount = (internals)->m_AllocCount + MAXI((elems_new),1); \
		(internals)->m_Buffer = (char *)realloc((internals)->m_Buffer, (internals)->m_AllocCount * (internals)->m_ElemSize); \
	} \
}
#define ELEM_PTR(internals, idx)	((internals)->m_Buffer + (idx) * (internals)->m_ElemSize)

// ***************************************************************
// construction
// ***************************************************************

BArray::BArray(unsigned long iElemSize, unsigned long iInitialCount, BArray_cbInit iCbInit, BArray_cbDestroy iCbDestroy, BArray_cbCompare iCbCompare)
{
	m_Internals = new BArray_Internals;
	if (m_Internals) {
		m_Internals->m_ElemCount = 0;
		m_Internals->m_ElemSize = iElemSize;
		m_Internals->m_AllocCount = 0;
		m_Internals->m_Buffer = 0;
		REALLOC(m_Internals, iInitialCount);
		m_Internals->m_cbDestroy = iCbDestroy;
		m_Internals->m_cbInit = iCbInit;
		m_Internals->m_cbCompare = iCbCompare;
	}
}

// ***************************************************************
// destruction
// ***************************************************************

BArray::~BArray()
{
	if (m_Internals) {
		RemoveAll();
		if (m_Internals->m_Buffer) free(m_Internals->m_Buffer);
		delete m_Internals;
	}
}

// getter
unsigned long BArray::GetCount(void) const
{
	return m_Internals ? m_Internals->m_ElemCount : 0;
}

// add elements
long BArray::Append(void *iElem)
{
	return InsertAt(iElem, BArray::LastElement);
}

long BArray::InsertAt(void *iElem, unsigned long iIndex)
{
	long r = Invalid;
	char *pE;
	bool r_bool = true;
	if (m_Internals && iElem) {
		REALLOC(m_Internals, 1);
		if (iIndex == (unsigned long )BArray::LastElement || iIndex > m_Internals->m_ElemCount) {
			iIndex = m_Internals->m_ElemCount;
		}
		pE = m_Internals->m_Buffer + iIndex * m_Internals->m_ElemSize;
		if (iIndex != m_Internals->m_ElemCount) {
			memmove(pE + m_Internals->m_ElemSize, pE, (m_Internals->m_ElemCount - iIndex) * m_Internals->m_ElemSize);
		}
		memcpy(pE, iElem, m_Internals->m_ElemSize);
		if (m_Internals->m_cbInit) {
			r_bool = m_Internals->m_cbInit(pE);
		}
		if (r_bool) {
			r = m_Internals->m_ElemCount++;
		}
	}
	return r;
}

long BArray::BInsert(void *iElem)
{
	long r = Invalid;
	if (iElem) {
		if (m_Internals->m_cbCompare) {
			r = BFind(iElem);
			if (r < 0) {
				r = InsertAt(iElem, ~r);
			}
		}
	}
	return r;
}

// get/find elements
void *BArray::GetAt(unsigned long iIndex) const
{
	void *r = 0;
	if (iIndex == (unsigned long )BArray::LastElement) iIndex = m_Internals->m_ElemCount - 1;
	if (CHECK_INDEX(m_Internals, iIndex)) {
		r = ELEM_PTR(m_Internals, iIndex);
	}
	return r;
}

static long _find(void *iElem, char *iBuffer, unsigned long iCount, unsigned long iElemSize, BArray_cbCompare iCbCompare, unsigned long *position)
{
	long r = BArray::Invalid;
	unsigned long splitIdx;
	if (iCount > 0) {
		splitIdx = iCount / 2;
		int cmp = iCbCompare(iBuffer + splitIdx * iElemSize, iElem);
		if (cmp == 0) {
			r = splitIdx;
		} else if (cmp > 0) {
			r = _find(iElem, iBuffer, splitIdx, iElemSize, iCbCompare, position);
		} else {
			*position += splitIdx + 1;
			r = _find(iElem, iBuffer + (splitIdx + 1) * iElemSize, iCount - splitIdx - 1, iElemSize, iCbCompare, position);
			if (r != BArray::Invalid) {
				r += splitIdx + 1;
			}
		}
	}
	return r;
}

long BArray::BFind(void *iElem) const
{
	long r = Invalid;
	unsigned long pos = 0;
	if (iElem && m_Internals && m_Internals->m_cbCompare) {
		r = _find(iElem, m_Internals->m_Buffer, m_Internals->m_ElemCount, m_Internals->m_ElemSize, m_Internals->m_cbCompare, &pos);
		if (r == BArray::Invalid) {
			r = r ^ pos;
		}
	}
	return r;
}

// remove elements
bool BArray::RemoveAt(unsigned long iIndex)
{
	bool r = false;
	void *pE;

	if (iIndex == (unsigned long )LastElement) iIndex = m_Internals->m_ElemCount - 1;
	if (CHECK_INDEX(m_Internals, iIndex)) {
		r = true;
		pE = ELEM_PTR(m_Internals, iIndex);
		if (m_Internals->m_cbDestroy) {
			m_Internals->m_cbDestroy(pE);
		}
		if (r == true) {
			if (iIndex < m_Internals->m_ElemCount - 1) {
				memmove(pE, (char *)pE + m_Internals->m_ElemSize, (m_Internals->m_ElemCount - iIndex - 1) * m_Internals->m_ElemSize);
			}
			m_Internals->m_ElemCount--;
		}
	}
	return r;
}

bool BArray::RemoveAll(void)
{
	bool r = true;
	if (m_Internals->m_cbDestroy) {
		for (unsigned long idx = m_Internals->m_ElemCount; idx > 0 && r == true; idx--) {
			r = RemoveAt(idx-1);
		}
	} else {
		m_Internals->m_ElemCount = 0;
	}
	return r;
}

bool BArray::BRemove(void *iElem)
{
	bool r = false;
	if (iElem) {
		if (m_Internals->m_cbCompare) {
			long idx = BFind(iElem);
			if (idx >= 0) {
				r = RemoveAt(idx);
			}
		}
	}
	return r;
}

// sorting
void BArray::BSort(BArray_cbCompare iCbCompare)
{
	if (m_Internals) {
		if (iCbCompare) m_Internals->m_cbCompare = iCbCompare;
		if (m_Internals->m_cbCompare) {
			qsort(m_Internals->m_Buffer, m_Internals->m_ElemCount, m_Internals->m_ElemSize, m_Internals->m_cbCompare);
		}
	}
}

void *BArray::IterateNext(void *iElem) const
{
	void *r = 0;
	if (CHECK_INDEX(m_Internals, 0)) {	
		if (iElem == 0) {
			r = GetAt(0);
		} else if (iElem >= m_Internals->m_Buffer) {
			r = ELEM_PTR(m_Internals, m_Internals->m_ElemCount - 1);
			if (r > iElem) {
				r = (char *)iElem + m_Internals->m_ElemSize;
			} else {
				r = 0;
			}
		}
	}
	return r;
}

} // namespace
} // namespace

// eof
