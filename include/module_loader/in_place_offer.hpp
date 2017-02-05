/**
 *	\file
 */

#pragma once

#include "in_place_object.hpp"
#include "object.hpp"
#include "variadic_offer.hpp"
#include "request.hpp"
#include <cstddef>
#include <memory>
#include <utility>

namespace module_loader {

/**
 *	An offer which generates \ref in_place_object
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
class in_place_offer : public variadic_offer<T,Ts...> {
private:
	using base = variadic_offer<T,Ts...>;
public:
	using fulfill_type = typename base::fulfill_type;
private:
	template <std::size_t... Is>
	std::unique_ptr<object> fulfill (const fulfill_type & objects, std::index_sequence<Is...>) {
		return std::make_unique<in_place_object<T>>(
			*this,
			*static_cast<Ts *>(*objects[Is].first)...
		);
	}
	template <std::size_t... Is>
	std::shared_ptr<object> fulfill_shared (const fulfill_type & objects, std::index_sequence<Is...>) {
		return std::make_shared<in_place_object<T>>(
			*this,
			*static_cast<Ts *>(*objects[Is].first)...
		);
	}
public:
	using base::base;
	virtual std::unique_ptr<object> fulfill (const fulfill_type & objects) override {
		base::check_requests(objects);
		return fulfill(objects,base::index_sequence);
	}
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type & objects) override {
		base::check_requests(objects);
		return fulfill_shared(objects,base::index_sequence);
	}
};

}
