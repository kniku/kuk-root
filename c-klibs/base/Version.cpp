#include "pklibbase.h"
#include <boost/format.hpp>
#include <boost/version.hpp>

namespace klib {
namespace base {

const std::string &LibDependencies(void) {
	static std::string s;
	s.clear();
	s = (boost::format("boost v%1%.%2%.%3%") % (BOOST_VERSION/100000) % (BOOST_VERSION/100%1000) % (BOOST_VERSION%100)).str();
	return s;
}

} // namespace dbi
} // namespace

// eof
