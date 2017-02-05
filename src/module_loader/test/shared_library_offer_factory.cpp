#include <module_loader/shared_library_offer_factory.hpp>
#include <boost/dll/shared_library.hpp>
#include <module_loader/counting_shared_library_offer_factory_observer.hpp>
#include <module_loader/optional.hpp>
#include <module_loader/queue_shared_library_factory.hpp>
#include <module_loader/whereami.hpp>
#include <cstddef>
#include <sstream>
#include <string>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

static boost::dll::shared_library get_shared_library (const char * name) {
	std::ostringstream ss;
	ss << "libshared_library_offer_factory_" << name << "." 
	#ifdef _WIN32
	"dll"
	#else
	"so"
	#endif
	;
	auto path = current_executable_directory_path();
	return boost::dll::shared_library(path / ss.str());
}

SCENARIO("module_loader::shared_library_offer_factory yields module_loader::offer objects even if the associated module_loader::shared_library_factory does not yield boost::dll::shared_library objects when the module_loader::shared_library_offer_factory is constructed but does yield them when module_loader::shared_library_offer_factory::next is invoked","[module_loader][shared_library_offer_factory]") {
	GIVEN("A module_loader::shared_library_offer_factory constructed with a module_loader::shared_library_factory which does not yield boost::dll::shared_library objects") {
		queue_shared_library_factory qslf;
		//	Sanity check
		REQUIRE_FALSE(qslf.next());
		shared_library_offer_factory slof(qslf);
		//	Sanity check
		REQUIRE_FALSE(slof.next());
		WHEN("The module_loader::shared_library_factory is made to yield boost::dll::shared_library objects") {
			qslf.add(get_shared_library("success"));
			AND_WHEN("module_loader::shared_library_offer_factory::next is invoked") {
				auto offer = slof.next();
				THEN("A module_loader::offer is returned") {
					CHECK(offer);
				}
			}
		}
	}
}

SCENARIO("module_loader::shared_library_offer_factory catches exceptions thrown by shared libraries and rethrows them as a different type","[module_loader][shared_library_offer_factory]") {
	queue_shared_library_factory qslf;
	shared_library_offer_factory slof(qslf);
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces a boost::dll::shared_library whose \"load\" function throws an exception") {
		auto so = get_shared_library("throws");
		qslf.add(so);
		WHEN("module_loader::shared_library_offer_factory::next is invoked") {
			std::string what;
			try {
				slof.next();
				FAIL("module_loader::shared_library_offer_factory::next did not throw");
			} catch (const shared_library_offer_factory::error & error) {
				what = error.what();
			} catch (...) {
				FAIL("module_loader::shared_library_offer_factory::next threw exception of type other than module_loader::shared_library_offer_factory::error");
			}
			THEN("A module_loader::shared_library_offer_factory::error exception is thrown with a message that describes the exception thrown from the shared library") {
				std::ostringstream ss;
				ss << "int thrown from " << so.location().string();
				CHECK(what == ss.str());
			}
		}
	}
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces a boost::dll::shared_library which submits a module_loader::offer whose fulfill and fulfill_shared methods throw an exception") {
		auto so = get_shared_library("fulfill_throws");
		qslf.add(so);
		WHEN("module_loader::shared_library_offer_factory::next is invoked") {
			auto next = slof.next();
			THEN("A module_loader::offer is returned") {
				REQUIRE(next);
				AND_WHEN("module_loader::offer::fulfill is invoked") {
					std::string what;
					try {
						next->fulfill({});
						FAIL("module_loader::offer::fulfill did not throw");
					} catch (const shared_library_offer_factory::error & error) {
						what = error.what();
					} catch (...) {
						FAIL("module_loader::offer::fulfill threw exception of type other than module_loader::shared_library_offer_factory::error");
					}
					THEN("A module_loader::shared_library_offer_factory::error exception is throws with a message that describes the exception thrown from the shared library code") {
						std::ostringstream ss;
						ss << "std::runtime_error thrown from " << so.location().string() << ": Test";
						CHECK(what == ss.str());
					}
				}
				AND_WHEN("module_loader::offer::fulfill_shared is invoked") {
					std::string what;
					try {
						next->fulfill_shared({});
						FAIL("module_loader::offer::fulfill_shared did not throw");
					} catch (const shared_library_offer_factory::error & error) {
						what = error.what();
					} catch (...) {
						FAIL("module_loader::offer::fulfill_shared threw exception of type other than module_loader::shared_library_offer_factory::error");
					}
					THEN("A module_loader::shared_library_offer_factory::error exception is throws with a message that describes the exception thrown from the shared library code") {
						std::ostringstream ss;
						ss << "std::runtime_error thrown from " << so.location().string() << ": Test";
						CHECK(what == ss.str());
					}
				}
			}
		}
	}
}

SCENARIO("module_loader::shared_library_offer_factory enumerates module_loader::offer objects provided by boost::dll::shared_library objects provided to it","[module_loader][shared_library_offer_factory]") {
	queue_shared_library_factory qslf;
	counting_shared_library_offer_factory_observer o;
	shared_library_offer_factory slof(qslf,o);
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces no boost::dll::shared_library objects") {
		WHEN("module_loader::shared_library_offer_factory::next is invoked") {
			auto offer = slof.next();
			THEN("No module_loader::offer is returned") {
				CHECK_FALSE(offer);
			}
			THEN("No events are dispatched") {
				CHECK(o.begin_load() == 0U);
				CHECK(o.end_load() == 0U);
				CHECK(o.add() == 0U);
			}
		}
	}
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces a boost::dll::shared_library object which produces no module_loader::offer objects") {
		qslf.add(get_shared_library("none"));
		WHEN("module_loader::shared_library_offer_factory::next is invoked") {
			auto offer = slof.next();
			THEN("No module_loader::offer is returned") {
				CHECK_FALSE(offer);
			}
			THEN("The appropriate events are dispatched") {
				CHECK(o.begin_load() == 1U);
				CHECK(o.end_load() == 1U);
				CHECK(o.add() == 0U);
			}
		}
	}
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces a boost::dll::shared_library object which produces one module_loader::offer object") {
		qslf.add(get_shared_library("success"));
		WHEN("module_loader::shared_library_offer_factory::next is invoked") {
			auto offer = slof.next();
			THEN("A module_loader::offer is returned") {
				CHECK(offer);
			}
			THEN("The appropriate events are dispatched") {
				CHECK(o.begin_load() == 1U);
				CHECK(o.end_load() == 1U);
				CHECK(o.add() == 1U);
			}
			AND_WHEN("module_loader::shared_library_offer_factory::next is invoked") {
				offer = slof.next();
				THEN("No module_loader::offer is returned") {
					CHECK_FALSE(offer);
				}
				THEN("No further events are dispatched") {
					CHECK(o.begin_load() == 1U);
					CHECK(o.end_load() == 1U);
					CHECK(o.add() == 1U);
				}
			}
		}
	}
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces a boost::dll::shared_library object which produces two module_loader::offer objects") {
		qslf.add(get_shared_library("multiple"));
		WHEN("module_loader::shared_library_offer_factory::next is invoked") {
			auto offer = slof.next();
			THEN("A module_loader::offer is returned") {
				CHECK(offer);
			}
			THEN("The appropriate events are dispatched") {
				CHECK(o.begin_load() == 1U);
				CHECK(o.end_load() == 1U);
				CHECK(o.add() == 2U);
			}
			AND_WHEN("module_loader::shared_library_offer_factory::next is invoked") {
				offer = slof.next();
				THEN("A module_loader::offer is returned") {
					CHECK(offer);
				}
				THEN("No further events are dispatched") {
					CHECK(o.begin_load() == 1U);
					CHECK(o.end_load() == 1U);
					CHECK(o.add() == 2U);
				}
				AND_WHEN("module_loader::shared_library_offer_factory::next is invoked") {
					offer = slof.next();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(offer);
					}
					THEN("No further events are dispatched") {
						CHECK(o.begin_load() == 1U);
						CHECK(o.end_load() == 1U);
						CHECK(o.add() == 2U);
					}
				}
			}
		}
	}
	GIVEN("A module_loader::shared_library_offer_factory whose associated module_loader::shared_library_factory produces multile boost::dll::shared_library objects") {
		//	Should yield a total of three offers
		qslf.add(get_shared_library("none"));
		qslf.add(get_shared_library("multiple"));
		qslf.add(get_shared_library("success"));
		WHEN("All module_loader::offer objects are yielded") {
			std::size_t n = 0;
			for (auto offer = slof.next(); offer; offer = slof.next(), ++n);
			THEN("The correct number of module_loader::offer objects are yielded") {
				CHECK(n == 3U);
			}
			THEN("The appropriate events are dispatched") {
				CHECK(o.begin_load() == 3U);
				CHECK(o.end_load() == 3U);
				CHECK(o.add() == 3U);
			}
		}
	}
}

}
}
}
