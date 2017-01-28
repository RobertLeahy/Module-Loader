#include <module_loader/void_object.hpp>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::void_object objects represent no object","[module_loader][void_object]") {
	GIVEN("A module_loader::void_object") {
		void_object o;
		THEN("module_loader::void_object::type returns the std::type_info for void") {
			CHECK(o.type() == typeid(void));
		}
		THEN("module_loader::void_object::name returns \"void\"") {
			CHECK(o.name() == "void");
		}
		THEN("module_loader::void_object::get returns the null pointer") {
			CHECK_FALSE(o.get());
			const auto & oc = o;
			CHECK_FALSE(o.get());
		}
	}
}

}
}
}
