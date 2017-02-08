#include <module_loader/counting_directory_scanning_shared_library_factory_observer.hpp>

namespace module_loader {

counting_directory_scanning_shared_library_factory_observer::counting_directory_scanning_shared_library_factory_observer ()
	:	begin_directory_(0),
		end_directory_(0),
		load_(0)
{	}

void counting_directory_scanning_shared_library_factory_observer::on_begin_directory (begin_directory_event) {
	++begin_directory_;
}

void counting_directory_scanning_shared_library_factory_observer::on_end_directory (end_directory_event) {
	++end_directory_;
}

void counting_directory_scanning_shared_library_factory_observer::on_load (load_event) {
	++load_;
}

std::size_t counting_directory_scanning_shared_library_factory_observer::begin_directory () const noexcept {
	return begin_directory_;
}

std::size_t counting_directory_scanning_shared_library_factory_observer::end_directory () const noexcept {
	return end_directory_;
}

std::size_t counting_directory_scanning_shared_library_factory_observer::load () const noexcept {
	return load_;
}

}
