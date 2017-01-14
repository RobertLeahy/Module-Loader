/**
 *	\file
 */

#pragma once

#include "inplace_object.hpp"
#include "object.hpp"
#include "offer_base.hpp"
#include "request.hpp"
#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

namespace module_loader {

/**
 *	An offer which generates \ref inplace_object
 *	objects when fulfilled.
 *
 *	\tparam T
 *		The type of object to offer.
 *	\tparam Ts
 *		The types of objects to request.  Exactly one
 *		instance of each such type shall be requested
 *		and shall be passed through to a constructor
 *		of \em T when the offer is fulfilled.
 */
template <typename T, typename... Ts>
class inplace_offer : public offer_base<T> {
public:
	using fulfill_type = typename offer_base<T>::fulfill_type;
	using requests_type = typename offer_base<T>::requests_type;
private:
	requests_type requests_;
	using index_sequence_t = std::index_sequence_for<Ts...>;
	static constexpr index_sequence_t index_sequence{};
	void check_requests (const fulfill_type & objects) noexcept {
		assert(objects.size() == requests_.size());
		#ifndef NDEBUG
		for (auto && pair : objects) {
			assert(pair.first);
			assert(pair.second == 1U);
		}
		#endif
	}
	template <std::size_t... Is>
	std::unique_ptr<object> fulfill (const fulfill_type & objects, std::index_sequence<Is...>) {
		return std::make_unique<inplace_object<T>>(
			*this,
			*static_cast<Ts *>(*objects[Is].first)...
		);
	}
	template <std::size_t... Is>
	std::shared_ptr<object> fulfill_shared (const fulfill_type & objects, std::index_sequence<Is...>) {
		return std::make_shared<inplace_object<T>>(
			*this,
			*static_cast<Ts *>(*objects[Is].first)...
		);
	}
public:
	/**
	 *	Creates an inplace_offer with a default name.
	 */
	inplace_offer () : requests_{request(typeid(Ts))...} {	}
	/**
	 *	Creates an inplace_offer with a custom name.
	 *
	 *	\param [in] name
	 *		The name for this offer and the resulting
	 *		object.
	 */
	explicit inplace_offer (std::string name)
		:	offer_base<T>(std::move(name)),
			requests_{request(typeid(Ts))...}
	{	}
	virtual const requests_type & requests () const noexcept override {
		return requests_;
	}
	virtual std::unique_ptr<object> fulfill (const fulfill_type & objects) override {
		check_requests(objects);
		return fulfill(objects,index_sequence);
	}
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type & objects) override {
		check_requests(objects);
		return fulfill_shared(objects,index_sequence);
	}
};

}
