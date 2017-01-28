/**
 *	\file
 */

#pragma once

#include "base.hpp"
#include "object.hpp"

namespace module_loader {

/**
 *	A convenience base class for implementing
 *	classes derived from \ref object.
 *
 *	\tparam T
 *		The type of object.
 */
template <typename T>
class object_base : public base<T,object> {
public:
	using base<T,object>::base;
};

}
