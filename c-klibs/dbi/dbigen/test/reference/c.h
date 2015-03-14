#ifndef _DBIGEN_957289677_H
#define _DBIGEN_957289677_H
/*
 * DBD-GenCDecl auto generated header file
 */

namespace DbiGen_Mod1 {

// maximum length of string types
#define MAX_LEN_Typ1		10
#define MAX_LEN_Typ4		35

// TestStruct 3
struct D_S3 {
  char S3C1[MAX_LEN_Typ1+1];	// Spalte 1
  char S3C2[MAX_LEN_Typ4+1];	// Spalte 2// TestStruct 3
};
class S3 : public Dbi::CStdRecord {
private:
 D_S3 Data;
public:
 static const unsigned int cntColumns = 2;
 virtual Dbi::BindDescPtr getBindDesc(const unsigned int iIndex) const;
 virtual void *getStdData(void) const;
 struct D_S3 *getData(void) const;
 virtual void reset(void);
};

// TestStruct 2
struct D_S2 {
  char S2C1[MAX_LEN_Typ1+1];	// Spalte 1
  Dbi::DateTime S2C2;	// Spalte 2
  Dbi::DateTime S2C3;	// Spalte 3// TestStruct 2
};
class S2 : public Dbi::CStdRecord {
private:
 D_S2 Data;
public:
 static const unsigned int cntColumns = 3;
 virtual Dbi::BindDescPtr getBindDesc(const unsigned int iIndex) const;
 virtual void *getStdData(void) const;
 struct D_S2 *getData(void) const;
 virtual void reset(void);
};

// TestStruct 1
struct D_S1 {
  long S1C0;	// automatic id test
  char S1C1[MAX_LEN_Typ1+1];	// Spalte 1
  double S1C2;	// Spalte 2
  Dbi::DateTime S1C3;	// Spalte 3
  char S1C4[MAX_LEN_Typ4+1];	// Spalte 4
  char S1C5[MAX_LEN_Typ4+1];	// Spalte 5
  char S1C6[MAX_LEN_Typ4+1];	// Spalte 6// TestStruct 1
};
class S1 : public Dbi::CStdRecord {
private:
 D_S1 Data;
public:
 static const unsigned int cntColumns = 7;
 virtual Dbi::BindDescPtr getBindDesc(const unsigned int iIndex) const;
 virtual void *getStdData(void) const;
 struct D_S1 *getData(void) const;
 virtual void reset(void);
};

void RegisterTablesOf_Mod1(void);

} // namespace DbiGen
#endif // _DBIGEN_957289677_H
// eof
