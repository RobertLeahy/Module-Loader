#include <boost/filesystem/path.hpp>
#include <module_loader/whereami.hpp>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>
#include <tuple>
#include <utility>

namespace module_loader {

namespace {
#include "whereami.h"
#include "whereami.c"
}

wai_error::wai_error () : std::runtime_error("whereami reported an error during a call to wai_getExecutablePath") {	}

static auto wai_wrapper_impl () {
	int len = wai_getExecutablePath(nullptr,0,nullptr);
	if (len == -1) throw wai_error{};
	auto fn = &std::free;
	std::unique_ptr<char,decltype(fn)> ptr(
		static_cast<char *>(std::malloc(len)),
		fn
	);
	if (!ptr) throw std::bad_alloc{};
	int dir_len;
	if (wai_getExecutablePath(ptr.get(),len,&dir_len) != len) throw wai_error{};
	return std::make_tuple(std::move(ptr),len,dir_len);
}

template <std::size_t N>
static boost::filesystem::path wai_wrapper () {
	auto tuple = wai_wrapper_impl();
	auto ptr = std::get<0>(tuple).get();
	auto len = std::get<N>(tuple);
	return boost::filesystem::path(ptr,ptr + len);
}

boost::filesystem::path current_executable_path () {
	return wai_wrapper<1>();
}

boost::filesystem::path current_executable_directory_path () {
	return wai_wrapper<2>();
}

}
