#include "pklibbase.h"

namespace klib {
namespace base {

// ***************************************************************
// construction
// ***************************************************************

BStack::BStack(unsigned long iElemSize, unsigned long iInitialCount, BArray_cbInit iCbInit, BArray_cbDestroy iCbDestroy)
: BArray(iElemSize, iInitialCount, iCbInit, iCbDestroy, 0)
{
};

long BStack::Push(void *iElem)
{
	return BArray::Append(iElem);
}

bool BStack::Pop(void)
{
	return RemoveAt(BArray::LastElement);
}

void *BStack::Top(void) const
{
	return GetAt(BArray::LastElement);
}

void *BStack::GetAt(unsigned long iIndex) const
{
	return BArray::GetAt(iIndex);
}

bool BStack::Clear(void)
{
	return BArray::RemoveAll();
}

} // namespace
} // namespace

// eof
