#include <module_loader/counting_resolver_observer.hpp>

namespace module_loader {

counting_resolver_observer::counting_resolver_observer () : resolve_(0), create_(0), destroy_(0) {	}

void counting_resolver_observer::on_resolve (resolve_event) {
	++resolve_;
}

void counting_resolver_observer::on_create (create_event) {
	++create_;
}

void counting_resolver_observer::on_destroy (destroy_event) {
	++destroy_;
}

std::size_t counting_resolver_observer::resolve () const noexcept {
	return resolve_;
}

std::size_t counting_resolver_observer::create () const noexcept {
	return create_;
}

std::size_t counting_resolver_observer::destroy () const noexcept {
	return destroy_;
}

}
