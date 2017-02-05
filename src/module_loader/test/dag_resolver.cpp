#include <module_loader/dag_resolver.hpp>
#include <module_loader/counting_resolver_observer.hpp>
#include <module_loader/function_offer.hpp>
#include <module_loader/in_place_offer.hpp>
#include <module_loader/not_a_dag_error.hpp>
#include <module_loader/optional.hpp>
#include <module_loader/unfulfilled_error.hpp>
#include <module_loader/queue_offer_factory.hpp>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <catch.hpp>

namespace module_loader {
namespace tests {
namespace {

SCENARIO("module_loader::dag_resolver objects reject dependency graphs which cannot be resolved","[module_loader][dag_resolver]") {
	GIVEN("A module_loader::dag_resolver whose associated module_loader::offer_factory yields module_loader::offer objects which form a dependency graph which cannot be resolved due to missing dependencies") {
		queue_offer_factory of;
		of.add(std::make_unique<in_place_offer<double,int>>());
		dag_resolver resolver(of);
		THEN("Calling module_loader::dag_resolver::resolve throws a module_loader::unfulfilled_error") {
			CHECK_THROWS_AS(resolver.resolve(),unfulfilled_error);
		}
	}
	GIVEN("A module_loader::dag_resolver whose associated module_loader::offer_factory yields module_loader::offer objects which form a dependency graph which cannot be resolved due to duplicate dependencies") {
		queue_offer_factory of;
		of.add(std::make_unique<in_place_offer<int,int>>());
		of.add(std::make_unique<in_place_offer<int,int>>());
		of.add(std::make_unique<in_place_offer<double,int>>());
		dag_resolver resolver(of);
		THEN("Calling module_loader::dag_resolver::resolve throws a module_loader::unfulfilled_error") {
			CHECK_THROWS_AS(resolver.resolve(),unfulfilled_error);
		}
	}
	GIVEN("A module_loader::dag_resolver whose associated module_loader::offer_factory yields module_loader::offer objects which form a dependency graph which cannot be resolved due to a cycle") {
		queue_offer_factory of;
		of.add(std::make_unique<in_place_offer<int,double>>());
		of.add(std::make_unique<in_place_offer<double,float>>());
		of.add(std::make_unique<in_place_offer<float,int>>());
		dag_resolver resolver(of);
		THEN("Calling module_loader::dag_resolver::resolve throws a module_loader::not_a_dag_error") {
			CHECK_THROWS_AS(resolver.resolve(),not_a_dag_error);
		}
	}
}

template <typename... Ts, typename F>
std::unique_ptr<function_offer<std::decay_t<F>,Ts...>> make_function_offer (F && func) {
	return std::make_unique<function_offer<std::decay_t<F>,Ts...>>(std::forward<F>(func));
}

SCENARIO("module_loader::dag_resolver objects resolve dependency graphs which are directed acyclic graphs","[module_loader][dag_resolver]") {
	GIVEN("A module_loader::dag_resolver whose associated module_loader::offer_factory yields an acyclic dependency graph") {
		queue_offer_factory of;
		std::size_t n(0);
		optional<std::size_t> root;
		of.add(make_function_offer([&] () noexcept -> int {
			root = n++;
			return 5;
		}));
		optional<std::size_t> node;
		optional<int> i;
		of.add(make_function_offer<int>([&] (int j) noexcept {
			node = n++;
			i = j;
		}));
		counting_resolver_observer ro;
		optional<dag_resolver> resolver(in_place,of,ro);
		WHEN("module_loader::dag_resolver::resolve is invoked") {
			resolver->resolve();
			THEN("The dependencies are resolved") {
				REQUIRE(root);
				CHECK(*root == 0U);
				REQUIRE(node);
				CHECK(*node == 1U);
				REQUIRE(i);
				CHECK(*i == 5);
			}
			THEN("The appropriate events are dispatched to the associated module_loader::resolver_observer") {
				CHECK(ro.create() == 2U);
				CHECK(ro.resolve() == 1U);
				CHECK(ro.destroy() == 0U);
			}
			AND_WHEN("The module_loader::dag_resolver is destroyed") {
				resolver = nullopt;
				THEN("The appropriate events are dispatched to the associated module_loader::resolver_observer") {
					CHECK(ro.create() == 2U);
					CHECK(ro.resolve() == 1U);
					CHECK(ro.destroy() == 2U);
				}
			}
		}
	}
}

}
}
}
