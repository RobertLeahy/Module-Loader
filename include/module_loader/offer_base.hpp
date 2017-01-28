/**
 *	\file
 */

#pragma once

#include "base.hpp"
#include "offer.hpp"

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
};

}
