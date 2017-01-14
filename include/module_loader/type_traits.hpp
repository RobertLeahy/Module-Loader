/**
 *	\file
 */

#pragma once

#include <type_traits>

namespace module_loader {

/**
 *	A type trait which derives from std::true_type
 *	if \em Derived is a publicly accessible,
 *	unambiguous base class of \em Base and from
 *	std::false_type otherwise.
 *
 *	\tparam Base
 *		The base class.
 *	\tparam Derived
 *		The class which shall be checked to see
 *		if it is derived from \em Base.
 */
template <typename Base, typename Derived>
class is_public_unambiguous_base_of : public std::integral_constant<
	bool,
	std::is_convertible<
		std::add_pointer_t<Derived>,
		std::add_pointer_t<Base>
	>::value
> {	};

/**
 *	\em true if \em Base is a publicly accessible,
 *	unambiguous base class of \em Derived, \em false
 *	otherwise.
 *
 *	\tparam Base
 *		The base class.
 *	\tparam Derived
 *		The class which shall be checked to see
 *		if it is derived from \em Base.
 */
template <typename Base, typename Derived>
constexpr bool is_public_unambiguous_base_of_v = is_public_unambiguous_base_of<Base,Derived>::value;

}
