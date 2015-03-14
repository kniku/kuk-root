#include "pklibdbi.h"

namespace klib {
namespace dbi {

struct CResult_Internals {
	CStatement *m_hStmt;
	DbdDyn_C_Result * m_Interface;
};

#define RESULT	(m_Internals->m_Interface)

// ***************************************************************
// construction
// ***************************************************************

CResult::CResult(CStatement *iStmt)
{
	m_Internals = new CResult_Internals;
	if (m_Internals) {
		memset(m_Internals, 0, sizeof(CResult_Internals));
		m_Internals->m_hStmt = iStmt;
		try {
			if (iStmt && m_Internals->m_Interface) m_Internals->m_Interface = m_Internals->m_hStmt->getInterface()->i_create_result();
		} catch (klib::base::Exception e) {
			HandleException(m_Internals->m_hStmt->getConnection()->getEnvironment(), e);
		}
	}
}

// ***************************************************************
// destruction
// ***************************************************************

CResult::~CResult(void)
{
	try {
		if (m_Internals) {
			m_Internals->m_hStmt->getInterface()->i_destroy_result(m_Internals->m_Interface);
			delete m_Internals;
		}
	} catch (klib::base::Exception e) {
		HandleException(m_Internals->m_hStmt->getConnection()->getEnvironment(), e);
	}
}

// ***************************************************************
// getter
// ***************************************************************

DataType CResult::getType(unsigned int idx) const
{
	DataType r = DataType_None;
	try {
		if (RESULT) r = RESULT->i_getType(idx);
	} catch (klib::base::Exception e) {
		HandleException(m_Internals->m_hStmt->getConnection()->getEnvironment(), e);
	}
	return r;
}

// ***************************************************************
// methods
// ***************************************************************

bool CResult::Next(void) const
{
	bool r = false;
	try {
		if (RESULT) {
			r = m_Internals->m_hStmt->Next();
		}
	} catch (klib::base::Exception e) {
		HandleException(m_Internals->m_hStmt->getConnection()->getEnvironment(), e);
	}
	return r;
}

} // namespace dbi
} // namespace klib

//eof
