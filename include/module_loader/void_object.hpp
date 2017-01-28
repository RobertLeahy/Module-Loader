/**
 *	\file
 */

#pragma once

#include "object_base.hpp"

namespace module_loader {

/**
 *	Represents an \ref object with a type of
 *	\em void.
 */
class void_object : public object_base<void> {
public:
	using object_base<void>::object_base;
	virtual void * get () noexcept override;
	virtual const void * get () const noexcept override;
};

}
