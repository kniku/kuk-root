#include "oracle9_pDriver.h"

#if (DBD_ORACLE9)

namespace klib {
namespace dbi {

// ******************************
// DbdDyn_C_Result_oracle9
// ******************************

bool DbdDyn_C_Result_oracle9::i_Next(void)
{
	return getStatement()->i_next_result();
}

DataType DbdDyn_C_Result_oracle9::i_getType(unsigned int idx) const
{
	DataType r = DataType_None;
	DbdDyn_T_Binding *pBind = (DbdDyn_T_Binding *)m_hStmt->m_bindResultArray.GetAt(idx);
	if (pBind) {
		r = pBind->dataType;
	}
	return r;
}

bool DbdDyn_C_Result_oracle9::i_isNull(unsigned int idx) const
{
	bool r = false;
	DbdDyn_T_Binding *pBind = (DbdDyn_T_Binding *)m_hStmt->m_bindResultArray.GetAt(idx);
	if (pBind && pBind->nullIndicator) {
		r = (*(sb2 *)pBind->nullIndicator == -1);
	}
	return r;
}

} // namespace dbi
} // namespace klib

#endif // DBD_ORACLE9

// eof
