#include <module_loader/type_name.hpp>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::type_name demangles the names returned by std::type_name::name and std::type_index::name","[module_loader][type_name]") {
	GIVEN("A std::type_info object") {
		auto && ti = typeid(int);
		WHEN("module_loader::type_name is called thereupon") {
			auto name = type_name(ti);
			THEN("A human readable name for the type is returned") {
				CHECK(name == "int");
			}
		}
	}
}

}
}
}
