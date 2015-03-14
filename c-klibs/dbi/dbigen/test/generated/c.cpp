/*
 * DBD-GenCDecl auto generated source file
 */
#define DBI_GENERATED_CODE
#include <CppDBI.h>
#include "c.h"
namespace DbiGen_Mod1 {

// TestStruct 3
const Dbi::BindDesc BD_S3[] = {
 {Dbi::DataType_String, "S3C1", MAX_LEN_Typ1+1, offsetof(D_S3, S3C1)},
 {Dbi::DataType_String, "S3C2", MAX_LEN_Typ4+1, offsetof(D_S3, S3C2)},
 {Dbi::DataType_None}
};
Dbi::BindDescPtr S3::getBindDesc(const unsigned int iIndex) const {
 if (iIndex < cntColumns) return &BD_S3[iIndex];
 else return 0;
}
void *S3::getStdData(void) const {
 return (void *)&Data;
}
struct D_S3 *S3::getData(void) const {
 return (struct D_S3*)&Data;
}
void S3::reset(void) {
 memset(&Data, 0, sizeof(Data));
}

// TestStruct 2
const Dbi::BindDesc BD_S2[] = {
 {Dbi::DataType_String, "S2C1", MAX_LEN_Typ1+1, offsetof(D_S2, S2C1)},
 {Dbi::DataType_DateTime, "S2C2", sizeof(Dbi::DateTime), offsetof(D_S2, S2C2)},
 {Dbi::DataType_DateTime, "S2C3", sizeof(Dbi::DateTime), offsetof(D_S2, S2C3)},
 {Dbi::DataType_None}
};
Dbi::BindDescPtr S2::getBindDesc(const unsigned int iIndex) const {
 if (iIndex < cntColumns) return &BD_S2[iIndex];
 else return 0;
}
void *S2::getStdData(void) const {
 return (void *)&Data;
}
struct D_S2 *S2::getData(void) const {
 return (struct D_S2*)&Data;
}
void S2::reset(void) {
 memset(&Data, 0, sizeof(Data));
}

// TestStruct 1
const Dbi::BindDesc BD_S1[] = {
 {Dbi::DataType_Long, "S1C0", sizeof(long), offsetof(D_S1, S1C0)},
 {Dbi::DataType_String, "S1C1", MAX_LEN_Typ1+1, offsetof(D_S1, S1C1)},
 {Dbi::DataType_Double, "S1C2", sizeof(double), offsetof(D_S1, S1C2)},
 {Dbi::DataType_DateTime, "S1C3", sizeof(Dbi::DateTime), offsetof(D_S1, S1C3)},
 {Dbi::DataType_String, "S1C4", MAX_LEN_Typ4+1, offsetof(D_S1, S1C4)},
 {Dbi::DataType_String, "S1C5", MAX_LEN_Typ4+1, offsetof(D_S1, S1C5)},
 {Dbi::DataType_String, "S1C6", MAX_LEN_Typ4+1, offsetof(D_S1, S1C6)},
 {Dbi::DataType_None}
};
Dbi::BindDescPtr S1::getBindDesc(const unsigned int iIndex) const {
 if (iIndex < cntColumns) return &BD_S1[iIndex];
 else return 0;
}
void *S1::getStdData(void) const {
 return (void *)&Data;
}
struct D_S1 *S1::getData(void) const {
 return (struct D_S1*)&Data;
}
void S1::reset(void) {
 memset(&Data, 0, sizeof(Data));
}

Dbi::BindDescPtr PK_T2[] = {&BD_S2[0],0};
Dbi::BindDescPtr PK_TX[] = {&BD_S1[2],0};
Dbi::BindDescPtr PK_T1[] = {&BD_S1[1],&BD_S1[2],0};
const Dbi::TableDesc TD[] = {
// tablename, primary key binddesc, binddesc, length primary key, length binddesc
{"T2", PK_T2, BD_S2, 1, 3},
{"TX", PK_TX, BD_S1, 1, 7},
{"T1", PK_T1, BD_S1, 2, 7},
{0}};

void RegisterTablesOf_Mod1(void) {
 Dbi::RegisterTables(TD);
}


} // namespace DbiGen
// eof
