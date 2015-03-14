#ifndef _BASE_H
#define _BASE_H

#include <stdarg.h>
#include <time.h>
#include <string>

#if (WIN32)
#	define SYSTEM_WIN32 1
#endif

// check system type
#if (SYSTEM_WIN32==0) && (SYSTEM_LINUX==0)
#	error ##### can't determine system type! #####
#endif

#ifdef SYSTEM_WIN32
#	ifdef BASE_EXPORTS
#		define BASEDLLCALL	__declspec( dllexport )
#	else
#		define BASEDLLCALL	__declspec( dllimport )
#	endif
#else
#	define BASEDLLCALL
#endif

namespace klib {
namespace base {

// *******************************************
// Version stuff
// *******************************************
#define KLIBS_NAME				"klib"
#define KLIBS_VERSION_MAJOR		1
#define KLIBS_VERSION_MINOR		1
#define KLIBS_VERSION_REVISION	0
#define KLIBS_VERSION			(KLIBS_VERSION_MAJOR*1000000 + KLIBS_VERSION_MINOR*1000 + KLIBS_VERSION_REVISION)
BASEDLLCALL const std::string &LibDependencies(void);

// *******************************************
// BArray class
// *******************************************

typedef void (*BArray_cbDestroy)(void *iElem);
typedef bool (*BArray_cbInit)(void *iElem);
typedef int (*BArray_cbCompare)(const void *iElem1, const void *iElem2);

// simple pointer array without any overhead
typedef struct PtrArrayRec *PtrArray;
BASEDLLCALL PtrArray PtrArrayCreate(void);										// create an array
BASEDLLCALL PtrArray PtrArrayDestroy(PtrArray iArray, BArray_cbDestroy iDestroyer);	// destroy iArray; returns 0
BASEDLLCALL PtrArray PtrArrayAdd(PtrArray iArray, void *iPtr);					// add ptr to iArray; creates array if iArray == 0
BASEDLLCALL void *PtrArrayRemove(PtrArray iArray, void *iPtr);					// remove iPtr from iArray if found; return=iPtr if found, 0 otherwise
BASEDLLCALL void *PtrArrayGetAt(PtrArray iArray, const unsigned int iIndex);	// get ptr on position iIndex of iArray or 0 if not found
BASEDLLCALL unsigned int PtrArrayGetCount(PtrArray iArray);						// returns number of ptr in iArray

class BASEDLLCALL BArray {
private:
	struct BArray_Internals *m_Internals;
public:
	static const long Invalid = -1;
	static const long LastElement = -2;
	// construction
	BArray(unsigned long iElemSize, unsigned long iInitialCount = 10, BArray_cbInit iCbInit = 0, BArray_cbDestroy iCbDestroy = 0, BArray_cbCompare iCbCompare = 0);
	// destruction
	~BArray();
	// getter
	unsigned long GetCount(void) const;
	// add elements
	long Append(void *iElem);
	long InsertAt(void *iElem, unsigned long iIndex);
	long BInsert(void *iElem);
	// get/find elements
	void *GetAt(unsigned long iIndex) const;
	long BFind(void *iElem) const;
	// remove elements
	bool RemoveAt(unsigned long iIndex);
	bool RemoveAll(void);
	bool BRemove(void *iElem);
	// sorting
	void BSort(BArray_cbCompare iCbCompare);
	// iterator
	void *IterateNext(void *iElem) const;
};

class BASEDLLCALL BPtrArray : public BArray {
public:
	BPtrArray(unsigned long iInitialCount = 10, BArray_cbInit iCbInit = 0, BArray_cbDestroy iCbDestroy = 0, BArray_cbCompare iCbCompare = 0);
	long Append(void *iElem);
	long InsertAt(void *iElem, unsigned long iIndex) { return BArray::InsertAt(&iElem, iIndex); }
	long BInsert(void *iElem) { return BArray::BInsert(&iElem); }
	void *GetAt(unsigned long iIndex) const;
	long BFind(void *iElem) const;
	bool BRemove(void *iElem) { return BArray::BRemove(&iElem); }
	void BSort(BArray_cbCompare iCbCompare) { BArray::BSort(iCbCompare); }
	static void *derefPtr(const void *iElem);
	static char *derefCStr(const void *iElem);
	static char *derefCArray(const void *iElem);
	static int comparePtr(const void *iElem1, const void *iElem2);
	static int compareCStr(const void *iElem1, const void *iElem2);
	static int compareCArray(const void *iElem1, const void *iElem2);
};

class BASEDLLCALL BStack : public BArray {
public:
	BStack(unsigned long iElemSize, unsigned long iInitialCount = 10, BArray_cbInit iCbInit = 0, BArray_cbDestroy iCbDestroy = 0);
	long Push(void *iElem);
	bool Pop(void);
	void *Top(void) const;
	void *GetAt(unsigned long iIndex) const;
	bool Clear(void);
};

// *******************************************
// Exception class
// *******************************************

class BASEDLLCALL Exception {
	struct Exception_Internals *m_Internals;
public:
	Exception(const char *iError, const char *iFile = "", const unsigned int iLine = 0, const int iErrNo = -1, const char *iSqlState = "");
	Exception(const Exception &iEx);
	~Exception();
	const std::string &	getError() const;
	const int			getErrno() const;
	const std::string &	getSqlState() const;
	const std::string &	getFile() const;
	const unsigned int	getLine() const;
};

// *******************************************
// Logger class
// *******************************************

//typedef void (*LogWriter)(int iLogLevel, const char *iMsg);
//static const LogWriter LogWriter_Stdout = (LogWriter)(-1);

// Log Level
enum LogLevel {
	LogLevel_Always = 1,
	LogLevel_Trace,
	LogLevel_Debug,
	LogLevel_Info,
	LogLevel_Warning,
	LogLevel_Error,
	LogLevel_Never	// only for use in setLogLevel(); only messages with LogLevel_Always will be written
};

class CLogWriter {
private:
	std::string m_Name;
	LogLevel m_LogLevel;
public:
	BASEDLLCALL CLogWriter(const std::string &iName);
	BASEDLLCALL virtual ~CLogWriter() {}
	// set new LogLevel; returns old level
	BASEDLLCALL LogLevel setLogLevel(LogLevel iLevel);
	// get Name of writer
	BASEDLLCALL const std::string &getName(void) const;
	// get Name of writer
	BASEDLLCALL LogLevel getLogLevel(void) const;
	// std write handler; calls write method
	BASEDLLCALL void writeMessage(LogLevel iLogLevel, const std::string &iMsg) const;
	// virtual write method; must be implemented by derived classes
	BASEDLLCALL virtual void write(LogLevel iLogLevel, const std::string &iMsg) const = 0;
};

class CLogWriter_stdout : public CLogWriter {
public:
	BASEDLLCALL CLogWriter_stdout(const std::string &iName);
	BASEDLLCALL void write(LogLevel iLogLevel, const std::string &iMsg) const;
};

class CLogWriter_file : public CLogWriter {
private:
	std::string m_FileName;
	FILE *m_FileHandle;
public:
	BASEDLLCALL CLogWriter_file(const std::string &iName, const std::string &iFilename);
	BASEDLLCALL ~CLogWriter_file();
	BASEDLLCALL void write(LogLevel iLogLevel, const std::string &iMsg) const;
	BASEDLLCALL void flush(void) const;
};

// log facility
class BASEDLLCALL CLogger {
private:
	struct CLogger_Internals *m_Internals;
public:
	// construction
	CLogger(CLogWriter *iWriter = 0);
	// destruction
	~CLogger();
	// set LogLevel for all writers
	void setLogLevel(const LogLevel iLevel);
	// set log format; recognized patters are: $(Date), $(Time), $(Level) and $(Message); default format="$(Date),$(Time)($(Level)) $(Message)"
	void setLogFormat(const std::string &iFormat);
	// get min of all the writers loglevel
	LogLevel getLogLevelMin(void) const;
	// add a new writer
	void addLogWriter(CLogWriter *iHandler);
	// find a certain writer by name
	CLogWriter *getLogWriter(const std::string &iWriterName) const;
	// add new prefix to the prefix stack
	void pushLogPrefix(const std::string &iPrefix);
	// remove top prefix from prefix stack
	void popLogPrefix(void);
	// simple log write method
	void Log(const LogLevel iLevel, const std::string &iMsg) const;
};

} // namespace
} // namespace

#endif	// _KUKLIB_H

// eof
