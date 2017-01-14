/**
 *	\file
 */

#pragma once

#include "base.hpp"
#include "offer.hpp"
#include <typeinfo>

namespace module_loader {

/**
 *	A convenience base class for implementing
 *	\ref offer.
 *
 *	\tparam T
 *		The type being offered.
 */
template <typename T>
class offer_base : public base<T,offer> {
public:
	using base<T,offer>::base;
	virtual const std::type_info * type () const noexcept override {
		return &typeid(T);
	}
};

template <>
class offer_base<void> : public base<void,offer> {
public:
	using base<void,offer>::base;
	virtual const std::type_info * type () const noexcept override {
		return nullptr;
	}
};

}
