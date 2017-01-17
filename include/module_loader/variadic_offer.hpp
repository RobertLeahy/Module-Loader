/**
 *	\file
 */

#pragma once

#include "offer_base.hpp"
#include "request.hpp"
#include <cassert>
#include <string>
#include <typeinfo>
#include <utility>

namespace module_loader {

/**
 *	A convenience base class for implementing
 *	offers which request types from a variadic
 *	template parameter pack and which request
 *	exactly one of each such type.
 *
 *	\tparam T
 *		The type being offered.
 *	\tparam Ts
 *		The types being requested.  Exactly one
 *		of each type shall be requested in the
 *		order in which they occur in the pack.
 *		Note that if the same type occurs
 *		multiple times it will be requested
 *		multiple times but in separate requests.
 */
template <typename T, typename... Ts>
class variadic_offer : public offer_base<T> {
private:
	using base = offer_base<T>;
public:
	using requests_type = typename base::requests_type;
	using fulfill_type = typename base::fulfill_type;
private:
	requests_type requests_;
	static requests_type get_requests () {
		return requests_type{request(typeid(Ts))...};
	}
protected:
	/**
	 *	A std::index_sequence which is populated
	 *	for \em Ts.
	 */
	static constexpr std::index_sequence_for<Ts...> index_sequence{};
	/**
	 *	In debug mode sanity checks the fulfillment
	 *	of this object's requests.
	 *
	 *	In release mode does nothing.
	 *
	 *	\param [in] objects
	 *		The objects to check against this object's
	 *		requests.
	 */
	void check_requests (const fulfill_type & objects) {
		assert(objects.size() == requests_.size());
		#ifndef NDEBUG
		for (auto && pair : objects) {
			assert(pair.first);
			assert(pair.second == 1U);
		}
		#endif
	}
public:
	/**
	 *	Creates a variadic_offer with a default name.
	 */
	variadic_offer () : requests_(get_requests()) {	}
	/**
	 *	Creates a variadic_offer with a custom name.
	 *
	 *	\param [in] name
	 *		The name for this offer and the resulting
	 *		object.
	 */
	explicit variadic_offer (std::string name)
		:	base(std::move(name)),
			requests_(get_requests())
	{	}
	virtual const requests_type & requests () const noexcept override {
		return requests_;
	}
};

}
