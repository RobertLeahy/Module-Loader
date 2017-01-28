#include <module_loader/void_object.hpp>

namespace module_loader {

void * void_object::get () noexcept {
	return nullptr;
}

const void * void_object::get () const noexcept {
	return nullptr;
}

}
