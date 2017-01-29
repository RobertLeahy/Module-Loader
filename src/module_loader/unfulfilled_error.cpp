#include <module_loader/unfulfilled_error.hpp>
#include <module_loader/type_name.hpp>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace module_loader {

unfulfilled_error::entry::request_details::request_details (std::size_t i, module_loader::request req, fulfilled_by_type fulfilled_by)
	:	i_(i),
		request_(std::move(req)),
		fulfilled_by_(std::move(fulfilled_by))
{	}

std::size_t unfulfilled_error::entry::request_details::index () const noexcept {
	return i_;
}

const request & unfulfilled_error::entry::request_details::request () const noexcept {
	return request_;
}

const unfulfilled_error::entry::request_details::fulfilled_by_type & unfulfilled_error::entry::request_details::fulfilled_by () const noexcept {
	return fulfilled_by_;
}

void unfulfilled_error::entry::request_details::to_string (std::ostream & os) const {
	os << '#' << i_ << ": " << type_name(request_.type()) << " [" << request_.lower_bound() << ", ";
	auto u = request_.upper_bound();
	if (u == module_loader::request::infinity) os << u8"∞)";
	else os << u << ']';
	os << " fulfilled by [";
	bool first = true;
	for (auto && ptr : fulfilled_by_) {
		if (first) first = false;
		else os << ", ";
		os << ptr->name();
	}
	os << ']';
}

unfulfilled_error::entry::entry (std::shared_ptr<module_loader::offer> offer, requests_type requests)
	:	offer_(std::move(offer)),
		requests_(std::move(requests))
{
	if (!offer_) throw std::logic_error("Expected a module_loader::offer");
	if (requests_.empty()) throw std::logic_error("Expected at least one module_loader::unfulfilled_error::entry::request_details");
}

const unfulfilled_error::entry::requests_type & unfulfilled_error::entry::requests () const noexcept {
	return requests_;
}

const offer & unfulfilled_error::entry::offer () const noexcept {
	return *offer_;
}

void unfulfilled_error::entry::to_string (std::ostream & os) const {
	os << offer_->name() << ':';
	for (auto && request : requests_) {
		os << "\n\t";
		request.to_string(os);
	}
}

static std::string get_what (const unfulfilled_error::entries_type & entries) {
	std::ostringstream ss;
	ss << "Unable to fulfill all requests of the following offers:";
	for (std::size_t i = 0; i < entries.size(); ++i) {
		ss << "\n#" << (i + 1) << ": ";
		entries[i].to_string(ss);
	}
	return ss.str();
}

unfulfilled_error::unfulfilled_error (entries_type entries)
	:	resolver_error(get_what(entries)),
		entries_(std::move(entries))
{
	if (entries_.empty()) throw std::logic_error("Expected at least one module_loader::unfulfilled_error::entry");
}

const unfulfilled_error::entries_type & unfulfilled_error::entries () const noexcept {
	return entries_;
}

}
