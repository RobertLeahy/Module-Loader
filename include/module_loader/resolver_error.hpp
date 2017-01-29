/**
 *	\file
 */

#pragma once

#include "error.hpp"

namespace module_loader {

/**
 *	A base class for all resolver errors.
 */
class resolver_error : public error {
public:
	using error::error;
};

}
