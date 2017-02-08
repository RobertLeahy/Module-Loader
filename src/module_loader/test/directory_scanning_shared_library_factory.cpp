#include <module_loader/directory_scanning_shared_library_factory.hpp>
#include <boost/filesystem.hpp>
#include <module_loader/counting_directory_scanning_shared_library_factory_observer.hpp>
#include <module_loader/directory_entry_filter.hpp>
#include <module_loader/whereami.hpp>
#include <cstddef>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::directory_scanning_shared_library_factory objects traverse all shared libraries in their managed directories","[module_loader][directory_scanning_shared_library_factory]") {
	counting_directory_scanning_shared_library_factory_observer o;
	directory_scanning_shared_library_factory scanner(o);
	GIVEN("A module_loader::directory_scanning_shared_library_factory which does not traverse any directories") {
		WHEN("module_loader::directory_scanning_shared_library_factory::next is invoked") {
			auto so = scanner.next();
			THEN("No boost::dll::shared_library is returned") {
				CHECK_FALSE(so);
			}
			THEN("No events are dispatched") {
				CHECK(o.begin_directory() == 0U);
				CHECK(o.end_directory() == 0U);
				CHECK(o.load() == 0U);
			}
		}
	}
	GIVEN("A module_loader::directory_scanning_shared_library_factory which traverses a directory which contains shared libraries") {
		auto path = current_executable_directory_path();
		auto result = scanner.add(path);
		CHECK(result);
		WHEN("module_loader::directory_scanning_shared_library_factory::next is invoked repeatedly") {
			std::size_t i(0);
			for (; scanner.next(); ++i);
			THEN("The correct number of boost::dll::shared_library objects are yielded") {
				CHECK(i == 6U);
			}
			THEN("The correct events are dispatched") {
				CHECK(o.begin_directory() == 1U);
				CHECK(o.end_directory() == 1U);
				CHECK(o.load() == 6U);
			}
		}
		WHEN("The same directory is added again") {
			path /= "./";
			result = scanner.add(path);
			CHECK_FALSE(result);
			WHEN("module_loader::directory_scanning_shared_library_factory::next is invoked repeatedly") {
				std::size_t i(0);
				for (; scanner.next(); ++i);
				THEN("The correct number of boost::dll::shared_library objects are yielded") {
					CHECK(i == 6U);
				}
				THEN("The correct events are dispatched") {
					CHECK(o.begin_directory() == 1U);
					CHECK(o.end_directory() == 1U);
					CHECK(o.load() == 6U);
				}
			}
		}
	}
}

SCENARIO("module_loader::directory_scanning_shared_library_factory objects accept a module_loader::directory_entry_filter which determines which files to include and which to exclude","[module_loader][directory_scanning_shared_library_factory]") {
	GIVEN("A module_loader::directory_scanning_shared_library_factory with a module_loader::directory_entry_filter which rejects all shared libraries and which scans a directory which contains shared libraries") {
		class : public directory_entry_filter {
			virtual bool check (const boost::filesystem::directory_entry &) override {
				return false;
			}
		} filter;
		//	The directory scanned is the same that we scan
		//	above and verify that it has 6
		directory_scanning_shared_library_factory scanner(filter);
		WHEN("module_loader::directory_scanning_shared_library_factory::next is invoked") {
			auto so = scanner.next();
			THEN("No boost::dll::shared_library is returned") {
				CHECK_FALSE(so);
			}
		}
	}
}

}
}
}
