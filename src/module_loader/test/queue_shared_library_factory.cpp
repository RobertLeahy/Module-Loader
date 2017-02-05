#include <module_loader/queue_shared_library_factory.hpp>
#include <boost/dll/shared_library.hpp>
#include <module_loader/whereami.hpp>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("boost::dll::shared_library objects may be added to and dequed from a module_loader::queue_shared_library_factory","[module_loader][queue_shared_library_factory]") {
	GIVEN("A module_loader::queue_shared_library_factory") {
		queue_shared_library_factory qslf;
		WHEN("module_loader::queue_shared_library_factory::next is called") {
			auto so = qslf.next();
			THEN("The returned boost::dll::shared_library object does not represent a shared library") {
				CHECK_FALSE(so);
			}
		}
		WHEN("A boost::dll::shared_library object is added thereto") {
			boost::dll::shared_library so(current_executable_directory_path() / "libmodule_loader."
				#ifdef _WIN32
				"dll"
				#else
				"so"
				#endif
			);
			qslf.add(so);
			AND_WHEN("module_loader::queue_shared_library_factory::next is called") {
				auto next = qslf.next();
				THEN("The correct boost::dll::shared_library is returned") {
					CHECK(so == next);
				}
				AND_WHEN("module_loader::queue_shared_library_factory::next is called") {
					next = qslf.next();
					THEN("The returned boost::dll::shared_library object does not represent a shared library") {
						CHECK_FALSE(next);
					}
				}
			}
		}
	}
}

}
}
}
