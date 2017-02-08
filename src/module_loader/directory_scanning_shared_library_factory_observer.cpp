#include <module_loader/directory_scanning_shared_library_factory_observer.hpp>
#include <utility>

namespace module_loader {

directory_scanning_shared_library_factory_observer::~directory_scanning_shared_library_factory_observer () noexcept {	}

directory_scanning_shared_library_factory_observer::begin_directory_event::begin_directory_event (boost::filesystem::path path)
	:	path_(std::move(path))
{	}

const boost::filesystem::path & directory_scanning_shared_library_factory_observer::begin_directory_event::path () const noexcept {
	return path_;
}

directory_scanning_shared_library_factory_observer::load_event::load_event (boost::dll::shared_library so)
	:	so_(std::move(so))
{	}

const boost::dll::shared_library & directory_scanning_shared_library_factory_observer::load_event::shared_library () const noexcept {
	return so_;
}

}
