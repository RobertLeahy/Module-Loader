#include <module_loader/request.hpp>

namespace module_loader {

request::request (const std::type_info & type, std::size_t lo, std::size_t hi) noexcept
	:	type_(&type),
		lo_(lo),
		hi_(hi)
{	}

const std::type_info & request::type () const noexcept {
	return *type_;
}

std::size_t request::lower_bound () const noexcept {
	return lo_;
}

std::size_t request::upper_bound () const noexcept {
	return hi_;
}

}
