#include <module_loader/resolver_observer.hpp>

namespace module_loader {

resolver_observer::~resolver_observer () noexcept {	}

resolver_observer::resolve_event::resolve_event (const offer & requester, const offer & provider, const module_loader::request & request) noexcept
	:	requester_(&requester),
		provider_(&provider),
		request_(&request)
{	}

const offer & resolver_observer::resolve_event::requester () const noexcept {
	return *requester_;
}

const offer & resolver_observer::resolve_event::provider () const noexcept {
	return *provider_;
}

const request & resolver_observer::resolve_event::request () const noexcept {
	return *request_;
}

resolver_observer::create_event::create_event (const module_loader::offer & offer, const module_loader::object & object) noexcept
	:	offer_(&offer),
		object_(&object)
{	}

const offer & resolver_observer::create_event::offer () const noexcept {
	return *offer_;
}

const object & resolver_observer::create_event::object () const noexcept {
	return *object_;
}

resolver_observer::destroy_event::destroy_event (const module_loader::object & object) noexcept
	:	object_(&object)
{	}

const object & resolver_observer::destroy_event::object () const noexcept {
	return *object_;
}

}
