#include "pklibbase.h"
#include <iostream>
//#include <boost/date_time/gregorian/gregorian.hpp>
//#include <boost/date_time/posix_time/posix_time_types.hpp>
//#include <boost/format.hpp>

using namespace std;
namespace klib {
namespace base {

CLogWriter::CLogWriter(const std::string &iName) {
	m_Name = iName;
	m_LogLevel = LogLevel_Trace;
}

const std::string &CLogWriter::getName(void) const {
	return m_Name;
}

LogLevel CLogWriter::getLogLevel(void) const {
	return m_LogLevel;
}

LogLevel CLogWriter::setLogLevel(LogLevel iLevel) {
	LogLevel r = m_LogLevel;
	m_LogLevel = iLevel;
	return r;
}

void CLogWriter::writeMessage(LogLevel iLogLevel, const std::string &iMsg) const {
	if (iLogLevel == LogLevel_Always || m_LogLevel <= iLogLevel) {
		write(iLogLevel, iMsg);
	}
}

// some std writers

CLogWriter_stdout::CLogWriter_stdout(const std::string &iName)
: CLogWriter(iName)
{
}

void CLogWriter_stdout::write(LogLevel iLogLevel, const std::string &iMsg) const {
//	boost::posix_time::ptime ts = boost::posix_time::second_clock::local_time();
//	cout << stdDateToStr(ts.date()) << ',' << stdTimeToStr(ts.time_of_day()) << '(' << LogLevelToChar(iLogLevel) << ')' << ' ' << iMsg;
	cout << iMsg;
}

CLogWriter_file::CLogWriter_file(const std::string &iName, const std::string &iFilename)
: CLogWriter(iName)
{
	m_FileName = iFilename;
	m_FileHandle = fopen(iFilename.c_str(), "at");
}

CLogWriter_file::~CLogWriter_file() {
	if (m_FileHandle) fclose(m_FileHandle);
}

void CLogWriter_file::write(LogLevel iLogLevel, const std::string &iMsg) const {
//	boost::posix_time::ptime ts = boost::posix_time::second_clock::local_time();
//	std::string s = stdDateToStr(ts.date()) + ',' + stdTimeToStr(ts.time_of_day()) + '(' + LogLevelToChar(iLogLevel) + ')';
//	fprintf(m_FileHandle, "%s %s", s.c_str(), iMsg.c_str());
	fprintf(m_FileHandle, "%s", iMsg.c_str());
}

void CLogWriter_file::flush(void) const {
	if (m_FileHandle) fflush(m_FileHandle);
}
} // namespace
} // namespace
// eof
