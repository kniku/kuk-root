#define DBI_GENERATED_CODE
#include "pklibdbi.h"

namespace klib {
namespace dbi {

RetValue BindDescGet(BindDescPtr iBindDesc, BindDescResource iWhat) {
	RetValue r = 0;
	if (iBindDesc) {
		switch (iWhat) {
		case BindDescNcolumnName:
			r = (RetValue )(char *)iBindDesc->Name;
			break;
		case BindDescNcolumnOffset:
			r = (RetValue )iBindDesc->Offset;
			break;
		case BindDescNcolumnSize:
			r = (RetValue )iBindDesc->Length;
			break;
		case BindDescNdataType:
			r = (RetValue )iBindDesc->Type;
			break;
		default:
			break;
		}
	}
	return r;
}

static int _cb_cmp(const void *iElem1, const void *iElem2) {
	TableDescPtr iTD1 = (TableDescPtr )klib::base::BPtrArray::derefPtr(iElem1);
	TableDescPtr iTD2 = (TableDescPtr )klib::base::BPtrArray::derefPtr(iElem2);
	return strcmp(iTD1->tableName, iTD2->tableName);
}

static klib::base::BPtrArray regTables(32, 0, 0, _cb_cmp);

void RegisterTables(TableDescPtr iTableDesc) {
	int i;
	for (i = 0; iTableDesc[i].tableName; i++) {
		regTables.BInsert((void *)&iTableDesc[i]);
	}
}

TableDescPtr LookupTable(const char *iTableName) {
	TableDescPtr r = 0;
	struct TableDesc searchRec;
	if (iTableName) {
		searchRec.tableName = iTableName;
		long idx = regTables.BFind(&searchRec);
		if (idx >= 0) r = (TableDescPtr )regTables.GetAt(idx);
	}
	return r;
}

RetValue TableDescGet(TableDescPtr iTableDesc, TableDescResource iWhat, unsigned int iIndex) {
	RetValue r = 0;
	if (iTableDesc) {
		switch (iWhat) {
		case TableDescNtableName:
			r = (RetValue )iTableDesc->tableName;
			break;
		case TableDescNbindColumnCount:
			r = (RetValue )iTableDesc->cntCols;
			break;
		case TableDescNbindColumn:
			if (iIndex < iTableDesc->cntCols) {
				r = (RetValue )&iTableDesc->allCols[iIndex];
			}
			break;
		case TableDescNbindPKColumnCount:
			r = (RetValue )iTableDesc->cntPrimKeyCols;
			break;
		case TableDescNbindPKColumn:
			if (iIndex < iTableDesc->cntPrimKeyCols) {
				r = (RetValue )iTableDesc->primKeyCols[iIndex];
			}
			break;
		default:
			break;
		}
	}
	return r;
}

} // namespace dbi
} // namespace

// eof
