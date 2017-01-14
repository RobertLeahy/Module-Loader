#include <module_loader/type_name.hpp>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cxxabi.h>

namespace module_loader {

demangle_error::demangle_error () : std::logic_error("Unknown error demangling") {	}

static std::string invalid_name_error_what (const char * name) {
	std::ostringstream ss;
	ss << "\"" << name << "\" is not a valid mangled name";
	return ss.str();
}

invalid_name_error::invalid_name_error (const char * name) : demangle_error(invalid_name_error_what(name)) {	}

std::string type_name (const char * name) {
	//	Implementation currently only for GCC and Clang
	int status;
	std::unique_ptr<char,void (*) (void *)> ptr(
		::abi::__cxa_demangle(name,nullptr,nullptr,&status),
		&std::free
	);
	switch (status) {
		case 0:
			return std::string(ptr.get());
		case -1:
			throw std::bad_alloc{};
		case -2:
			throw invalid_name_error(name);
		case -3:
			throw std::invalid_argument("Argument to ::abi::__cxa_demangle invalid");
		default:
			throw demangle_error{};
	}
}

std::string type_name (const std::string & name) {
	return type_name(name.c_str());
}

std::string type_name (const std::type_info & info) {
	return type_name(info.name());
}

std::string type_name (const std::type_index & index) {
	return type_name(index.name());
}

}
