#include <module_loader/offer_factory_composite.hpp>
#include <module_loader/in_place_offer.hpp>
#include <module_loader/queue_offer_factory.hpp>
#include <memory>
#include <utility>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::offer_factory_composite objects may be empty","[module_loader][offer_factory_composite]") {
	GIVEN("A module_loader::offer_factory_composite object") {
		offer_factory_composite factory;
		WHEN("module_loader::offer_factory_composite::next is invoked") {
			auto ptr = factory.next();
			THEN("No module_loader::offer is returned") {
				CHECK_FALSE(ptr);
			}
		}
		WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
			auto ptr = factory.next_shared();
			THEN("No module_loader::offer is returned") {
				CHECK_FALSE(ptr);
			}
		}
		WHEN("A module_loader::offer_factory which yields no module_loader::offer objects is added thereto") {
			queue_offer_factory q;
			factory.add(q);
			AND_WHEN("module_loader::offer_factory_composite::next is invoked on the module_loader::offer_factory_composite") {
				auto ptr = factory.next();
				THEN("No module_loader::offer is returned") {
					CHECK_FALSE(ptr);
				}
			}
			AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked on the module_loader::offer_factory_composite") {
				auto ptr = factory.next_shared();
				THEN("No module_loader::offer is returned") {
					CHECK_FALSE(ptr);
				}
			}
			AND_WHEN("Another module_loader::offer_factory which yields no module_loader::offer objects is added thereto") {
				queue_offer_factory q;
				factory.add(q);
				AND_WHEN("module_loader::offer_factory_composite::next is invoked on the module_loader::offer_factory_composite") {
					auto ptr = factory.next();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked on the module_loader::offer_factory_composite") {
					auto ptr = factory.next_shared();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
			}
		}
	}
}

SCENARIO("module_loader::offer_factory_composite objects aggregate the sequence of module_loader::offer objects yielded by underlying module_loader::offer_factory objects","[module_loader][offer_factory_composite]") {
	GIVEN("A module_loader::offer_factory_composite object which wraps a single module_loader::offer_factory object which yields two module_loader::offer objects") {
		offer_factory_composite factory;
		queue_offer_factory q;
		factory.add(q);
		auto o1 = std::make_unique<in_place_offer<int,int>>();
		auto ptr1 = o1.get();
		q.add(std::move(o1));
		auto o2 = std::make_unique<in_place_offer<float,float>>();
		auto ptr2 = o2.get();
		q.add(std::move(o2));
		WHEN("module_loader::offer_factory_composite::next is invoked") {
			auto ptr = factory.next();
			THEN("The first module_loader::offer is returned") {
				REQUIRE(ptr);
				CHECK(ptr.get() == ptr1);
			}
			AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
				auto ptr = factory.next();
				THEN("The second module_loader::offer is returned") {
					REQUIRE(ptr);
					CHECK(ptr.get() == ptr2);
				}
				AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
					auto ptr = factory.next();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
					auto ptr = factory.next_shared();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
			}
			AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
				auto ptr = factory.next_shared();
				THEN("The second module_loader::offer is returned") {
					REQUIRE(ptr);
					CHECK(ptr.get() == ptr2);
				}
				AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
					auto ptr = factory.next();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
					auto ptr = factory.next_shared();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
			}
		}
		WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
			auto ptr = factory.next_shared();
			THEN("The first module_loader::offer is returned") {
				REQUIRE(ptr);
				CHECK(ptr.get() == ptr1);
			}
			AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
				auto ptr = factory.next();
				THEN("The second module_loader::offer is returned") {
					REQUIRE(ptr);
					CHECK(ptr.get() == ptr2);
				}
				AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
					auto ptr = factory.next();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
					auto ptr = factory.next_shared();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
			}
			AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
				auto ptr = factory.next_shared();
				THEN("The second module_loader::offer is returned") {
					REQUIRE(ptr);
					CHECK(ptr.get() == ptr2);
				}
				AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
					auto ptr = factory.next();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
					auto ptr = factory.next_shared();
					THEN("No module_loader::offer is returned") {
						CHECK_FALSE(ptr);
					}
				}
			}
		}
	}
	GIVEN("A module_loader::offer_factory_composite object which wraps multiple module_loader::offer_factory objects which yield three module_loader::offer objects in aggregate") {
		offer_factory_composite factory;
		queue_offer_factory q1;
		factory.add(q1);
		auto o1 = std::make_unique<in_place_offer<int,int>>();
		auto ptr1 = o1.get();
		q1.add(std::move(o1));
		auto o2 = std::make_unique<in_place_offer<float,float>>();
		auto ptr2 = o2.get();
		q1.add(std::move(o2));
		queue_offer_factory q2;
		factory.add(q2);
		queue_offer_factory q3;
		factory.add(q3);
		auto o3 = std::make_unique<in_place_offer<double,double>>();
		auto ptr3 = o3.get();
		q3.add(std::move(o3));
		WHEN("module_loader::offer_factory_composite::next is invoked") {
			auto ptr = factory.next();
			THEN("The first module_loader::offer is returned") {
				REQUIRE(ptr);
				CHECK(ptr.get() == ptr1);
			}
			AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
				auto ptr = factory.next();
				THEN("The second module_loader::offer is returned") {
					REQUIRE(ptr);
					CHECK(ptr.get() == ptr2);
				}
				AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
					auto ptr = factory.next();
					THEN("The third module_loader::offer is returned") {
						REQUIRE(ptr);
						CHECK(ptr.get() == ptr3);
					}
					AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
						auto ptr = factory.next();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
					AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
						auto ptr = factory.next_shared();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
					auto ptr = factory.next_shared();
					THEN("The third module_loader::offer is returned") {
						REQUIRE(ptr);
						CHECK(ptr.get() == ptr3);
					}
					AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
						auto ptr = factory.next();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
					AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
						auto ptr = factory.next_shared();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
				}
			}
		}
		WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
			auto ptr = factory.next_shared();
			THEN("The first module_loader::offer is returned") {
				REQUIRE(ptr);
				CHECK(ptr.get() == ptr1);
			}
			AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
				auto ptr = factory.next();
				THEN("The second module_loader::offer is returned") {
					REQUIRE(ptr);
					CHECK(ptr.get() == ptr2);
				}
				AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
					auto ptr = factory.next();
					THEN("The third module_loader::offer is returned") {
						REQUIRE(ptr);
						CHECK(ptr.get() == ptr3);
					}
					AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
						auto ptr = factory.next();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
					AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
						auto ptr = factory.next_shared();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
				}
				AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
					auto ptr = factory.next_shared();
					THEN("The third module_loader::offer is returned") {
						REQUIRE(ptr);
						CHECK(ptr.get() == ptr3);
					}
					AND_WHEN("module_loader::offer_factory_composite::next is invoked") {
						auto ptr = factory.next();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
						}
					}
					AND_WHEN("module_loader::offer_factory_composite::next_shared is invoked") {
						auto ptr = factory.next_shared();
						THEN("No module_loader::offer is returned") {
							CHECK_FALSE(ptr);
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
