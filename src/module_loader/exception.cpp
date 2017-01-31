#include <module_loader/exception.hpp>
#include <stdexcept>
#include <cxxabi.h>

namespace module_loader {

const std::type_info & current_exception_type () {
	auto ptr = abi::__cxa_current_exception_type();
	if (!ptr) throw std::logic_error("No exception is currently being handled");
	return *ptr;
}

}
