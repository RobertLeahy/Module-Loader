#include <module_loader/in_place_offer.hpp>
#include <typeinfo>
#include <utility>
#include <vector>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("A module_loader::in_place_offer may make no requests","[module_loader][in_place_offer]") {
	GIVEN("A module_loader::in_place_offer templated on only one argument") {
		using type = std::vector<int>;
		using offer_type = in_place_offer<type>;
		offer_type offer;
		THEN("It makes no requests") {
			CHECK(offer.requests().empty());
		}
		THEN("Its name is correct") {
			CHECK(offer.name() == "std::vector<int, std::allocator<int> >");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(type));
		}
		THEN("It provides the correct types") {
			auto && set = offer.provides();
			CHECK(set.count(typeid(type)) == 1U);
			//	There may be other types due to implementation
			//	details but we don't care
		}
		WHEN("It is fulfilled") {
			auto ptr = offer.fulfill(offer_type::fulfill_type{});
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("It is of the correct type") {
					REQUIRE(ptr->type() == typeid(type));
					AND_THEN("It is constructed as expected") {
						CHECK(static_cast<type *>(ptr->get())->empty());
					}
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.count(typeid(type)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "std::vector<int, std::allocator<int> >");
				}
			}
		}
	}
}

SCENARIO("A module_loader::in_place_offer may make requests","[module_loader][in_place_offer]") {
	GIVEN("A module_loader::in_place_offer templated on more than one argument") {
		using type = int;
		using offer_type = in_place_offer<int,int>;
		offer_type offer;
		THEN("It makes one or more requests") {
			auto && rs = offer.requests();
			REQUIRE(rs.size() == 1U);
			AND_THEN("That request or those requests is/are correct") {
				auto && r = rs.front();
				CHECK(r.type() == typeid(int));
				CHECK(r.lower_bound() == 1U);
				CHECK(r.upper_bound() == 1U);
			}
		}
		THEN("Its name is correct") {
			CHECK(offer.name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(type));
		}
		THEN("It provides the correct types") {
			auto && set = offer.provides();
			CHECK(set.size() == 1U);
			CHECK(set.count(typeid(type)) == 1U);
		}
		WHEN("It is fulfilled") {
			typename offer_type::fulfill_type fulfill;
			int i = 5;
			void * p = &i;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill(fulfill);
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("It is of the correct type") {
					REQUIRE(ptr->type() == typeid(type));
					AND_THEN("It is constructed as expected") {
						CHECK(*static_cast<type *>(ptr->get()) == i);
					}
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.size() == 1U);
					CHECK(set.count(typeid(type)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
			}
		}
	}
}

SCENARIO("A module_loader::in_place_offer may have a custom name","[module_loader][in_place_offer]") {
	GIVEN("A module_loader::in_place_offer constructed with a custom name") {
		using type = std::vector<int>;
		using offer_type = in_place_offer<type>;
		offer_type offer("foo");
		THEN("Its name is correct") {
			CHECK(offer.name() == "foo");
		}
		WHEN("It is fulfilled") {
			auto ptr = offer.fulfill(offer_type::fulfill_type{});
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "foo");
				}
			}
		}
	}
}

SCENARIO("A module_loader::in_place_offer may fulfill requests with a std::shared_ptr","[module_loader][in_place_offer]") {
	GIVEN("A module_loader::in_place_offer") {
		using type = std::vector<int>;
		using offer_type = in_place_offer<type>;
		offer_type offer;
		WHEN("It is fulfilled and a std::shared_ptr is requested") {
			auto ptr = offer.fulfill_shared(offer_type::fulfill_type{});
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("It is of the correct type") {
					REQUIRE(ptr->type() == typeid(type));
					AND_THEN("It is constructed as expected") {
						CHECK(static_cast<type *>(ptr->get())->empty());
					}
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.count(typeid(type)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "std::vector<int, std::allocator<int> >");
				}
			}
		}
	}
}

}
}
}
