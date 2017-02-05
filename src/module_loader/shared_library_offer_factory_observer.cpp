#include <module_loader/shared_library_offer_factory_observer.hpp>
#include <utility>

namespace module_loader {

shared_library_offer_factory_observer::~shared_library_offer_factory_observer () noexcept {	}

shared_library_offer_factory_observer::begin_load_event::begin_load_event (boost::dll::shared_library so)
	:	so_(std::move(so))
{	}

const boost::dll::shared_library & shared_library_offer_factory_observer::begin_load_event::shared_library () const noexcept {
	return so_;
}

shared_library_offer_factory_observer::add_event::add_event (boost::dll::shared_library so, const module_loader::offer & o)
	:	begin_load_event(std::move(so)),
		o_(&o)
{	}

const offer & shared_library_offer_factory_observer::add_event::offer () const noexcept {
	return *o_;
}

}
