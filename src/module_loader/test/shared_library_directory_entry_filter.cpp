#include <module_loader/shared_library_directory_entry_filter.hpp>
#include <boost/filesystem.hpp>
#include <module_loader/whereami.hpp>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::shared_library_directory_entry_filter rejects boost::filesystem::directory_entry objects which do not represent a regular file with an extension appropriate for a shared library on that platform","[module_loader][shared_library_directory_entry_filter]") {
	GIVEN("A module_loader::shared_library_directory_entry_filter object") {
		shared_library_directory_entry_filter filter;
		WHEN("module_loader::shared_library_directory_entry_filter::check is invoked with a boost::filesystem::directory_entry which represents a directory") {
			boost::filesystem::directory_entry entry(current_executable_directory_path());
			bool val = filter.check(entry);
			THEN("It is rejected") {
				CHECK_FALSE(val);
			}
		}
		WHEN("module_loader::shared_library_directory_entry_filter::check is invoked with a boost::filesystem::directory_entry which represents a regular file which does not have an appropriate extension for a shared library") {
			boost::filesystem::directory_entry entry(current_executable_path());
			bool val = filter.check(entry);
			THEN("It is rejected") {
				CHECK_FALSE(val);
			}
		}
		WHEN("module_loader::shared_library_directory_entry_filter::check is invoked with a boost::filesystem::directory_entry which represents a regular file which has an appropriate extension for a shared library") {
			boost::filesystem::directory_entry entry(current_executable_directory_path() / "libmodule_loader."
			#ifdef _WIN32
			"dll"
			#else
			"so"
			#endif
			);
			bool val = filter.check(entry);
			THEN("It is accepted") {
				CHECK(val);
			}
		}
	}
}

}
}
}
