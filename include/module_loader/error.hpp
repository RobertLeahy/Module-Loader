/**
 *	\file
 */

#pragma once

#include <stdexcept>

namespace module_loader {

/**
 *	A base class for all \ref module_loader errors.
 */
class error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

}
