#include <module_loader/in_place_object.hpp>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::in_place_object objects contain an object","[module_loader][in_place_object]") {
	GIVEN("A module_loader::in_place_object") {
		in_place_object<int> i(default_name,5);
		THEN("It contains the correct object") {
			CHECK(*static_cast<int *>(i.get()) == 5);
		}
		THEN("It returns the correct name") {
			CHECK(i.name() == "int");
		}
		THEN("It provides the correct types") {
			auto && provides = i.provides();
			CHECK(provides.size() == 1U);
			CHECK(provides.count(typeid(int)) == 1U);
		}
	}
}

SCENARIO("module_loader::in_place_object objects may be created with a custom name","[module_loader][in_place_object]") {
	GIVEN("A module_loader::in_place_object created with a custom name") {
		in_place_object<std::string> i("foo");
		THEN("It contains the correct object") {
			CHECK(static_cast<std::string *>(i.get())->empty());
		}
		THEN("It returns the correct name") {
			CHECK(i.name() == "foo");
		}
		THEN("It provides the correct types") {
			auto && provides = i.provides();
			CHECK(provides.count(typeid(std::string)) == 1U);
		}
	}
}

}
}
}
