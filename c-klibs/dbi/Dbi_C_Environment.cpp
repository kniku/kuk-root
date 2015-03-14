#include "pklibdbi.h"
#include <boost/format.hpp>

namespace klib {
namespace dbi {

static void _StdExceptionHandler(const CEnvironment *iEnv, const klib::base::Exception &ex)
{
	char *lib;
	switch (iEnv->getInterfaceType()) {
		case InterfaceType_mysql5:
			lib = "MySql";
			break;
		case InterfaceType_oracle9:
			lib = "Oracle";
			break;
		default:
			lib = "?";
			break;
	}
	iEnv->getLogHandler()->Log(klib::base::LogLevel_Error, (boost::format(
		"\n===============================================================================\n"
		"*** %s EXCEPTION:\n    %s (%u)\n    %d (%s): \"%s\"\n"
		"===============================================================================\n")
		% lib % ex.getFile() % ex.getLine() % ex.getErrno() % ex.getSqlState() % ex.getError()
		).str());
}

void HandleException(const CEnvironment *iEnv, const klib::base::Exception &ex)
{
	_StdExceptionHandler(iEnv, ex);
	DbiExceptionHandler handler = iEnv->getExceptionHandler();
	if (handler) {
		handler(iEnv, ex);
	} else {
		throw(ex);
	}
}


struct CEnvironment_Internals {
	InterfaceType	m_InterfaceType;
	DbdDyn_C_Environment *				m_Interface;
	DbiExceptionHandler					m_cExcHandler;
	klib::base::CLogger *					m_cLogHandler;
	klib::base::BPtrArray *					m_Connections;
};

// connection destroyer
static void cbDestroy_Connection(void *iElem)
{
	CConnection *pConn = (CConnection *)klib::base::BPtrArray::derefPtr(iElem);
	if (pConn) {
		delete pConn;
	}
}

// ***************************************************************
// construction
// ***************************************************************

CEnvironment::CEnvironment(InterfaceType iInterface, klib::base::CLogger *iLogHandler)
{
	m_Internals = new CEnvironment_Internals;
	if (m_Internals) {
		memset(m_Internals, 0, sizeof(CEnvironment_Internals));
		m_Internals->m_InterfaceType = iInterface;
		m_Internals->m_cLogHandler = iLogHandler;
		if (m_Internals->m_cLogHandler == 0) {
			m_Internals->m_cLogHandler = new klib::base::CLogger;
		}
		try {
			m_Internals->m_Interface = DbdDyn_CreateEnvironment(m_Internals->m_InterfaceType, m_Internals->m_cLogHandler);
			m_Internals->m_Interface->i_init();
			m_Internals->m_Connections = new klib::base::BPtrArray(10, 0, cbDestroy_Connection, klib::base::BPtrArray::comparePtr);
		} catch (klib::base::Exception e) {
			HandleException(this, e);
		}
	}
}

// ***************************************************************
// destruction
// ***************************************************************

CEnvironment::~CEnvironment()
{
	if (m_Internals) {
		if (m_Internals->m_Connections) {
			delete m_Internals->m_Connections;
			m_Internals->m_Connections = 0;
		}
		m_Internals->m_Interface->i_destroy();
		delete m_Internals;
	}
}

// ***************************************************************
// setter
// ***************************************************************

void CEnvironment::setExceptionHandler(DbiExceptionHandler iHandler)
{
	if (m_Internals) m_Internals->m_cExcHandler = iHandler;
}

void CEnvironment::setLogHandler(klib::base::CLogger *iHandler)
{
	if (m_Internals) m_Internals->m_cLogHandler = iHandler;
}

// ***************************************************************
// getter
// ***************************************************************

const InterfaceType CEnvironment::getInterfaceType(void) const
{
	if (m_Internals) return m_Internals->m_InterfaceType;
	else return InterfaceType_None;
}

class DbdDyn_C_Environment *CEnvironment::getInterface() const
{
	if (m_Internals) return m_Internals->m_Interface;
	else return 0;
}

DbiExceptionHandler CEnvironment::getExceptionHandler() const
{
	if (m_Internals) return m_Internals->m_cExcHandler;
	else return 0;
}

klib::base::CLogger *CEnvironment::getLogHandler() const
{
	if (m_Internals) return m_Internals->m_cLogHandler;
	else return 0;
}

// ***************************************************************
// methods
// ***************************************************************

CConnection *CEnvironment::createConnection(const char *iHost, const char *iUser, const char *iPasswd, const char *iDatabase)
{
	CConnection *r = new CConnection(this, iHost, iUser, iPasswd, iDatabase);
	if (r) {
		m_Internals->m_Connections->BInsert(r);
	}
	return r;
}

CConnection *CEnvironment::destroyConnection(CConnection *iConn)
{
//	CConnection *pConn = 0;
	if (iConn) {
		m_Internals->m_Connections->BRemove(iConn);
	}
	return 0;
}

} // namespace dbi
} // namespace

// eof
