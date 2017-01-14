#include <module_loader/type_traits.hpp>

namespace module_loader {
namespace test {
namespace {

static_assert(
	is_public_unambiguous_base_of_v<int,int>,
	"is_public_unambiguous_base_of_v: Comparison with self"
);
static_assert(
	!is_public_unambiguous_base_of_v<int,float>,
	"is_public_unambiguous_base_of_v: Comparison with unrelated type"
);

class A {	};
class B : public A {	};
class C : public A {	};
class D : public B, public C {	};

static_assert(
	is_public_unambiguous_base_of_v<A,B>,
	"is_public_unambiguous_base_of_v: Unambiguous"
);
static_assert(
	!is_public_unambiguous_base_of_v<A,D>,
	"is_public_unambiguous_base_of_v: Ambiguous"
);

class E : private A {	};

static_assert(
	!is_public_unambiguous_base_of_v<A,E>,
	"is_public_unambiguous_base_of_v: Inaccessible"
);

class F : public virtual A {	};
class G : public virtual A {	};
class H : public F, public G {	};

static_assert(
	is_public_unambiguous_base_of_v<A,H>,
	"is_public_unambiguous_base_of_v: Virtual, multiple, & unambiguous"
);

}
}
}
