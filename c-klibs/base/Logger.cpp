#include "pklibbase.h"
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>

using namespace std;

namespace klib {
namespace base {

const char *TOK_DATE = "$(Date)";
const char *TOK_TIME = "$(Time)";
const char *TOK_LEVEL = "$(Level)";
const char *TOK_MESSAGE = "$(Message)";

struct CLogger_Internals {
	BPtrArray *m_Writer;
	BPtrArray *m_Prefixes;
	string m_Format;
};

static void _cb_writer_destroy(void *iElem) {
	CLogWriter *iWriter = (CLogWriter *)BPtrArray::derefPtr(iElem);
	if (iWriter) delete iWriter;
}

static void _cb_prefix_destroy(void *iElem) {
	string *iPrefix = (string *)BPtrArray::derefPtr(iElem);
	if (iPrefix) delete iPrefix;
}

// ***************************************************************
// construction
// ***************************************************************

CLogger::CLogger(CLogWriter *iWriter)
{
	m_Internals = new CLogger_Internals;
	if (m_Internals) {
		//memset(m_Internals, 0, sizeof(CLogger_Internals));
		m_Internals->m_Writer = 0;
		m_Internals->m_Prefixes = 0;
		if (iWriter) addLogWriter(iWriter);
		setLogFormat((boost::format("%s,%s(%s) %s") % TOK_DATE % TOK_TIME % TOK_LEVEL % TOK_MESSAGE).str());
	}
}

// ***************************************************************
// destruction
// ***************************************************************

CLogger::~CLogger()
{
	if (m_Internals) {
		if (m_Internals->m_Writer) delete m_Internals->m_Writer;
		if (m_Internals->m_Prefixes) delete m_Internals->m_Prefixes;
		delete m_Internals;
	}
}

// ***************************************************************
// setter
// ***************************************************************

void CLogger::setLogLevel(const LogLevel iLevel)
{
	CLogWriter *writer;
	for (unsigned int i = 0; (writer = (CLogWriter *)m_Internals->m_Writer->GetAt(i)); i++) {
		writer->setLogLevel(iLevel);
	}
}

void CLogger::setLogFormat(const std::string &iFormat) {
	m_Internals->m_Format = iFormat;
	boost::algorithm::replace_all(m_Internals->m_Format, TOK_DATE, "%1%");
	boost::algorithm::replace_all(m_Internals->m_Format, TOK_TIME, "%2%");
	boost::algorithm::replace_all(m_Internals->m_Format, TOK_LEVEL, "%3%");
	boost::algorithm::replace_all(m_Internals->m_Format, TOK_MESSAGE, "%4%");
}

LogLevel CLogger::getLogLevelMin(void) const {
	LogLevel r = LogLevel_Never;
	CLogWriter *writer;
	if (m_Internals->m_Writer) {
		for (unsigned int i = 0; (writer = (CLogWriter *)m_Internals->m_Writer->GetAt(i)); i++) {
			r = min(r, writer->getLogLevel());
		}
	}
	return r;
}

// ***************************************************************
// methods
// ***************************************************************

void CLogger::addLogWriter(CLogWriter *iHandler)
{
	if (iHandler) {
		if (m_Internals->m_Writer == 0) {
			m_Internals->m_Writer = new BPtrArray(0, 0, _cb_writer_destroy, 0);
		}
		m_Internals->m_Writer->Append((void *)iHandler);
	}
}

CLogWriter *CLogger::getLogWriter(const std::string &iWriterName) const {
	CLogWriter *writer = 0;
	if (m_Internals->m_Writer) {
		for (unsigned int i = 0; (writer = (CLogWriter *)m_Internals->m_Writer->GetAt(i)); i++) {
			if (iWriterName == writer->getName()) {
				break;
			}
		}
	}
	return writer;
}

void CLogger::pushLogPrefix(const std::string &iPrefix) {
	string *ps = new string(iPrefix);
	if (m_Internals->m_Prefixes ==  0) {
		m_Internals->m_Prefixes = new BPtrArray(0, 0, _cb_prefix_destroy, 0);
	}
	m_Internals->m_Prefixes->Append(ps);
}

void CLogger::popLogPrefix(void) {
	if (m_Internals->m_Prefixes !=  0) {
		m_Internals->m_Prefixes->RemoveAt(BArray::LastElement);
	}
}

const std::string &stdDateToStr(const boost::gregorian::date &iDate) {
	static std::string s;
	s = (boost::format("%|3$02d|.%|2$02d|.%|1$04d|") % iDate.year() % iDate.month() % iDate.day()).str();
	return s;
}

const std::string &stdTimeToStr(const boost::posix_time::time_duration &iTime) {
	static std::string s;
	s = (boost::format("%|1$02d|:%|2$02d|:%|3$02d|") % iTime.hours() % iTime.minutes() % iTime.seconds()).str();
	return s;
}

static const char *LogLevelToChar(LogLevel iLevel) {
	const char *r = "?";
	switch (iLevel) {
	case LogLevel_Trace:
		r = "T";
		break;
	case LogLevel_Debug:
		r = "D";
		break;
	case LogLevel_Warning:
		r = "W";
		break;
	case LogLevel_Info:
		r = "I";
		break;
	case LogLevel_Error:
		r = "*E";
		break;
	case LogLevel_Always:
		r = "-";
		break;
	default:
		break;
	}
	return r;
}

void CLogger::Log(const LogLevel iLevel, const std::string &iMsg) const
{
	static string s;
	CLogWriter *writer;
	if (m_Internals->m_Writer) {
		if (m_Internals && (iLevel == LogLevel_Always || iLevel >= getLogLevelMin())) {
			if (m_Internals->m_Prefixes !=  0) {
				string *p;
				s.clear();
				for (unsigned int i = 0; (p = (string *)m_Internals->m_Prefixes->GetAt(i)); i++) {				
					s += *p;
				}
				s += iMsg;
			} else {
				s = iMsg;
			}
			boost::posix_time::ptime ts = boost::posix_time::second_clock::local_time();
			s = (boost::format(m_Internals->m_Format) % stdDateToStr(ts.date()) % stdTimeToStr(ts.time_of_day()) % LogLevelToChar(iLevel) % s).str();
			for (unsigned int i = 0; (writer = (CLogWriter *)m_Internals->m_Writer->GetAt(i)); i++) {
				writer->writeMessage(iLevel, s);
			}
		}
	}
}

} // namespace
} // namespace
// eof
