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
	auto && ext = entry.path().extension().string();
	return std::equal(
		ext.begin(),
		ext.end(),
		arr,
		arr + sizeof(arr) - 1U,
		[] (char a, char b) noexcept {	return std::tolower(a) == b;	}
	);
}

}
