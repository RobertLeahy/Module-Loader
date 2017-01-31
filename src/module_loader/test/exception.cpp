#include <module_loader/exception.hpp>
#include <new>
#include <stdexcept>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

TEST_CASE("module_loader::current_exception_type throws an exception when called while no exceptions are being handled","[module_loader][exception]") {
	CHECK_THROWS_AS(current_exception_type(),std::logic_error);
}

TEST_CASE("module_loader::current_exception_type returns the correct std::type_info when called during exception handling","[module_loader][exception]") {
	bool caught = false;
	try {
		throw std::bad_alloc{};
	} catch (...) {
		caught = true;
		CHECK(current_exception_type() == typeid(std::bad_alloc));
	}
	CHECK(caught);
}

}
}
}
