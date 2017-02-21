#include <module_loader/function_offer.hpp>
#include <functional>
#include <typeinfo>
#include <utility>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::function_offer objects wrap a functor and provide a module_loader::object from its return value when fulfilled","[module_loader][function_offer]") {
	GIVEN("A module_loader::function_offer which wraps a functor which returns a value") {
		using function_type = std::function<int (int)>;
		int i = 0;
		function_type function = [&] (int n) noexcept {
			i = n;
			return n + 1;
		};
		function_offer<function_type,int> offer(function);
		THEN("Its name is correct") {
			CHECK(offer.name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(int));
		}
		THEN("It provides the correct types") {
			auto && set = offer.provides();
			CHECK(set.size() == 1U);
			CHECK(set.count(typeid(int)) == 1U);
		}
		THEN("It requests the correct types") {
			auto && rs = offer.requests();
			REQUIRE(rs.size() == 1U);
			auto && r = rs.front();
			CHECK(r.type() == typeid(int));
			CHECK(r.lower_bound() == 1U);
			CHECK(r.upper_bound() == 1U);
		}
		WHEN("It is fulfilled and a std::unique_ptr is requested") {
			int j = 5;
			void * p = &j;
			decltype(offer)::fulfill_type fulfill;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill(fulfill);
			THEN("The wrapped functor is invoked") {
				CHECK(i == 5);
			}
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.size() == 1U);
					CHECK(set.count(typeid(int)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
				AND_THEN("Its value is correct") {
					auto p = ptr->get();
					REQUIRE(p);
					CHECK(*static_cast<int *>(p) == 6);
				}
			}
		}
		WHEN("It is fulfilled and a std::shared_ptr is requested") {
			int j = 5;
			void * p = &j;
			decltype(offer)::fulfill_type fulfill;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill_shared(fulfill);
			THEN("The wrapped functor is invoked") {
				CHECK(i == 5);
			}
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.size() == 1U);
					CHECK(set.count(typeid(int)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
				AND_THEN("Its value is correct") {
					auto p = ptr->get();
					REQUIRE(p);
					CHECK(*static_cast<int *>(p) == 6);
				}
			}
		}
	}
}

SCENARIO("module_loader::function_offer objects may wrap a functor which does not return a value","[module_loader][function_offer]") {
	GIVEN("A module_loader::function_offer which wraps a functor which does not return a value") {
		using function_type = std::function<void (int)>;
		int i = 0;
		function_type function = [&] (int n) noexcept {	i = n;	};
		function_offer<function_type,int> offer(function);
		THEN("Its name is correct") {
			CHECK(offer.name() == "void");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(void));
		}
		THEN("It provides the correct types") {
			CHECK(offer.provides().empty());
		}
		THEN("It requests the correct types") {
			auto && rs = offer.requests();
			REQUIRE(rs.size() == 1U);
			auto && r = rs.front();
			CHECK(r.type() == typeid(int));
			CHECK(r.lower_bound() == 1U);
			CHECK(r.upper_bound() == 1U);
		}
		WHEN("It is fulfilled and a std::unique_ptr is requested") {
			int j = 5;
			void * p = &j;
			decltype(offer)::fulfill_type fulfill;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill(fulfill);
			THEN("The wrapped functor is invoked") {
				CHECK(i == 5);
			}
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("module_loader::object::get returns a null pointer") {
					CHECK_FALSE(ptr->get());
					const auto & o = *ptr;
					CHECK_FALSE(o.get());
				}
				AND_THEN("module_loader::object::type returns the std::type_info for void") {
					CHECK(ptr->type() == typeid(void));
				}
				AND_THEN("module_loader::object::provides returns the empty set") {
					CHECK(ptr->provides().empty());
				}
				AND_THEN("module_loader::object::name returns the correct value") {
					CHECK(ptr->name() == "void");
				}
			}
		}
		WHEN("It is fulfilled and a std::shared_ptr is requested") {
			int j = 5;
			void * p = &j;
			decltype(offer)::fulfill_type fulfill;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill_shared(fulfill);
			THEN("The wrapped functor is invoked") {
				CHECK(i == 5);
			}
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("module_loader::object::get returns a null pointer") {
					CHECK_FALSE(ptr->get());
					const auto & o = *ptr;
					CHECK_FALSE(o.get());
				}
				AND_THEN("module_loader::object::type returns the std::type_info for void") {
					CHECK(ptr->type() == typeid(void));
				}
				AND_THEN("module_loader::object::provides returns the empty set") {
					CHECK(ptr->provides().empty());
				}
				AND_THEN("module_loader::object::name returns the correct value") {
					CHECK(ptr->name() == "void");
				}
			}
		}
	}
	GIVEN("A module_loader::function_offer which wraps a function which does not return a value and which has a custom name") {
		using function_type = std::function<void ()>;
		function_type function = [] () noexcept {	};
		function_offer<function_type> offer(function,"foo");
		THEN("Its name is correct") {
			CHECK(offer.name() == "foo");
		}
		WHEN("It is fulfilled and a std::unique_ptr is requested") {
			auto ptr = offer.fulfill({});
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("module_loader::object::name returns the correct value") {
					CHECK(ptr->name() == "foo");
				}
			}
		}
	}
}

SCENARIO("module_loader::function_offer objects may wrap a functor which returns a reference","[module_loader][function_offer]") {
	GIVEN("A module_loader::function_offer which wraps a functor which returns a reference") {
		using function_type = std::function<int & (int &)>;
		int * i = nullptr;
		function_type function = [&] (int & n) noexcept -> int & {
			i = &n;
			return n;
		};
		function_offer<function_type,int> offer(function);
		THEN("Its name is correct") {
			CHECK(offer.name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(int));
		}
		THEN("It provides the correct types") {
			auto && set = offer.provides();
			CHECK(set.size() == 1U);
			CHECK(set.count(typeid(int)) == 1U);
		}
		THEN("It requests the correct types") {
			auto && rs = offer.requests();
			REQUIRE(rs.size() == 1U);
			auto && r = rs.front();
			CHECK(r.type() == typeid(int));
			CHECK(r.lower_bound() == 1U);
			CHECK(r.upper_bound() == 1U);
		}
		WHEN("It is fulfilled and a std::unique_ptr is requested") {
			int j = 5;
			void * p = &j;
			decltype(offer)::fulfill_type fulfill;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill(fulfill);
			THEN("The wrapped functor is invoked") {
				CHECK(i == &j);
			}
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.size() == 1U);
					CHECK(set.count(typeid(int)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
				AND_THEN("Its value is correct") {
					CHECK(ptr->get() == &j);
				}
			}
		}
		WHEN("It is fulfilled and a std::shared_ptr is requested") {
			int j = 5;
			void * p = &j;
			decltype(offer)::fulfill_type fulfill;
			fulfill.push_back(std::make_pair(&p,1U));
			auto ptr = offer.fulfill_shared(fulfill);
			THEN("The wrapped functor is invoked") {
				CHECK(i == &j);
			}
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
				AND_THEN("It provides the correct types") {
					auto && set = ptr->provides();
					CHECK(set.size() == 1U);
					CHECK(set.count(typeid(int)) == 1U);
				}
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
				AND_THEN("Its value is correct") {
					CHECK(ptr->get() == &j);
				}
			}
		}
	}
}

SCENARIO("module_loader::function_offer objects may be provided with a custom name","[module_loader][function_offer]") {
	GIVEN("A module_loader::function_offer which is constructed with a custom name") {
		using function_type = std::function<int ()>;
		function_offer<function_type> offer([] () noexcept {	return 5;	},"foo");
		THEN("Its name is correct") {
			CHECK(offer.name() == "foo");
		}
		WHEN("It is fulfilled and a std::unique_ptr is requested") {
			auto ptr = offer.fulfill(decltype(offer)::fulfill_type{});
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "foo");
				}
			}
		}
		WHEN("It is fulfilled and a std::shared_ptr is requested") {
			auto ptr = offer.fulfill_shared(decltype(offer)::fulfill_type{});
			THEN("An object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "foo");
				}
			}
		}
	}
}

}
}
}
