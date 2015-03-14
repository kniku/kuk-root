#include "pklibbase.h"
#include <string.h>

namespace klib {
namespace base {

// ***************************************************************
// construction
// ***************************************************************

BPtrArray::BPtrArray(unsigned long iInitialCount, BArray_cbInit iCbInit, BArray_cbDestroy iCbDestroy, BArray_cbCompare iCbCompare)
: BArray(sizeof(void *), iInitialCount, iCbInit, iCbDestroy, iCbCompare)
{
};

// ***************************************************************
// methods
// ***************************************************************

long BPtrArray::Append(void *iElem)
{
	return BArray::Append(&iElem);
}

void *BPtrArray::GetAt(unsigned long iIndex) const
{
	void **r = (void **)BArray::GetAt(iIndex);
	return derefPtr(r);
}

long BPtrArray::BFind(void *iElem) const
{
	return BArray::BFind(&iElem);
}

void *BPtrArray::derefPtr(const void *iElem)
{
	if (iElem) iElem = *(void **)iElem;
	return (void *)iElem;
}

char *BPtrArray::derefCStr(const void *iElem)
{
	if (iElem) iElem = *(void **)iElem;
	return (char *)iElem;
}

char *BPtrArray::derefCArray(const void *iElem)
{
	return (char *)iElem;
}

int BPtrArray::comparePtr(const void *iElem1, const void *iElem2)
{
	int r = -1;
	void *e1 = derefPtr(iElem1);
	void *e2 = derefPtr(iElem2);
	if (e1 == e2) r = 0;
	else if (e1 > e2) r = 1;
	else r = -1;
	return r;
}

int BPtrArray::compareCStr(const void *iElem1, const void *iElem2)
{
	int r = -1;
	char *e1 = derefCStr(iElem1);
	char *e2 = derefCStr(iElem2);
	if (e1 && e2) {
		r = strcmp(e1, e2);
	}
	return r;
}

int BPtrArray::compareCArray(const void *iElem1, const void *iElem2)
{
	int r = -1;
	char *e1 = (char *)derefCArray(iElem1);
	char *e2 = (char *)derefCArray(iElem2);
	if (e1 && e2) {
		r = strcmp(e1, e2);
	}
	return r;
}

} // namespace
} // namespace

// eof
