#include <module_loader/reference_object.hpp>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::reference_object objects may be constructed from a reference to an object","[module_loader][reference_object]") {
	GIVEN("A module_loader::reference_object") {
		int i = 5;
		reference_object<int> obj(i);
		THEN("Its name is correct") {
			CHECK(obj.name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(obj.type() == typeid(int));
		}
		THEN("It returns a pointer to the object it refers to") {
			CHECK(obj.get() == &i);
		}
	}
}

SCENARIO("module_loader::reference_object objects may be constructed with a custom name","[module_loader][reference_object]") {
	GIVEN("A module_loader::reference_object constructed from a reference to an object and a std::string") {
		int i = 5;
		reference_object<int> obj("foo",i);
		THEN("Its name is correct") {
			CHECK(obj.name() == "foo");
		}
		THEN("Its type is correct") {
			CHECK(obj.type() == typeid(int));
		}
		THEN("It returns a pointer to the object it refers to") {
			CHECK(obj.get() == &i);
		}
	}
}

}
}
}
