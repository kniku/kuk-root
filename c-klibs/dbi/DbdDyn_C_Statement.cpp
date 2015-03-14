#include "DBD_Interface.h"

namespace klib {
namespace dbi {

static void bindinfo_destroyer(void *iElem)
{
	DbdDyn_T_Binding *p = (DbdDyn_T_Binding *)iElem;
	if (p) {
		if (p->appBuffer && p->isAllocApp) delete (char *)p->appBuffer;
		if (p->bindBuffer && p->isAllocBind) delete (char *)p->bindBuffer;
		if (p->nullIndicator) delete (char *)p->nullIndicator;
	}
}


DbdDyn_C_Statement::DbdDyn_C_Statement(DbdDyn_C_Connection *iConnection)
: m_bindResultArray(sizeof(DbdDyn_T_Binding), 10, 0, bindinfo_destroyer),
  m_bindParamArray(sizeof(DbdDyn_T_Binding), 10, 0, bindinfo_destroyer)
{
	m_DriverInternals = 0;
	m_hConnection = iConnection;
}

} // namespace dbi
} // namespace

//eof
