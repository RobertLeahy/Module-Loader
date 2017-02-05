#include <module_loader/queue_offer_factory.hpp>
#include <module_loader/in_place_offer.hpp>
#include <memory>
#include <utility>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::queue_offer_factory objects return a null pointer when their queue is empty","[module_loader][queue_offer_factory]") {
	GIVEN("A module_loader::queue_offer_factory") {
		queue_offer_factory qrf;
		THEN("module_loader::queue_offer_factory::next returns a null std::unique_ptr") {
			CHECK(!qrf.next());
		}
		THEN("module_loader::queue_offer_factory::next_shared returns a null std::shared_ptr") {
			CHECK(!qrf.next_shared());
		}
	}
}

SCENARIO("module_loader::queue_offer_factory objects return module_loader::offer objects in the order in which they were enqueued","[module_loader][queue_offer_factory]") {
	GIVEN("A module_loader::queue_offer_factory") {
		queue_offer_factory qrf;
		WHEN("Two module_loader::offer objects are enqueued") {
			std::unique_ptr<offer> ptr(std::make_unique<in_place_offer<double,double>>());
			offer * first = ptr.get();
			qrf.add(std::move(ptr));
			ptr = std::make_unique<in_place_offer<int,int>>();
			offer * second = ptr.get();
			qrf.add(std::move(ptr));
			AND_WHEN("module_loader::queue_offer_factory::next is invoked") {
				ptr = qrf.next();
				THEN("The first module_loader::offer enqueued is returned") {
					CHECK(ptr.get() == first);
				}
				AND_WHEN("module_loader::queue_offer_factory::next is invoked") {
					ptr = qrf.next();
					THEN("The second module_loader::offer enqueued is returned") {
						CHECK(ptr.get() == second);
					}
					AND_WHEN("module_loader::queue_offer_factory::next is invoked") {
						ptr = qrf.next();
						THEN("Null is returned") {
							CHECK(!ptr);
						}
					}
				}
			}
			AND_WHEN("module_loader::queue_offer_factory::next_shared is invoked") {
				auto s = qrf.next_shared();
				THEN("The first module_loader::offer enqueued is returned") {
					CHECK(s.get() == first);
				}
				AND_WHEN("module_loader::queue_offer_factory::next_shared is invoked") {
					s = qrf.next_shared();
					THEN("The second module_loader::offer enqueued is returned") {
						CHECK(s.get() == second);
					}
					AND_WHEN("module_loader::queue_offer_factory::next_shared is invoked") {
						s = qrf.next_shared();
						THEN("Null is returned") {
							CHECK(!s);
						}
					}
				}
			}
		}
	}
}

}
}
}
