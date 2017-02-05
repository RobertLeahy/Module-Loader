#include <module_loader/unfulfilled_error.hpp>
#include <module_loader/in_place_offer.hpp>
#include <module_loader/request.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

TEST_CASE("module_loader::unfulfilled_error rejects invalid constructor arguments","[module_loader][unfulfilled_error]") {
	CHECK_THROWS_AS(unfulfilled_error(unfulfilled_error::entries_type{}),std::logic_error);
}

TEST_CASE("module_loader::unfulfilled_error::entry rejects invalid constructor arguments","[module_loader][unfulfilled_error]") {
	unfulfilled_error::entry::requests_type rs;
	request r(typeid(int));
	rs.emplace_back(0,r,unfulfilled_error::entry::request_details::fulfilled_by_type{});
	CHECK_THROWS_AS(unfulfilled_error::entry({},std::move(rs)),std::logic_error);
	auto ptr = std::make_shared<in_place_offer<int,int>>();
	CHECK_THROWS_AS(unfulfilled_error::entry(std::move(ptr),{}),std::logic_error);
}

SCENARIO("module_loader::unfulfilled_error generates an error message appropriate for the given module_loader::unfulfilled_error::entry objects","[module_loader][unfulfilled_error]") {
	GIVEN("A module_loader::unfulfilled_error with one module_loader::unfulfilled_error::entry") {
		unfulfilled_error::entries_type entries;
		auto ptr = std::make_shared<in_place_offer<int,int>>();
		auto && rs = ptr->requests();
		CHECK(rs.size() == 1U);
		unfulfilled_error::entry::requests_type requests;
		requests.emplace_back(0,rs.front(),unfulfilled_error::entry::request_details::fulfilled_by_type{});
		entries.emplace_back(std::move(ptr),std::move(requests));
		unfulfilled_error error(std::move(entries));
		THEN("Its error message is correct") {
			auto str = error.what();
			std::string expected(
				"Unable to fulfill all requests of the following offers:\n"
				"#1: int:\n"
				"\t#0: int [1, 1] fulfilled by []"
			);
			CHECK(str == expected);
		}
	}
}

}
}
}
