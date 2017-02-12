#include <boost/filesystem.hpp>
#include <module_loader/shared_library_directory_entry_filter.hpp>
#include <algorithm>
#include <cctype>

namespace module_loader {

bool shared_library_directory_entry_filter::check (const boost::filesystem::directory_entry & entry) {
	if (!boost::filesystem::is_regular_file(entry.symlink_status())) return false;
	const char arr [] = "."
	#ifdef _WIN32
	"dll"
	#else
	"so"
	#endif
	;
	//	This is necessary (as opposed to one long
	//	expression) otherwise there's undefined
	//	behavior with Boost.Filesystem from Boost
	//	1.54.0.
	//
	//	From reading the documentation:
	//
	//	-	boost::filesystem::directory_entry::path
	//		returns by reference
	//	-	boost::filesystem::path::extension returns
	//		by value
	//	-	boost::filesystem::path::string returns by
	//		value
	//
	//	and therefore the following:
	//
	//	auto && ext_str = entry.path().extension().string();
	//
	//	Should not be problematic: A temporary will be
	//	created by the call to boost::filesystem::path::
	//	extension and will be destroyed at the end of
	//	the full expression, but another temporary will
	//	have been generated (and returned as the value
	//	of the full expression) by boost::filesystem::
	//	path::extension so there should be no UB.
	//
	//	Apparently Boost.Filesystem from Boost 1.54.0
	//	does not follow the documentation, and therefore
	//	we must structure the code like this
	auto && path = entry.path();
	auto && ext = path.extension();
	auto && ext_str = ext.string();
	return std::equal(
		ext_str.begin(),
		ext_str.end(),
		arr,
		arr + sizeof(arr) - 1U,
		[] (char a, char b) noexcept {	return std::tolower(a) == b;	}
	);
}

}
