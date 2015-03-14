#ifndef _DBI_PCPPDBI_H
#define _DBI_PCPPDBI_H

//#include "klibdbi.h"
#include "DBD_Interface.h"
using namespace std;

#if SYSTEM_WIN32
#	pragma once
#	define WIN32_LEAN_AND_MEAN		// Selten verwendete Teile der Windows-Header nicht einbinden
#elif SYSTEM_LINUX
#	define __stdcall
#	include <dlfcn.h>
#endif

namespace klib {
namespace dbi {

// helper functions
void HandleException(const CEnvironment *iEnv, const klib::base::Exception &ex);
//void HandleLog(const CEnvironment *iEnv, klib::base::LogLevel iLogLevel, const char *iFormat, ...);

} // namespace dbi
} // namespace
#endif
