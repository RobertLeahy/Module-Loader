/**
 *	\file
 */

#pragma once

#include <typeinfo>

namespace module_loader {

/**
 *	Retrieves a std::type_info object representing
 *	the type of the exception currently being
 *	handled.
 *
 *	\return
 *		A std::type_info object.
 */
const std::type_info & current_exception_type ();

}
