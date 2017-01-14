#include <module_loader/bases.hpp>
#include <typeinfo>
#include <catch.hpp>

namespace module_loader {
namespace test {
namespace {

SCENARIO("module_loader::public_unambiguous_bases returns the given type itself","[module_loader][public_unambiguous_bases]") {
	GIVEN("The return value of module_loader::public_unambiguous_bases templated on a type with no base classes") {
		auto set = public_unambiguous_bases<int>();
		THEN("It contains only the type on which module_loader::public_unambiguous_bases was templated on") {
			CHECK(set.size() == 1U);
			CHECK(set.count(typeid(int)) == 1U);
		}
	}
}

class A {	};
class B : public A {	};

SCENARIO("module_loader::public_unambiguous_bases returns publicly accessible, unambiguous base classes of the given type","[module_loader][public_unambiguous_bases]") {
	GIVEN("The return value of module_loader::public_unambiguous_bases templated on a type with at least one publicly accessible, unambiguous base class") {
		auto set = public_unambiguous_bases<B>();
		THEN("It contains the publicly accessible, unambiguous base classes") {
			CHECK(set.size() == 2U);
			CHECK(set.count(typeid(B)) == 1U);
			CHECK(set.count(typeid(A)) == 1U);
		}
	}
}

class C : public A {	};
class D : public B, public C {	};

SCENARIO("module_loader::public_unambiguous_bases does not return ambiguous bases","[module_loader][public_unambiguous_bases]") {
	GIVEN("The return value of module_loader::public_unambiguous_bases templated on a type with an ambiguous base") {
		auto set = public_unambiguous_bases<D>();
		THEN("It does not contain the ambiguous base") {
			CHECK(set.size() == 3U);
			CHECK(set.count(typeid(D)) == 1U);
			CHECK(set.count(typeid(B)) == 1U);
			CHECK(set.count(typeid(C)) == 1U);
		}
	}
}

class E : private A {	};

SCENARIO("module_loader::public_unambiguous_bases does not return inaccessible bases","[module_loader][public_unambiguous_bases]") {
	GIVEN("The return value of module_loader::public_unambiguous_bases templated on a type with an inaccessible base") {
		auto set = public_unambiguous_bases<E>();
		THEN("It does not contain the inaccessible base") {
			CHECK(set.size() == 1U);
			CHECK(set.count(typeid(E)) == 1U);
		}
	}
}

class F : public virtual A {	};
class G : public virtual A {	};
class H : public F, public G {	};

SCENARIO("module_loader::public_unambiguous_bases returns accessible, multiply inherited virtual bases","[module_loader][public_unambiguous_bases]") {
	GIVEN("The return value of module_loader::public_unambiguous_bases templated on a type with a multiply inherited virtual base") {
		auto set = public_unambiguous_bases<H>();
		THEN("It contains the multiply inherited virtual base") {
			CHECK(set.size() == 4U);
			CHECK(set.count(typeid(A)) == 1U);
			CHECK(set.count(typeid(F)) == 1U);
			CHECK(set.count(typeid(G)) == 1U);
			CHECK(set.count(typeid(H)) == 1U);
		}
	}
}

}
}
}
