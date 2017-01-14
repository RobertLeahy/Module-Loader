/**
 *	\file
 */

#pragma once

#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>

namespace module_loader {

/**
 *	Represents an error obtaining a human
 *	readable name for a type.
 */
class demangle_error : public std::logic_error {
public:
	using std::logic_error::logic_error;
	demangle_error ();
};

/**
 *	Indicates that the provided name was not
 *	valid under the implementation defined
 *	name mangling rules.
 */
class invalid_name_error : public demangle_error {
public:
	/**
	 *	Creates a new invalid_name_error.
	 *
	 *	\param [in] name
	 *		The name which was found to be
	 *		invalid.
	 */
	explicit invalid_name_error (const char * name);
};

/**
 *	Converts the implementation defined string
 *	returned by std::type_info::name or
 *	std::type_index::name to a human readable
 *	type name
 *
 *	\param [in] name
 *		A null terminated string.
 *
 *	\return
 *		A human readable string.
 */
std::string type_name (const char * name);
std::string type_name (const std::string & name);
/**
 *	Obtains a human readable type name from a
 *	std::type_info object.
 *
 *	\param [in] info
 *		A std::type_info.
 *
 *	\return
 *		A human readable string.
 */
std::string type_name (const std::type_info & info);
/**
 *	Obtains a human readable type name from a
 *	std::type_index object.
 *
 *	\param [in] index
 *		A std::type_index.
 *
 *	\return
 *		A human readable string.
 */
std::string type_name (const std::type_index & index);
/**
 *	Obtains a human readable name for a type.
 *
 *	\tparam T
 *		The type.
 *
 *	\return
 *		A human readable string.
 */
template <typename T>
std::string type_name () {
	return type_name(typeid(T));
}

}
