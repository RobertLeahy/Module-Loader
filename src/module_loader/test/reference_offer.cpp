#include <module_loader/reference_offer.hpp>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("A module_loader::reference_offer wraps a reference and provides a module_loader::object which wraps that same object when fulfilled","[module_loader][reference_offer]") {
	GIVEN("A module_loader::reference_offer object") {
		int i = 5;
		reference_offer<int> offer(i);
		THEN("It makes no requests") {
			CHECK(offer.requests().empty());
		}
		THEN("Its name is correct") {
			CHECK(offer.name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(int));
		}
		THEN("It provides the correct type(s)") {
			auto && ps = offer.provides();
			CHECK(ps.size() == 1U);
			CHECK(ps.count(typeid(int)) == 1U);
		}
		WHEN("module_loader::reference_offer::fulfill is called") {
			auto ptr = offer.fulfill({});
			THEN("A module_loader::object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
				AND_THEN("It provides the correct type(s)") {
					auto && ps = ptr->provides();
					CHECK(ps.size() == 1U);
					CHECK(ps.count(typeid(int)) == 1U);
				}
				AND_THEN("module_loader::object::get returns a pointer to the referred to object") {
					CHECK(ptr->get() == &i);
				}
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
			}
		}
		WHEN("module_loader::reference_offer::fulfill_shared is called") {
			auto ptr = offer.fulfill_shared({});
			THEN("A module_loader::object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "int");
				}
				AND_THEN("It provides the correct type(s)") {
					auto && ps = ptr->provides();
					CHECK(ps.size() == 1U);
					CHECK(ps.count(typeid(int)) == 1U);
				}
				AND_THEN("module_loader::object::get returns a pointer to the referred to object") {
					CHECK(ptr->get() == &i);
				}
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
			}
		}
	}
	GIVEN("A module_loader::reference_offer object with a custom name") {
		int i = 5;
		reference_offer<int> offer(i,"foo");
		THEN("It makes no requests") {
			CHECK(offer.requests().empty());
		}
		THEN("Its name is correct") {
			CHECK(offer.name() == "foo");
		}
		THEN("Its type is correct") {
			CHECK(offer.type() == typeid(int));
		}
		THEN("It provides the correct type(s)") {
			auto && ps = offer.provides();
			CHECK(ps.size() == 1U);
			CHECK(ps.count(typeid(int)) == 1U);
		}
		WHEN("module_loader::reference_offer::fulfill is called") {
			auto ptr = offer.fulfill({});
			THEN("A module_loader::object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "foo");
				}
				AND_THEN("It provides the correct type(s)") {
					auto && ps = ptr->provides();
					CHECK(ps.size() == 1U);
					CHECK(ps.count(typeid(int)) == 1U);
				}
				AND_THEN("module_loader::object::get returns a pointer to the referred to object") {
					CHECK(ptr->get() == &i);
				}
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
			}
		}
		WHEN("module_loader::reference_offer::fulfill_shared is called") {
			auto ptr = offer.fulfill_shared({});
			THEN("A module_loader::object is returned") {
				REQUIRE(ptr);
				AND_THEN("Its name is correct") {
					CHECK(ptr->name() == "foo");
				}
				AND_THEN("It provides the correct type(s)") {
					auto && ps = ptr->provides();
					CHECK(ps.size() == 1U);
					CHECK(ps.count(typeid(int)) == 1U);
				}
				AND_THEN("module_loader::object::get returns a pointer to the referred to object") {
					CHECK(ptr->get() == &i);
				}
				AND_THEN("Its type is correct") {
					CHECK(ptr->type() == typeid(int));
				}
			}
		}
	}
}

SCENARIO("module_loader::make_unique_reference_offer may be used to wrap references","[module_loader][reference_offer]") {
	GIVEN("A std::unique_ptr<module_loader::offer> obtained by calling module_loader::make_unique_reference_offer") {
		int i = 5;
		auto ptr = module_loader::make_unique_reference_offer(i);
		THEN("Its name is correct") {
			CHECK(ptr->name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(ptr->type() == typeid(int));
		}
		THEN("Its requests are correct") {
			CHECK(ptr->requests().empty());
		}
	}
	GIVEN("A std::unique_ptr<module_loader::offer> obtained by calling module_loader::make_unique_reference_offer with a custom name") {
		int i = 5;
		auto ptr = module_loader::make_unique_reference_offer(i,"bar");
		THEN("Its name is correct") {
			CHECK(ptr->name() == "bar");
		}
		THEN("Its type is correct") {
			CHECK(ptr->type() == typeid(int));
		}
		THEN("Its requests are correct") {
			CHECK(ptr->requests().empty());
		}
	}
}

SCENARIO("module_loader::make_shared_reference_offer may be used to wrap references","[module_loader][reference_offer]") {
	GIVEN("A std::shared_ptr<module_loader::offer> obtained by calling module_loader::make_shared_reference_offer") {
		int i = 5;
		auto ptr = module_loader::make_shared_reference_offer(i);
		THEN("Its name is correct") {
			CHECK(ptr->name() == "int");
		}
		THEN("Its type is correct") {
			CHECK(ptr->type() == typeid(int));
		}
		THEN("Its requests are correct") {
			CHECK(ptr->requests().empty());
		}
	}
	GIVEN("A std::shared_ptr<module_loader::offer> obtained by calling module_loader::make_shared_reference_offer with a custom name") {
		int i = 5;
		auto ptr = module_loader::make_shared_reference_offer(i,"bar");
		THEN("Its name is correct") {
			CHECK(ptr->name() == "bar");
		}
		THEN("Its type is correct") {
			CHECK(ptr->type() == typeid(int));
		}
		THEN("Its requests are correct") {
			CHECK(ptr->requests().empty());
		}
	}
}

}
}
}
