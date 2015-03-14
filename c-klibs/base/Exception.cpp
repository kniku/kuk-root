#include "pklibbase.h"
#include <assert.h>
#include <stdarg.h>

namespace klib {
namespace base {

struct Exception_Internals {
	std::string mError;
	std::string mSqlState;
	int mErrno;
	std::string mFile;
	unsigned int mLine;
};

Exception::Exception(const char *iError, const char *iFile, const unsigned int iLine, const int iErrNo, const char *iSqlState)
{
	assert(iError);
	m_Internals = new Exception_Internals;
	if (m_Internals) {
		//memset(m_Internals, 0, sizeof(Exception_Internals));
		m_Internals->mSqlState = iSqlState ? iSqlState : "";
		m_Internals->mError = iError;
		m_Internals->mErrno = iErrNo;
		m_Internals->mFile = iFile ? iFile : "";
		m_Internals->mLine = iLine;
	}
};

Exception::Exception(const Exception &iEx)
{
	m_Internals = new Exception_Internals;
	*m_Internals = *iEx.m_Internals;
/*	m_Internals->mErrno = iEx.m_Internals->mErrno;
	m_Internals->mError = iEx.m_Internals->mError;
	m_Internals->mFile = iEx.m_Internals->mFile;
	m_Internals->mLine = iEx.m_Internals->mLine;
	m_Internals->mSqlState = iEx.m_Internals->mSqlState;
	*/
}

Exception::~Exception()
{
	if (m_Internals) {
		delete m_Internals;
	}
}

const std::string &	Exception::getError() const
{
	return m_Internals->mError;
}

const int Exception::getErrno() const
{
	return m_Internals->mErrno;
}

const std::string &	Exception::getSqlState() const
{
	return m_Internals->mSqlState;
}

const std::string &	Exception::getFile() const
{
	return m_Internals->mFile;
}

const unsigned int	Exception::getLine() const
{
	return m_Internals->mLine;
}

} // namespace
} // namespace
// eof
