#include <module_loader/counting_shared_library_offer_factory_observer.hpp>

namespace module_loader {

counting_shared_library_offer_factory_observer::counting_shared_library_offer_factory_observer ()
	:	begin_load_(0),
		end_load_(0),
		add_(0)
{	}

void counting_shared_library_offer_factory_observer::on_begin_load (begin_load_event) {
	++begin_load_;
}

void counting_shared_library_offer_factory_observer::on_end_load (end_load_event) {
	++end_load_;
}

void counting_shared_library_offer_factory_observer::on_add (add_event) {
	++add_;
}

std::size_t counting_shared_library_offer_factory_observer::begin_load () const noexcept {
	return begin_load_;
}

std::size_t counting_shared_library_offer_factory_observer::end_load () const noexcept {
	return end_load_;
}

std::size_t counting_shared_library_offer_factory_observer::add () const noexcept {
	return add_;
}

}
