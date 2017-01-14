/**
 *	\file
 */

#pragma once

#include "type_traits.hpp"
#include <tr2/type_traits>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_set>

namespace module_loader {

namespace detail {

template <typename, typename>
void public_unambiguous_bases (std::unordered_set<std::type_index> &);

template <typename, typename>
void public_unambiguous_bases (const std::unordered_set<std::type_index> &, std::true_type) {	}

template <typename T, typename List>
void public_unambiguous_bases (std::unordered_set<std::type_index> & set, std::false_type) {
	using current_type = typename List::first::type;
	if (is_public_unambiguous_base_of_v<current_type,T>) {
		set.insert(typeid(current_type));
	}
	public_unambiguous_bases<T,typename List::rest::type>(set);
}

template <typename T, typename List>
void public_unambiguous_bases (std::unordered_set<std::type_index> & set) {
	public_unambiguous_bases<T,List>(set,typename List::empty{});
}

}

/**
 *	Retrieves a set of std::type_index objects
 *	representing all types which are publicly
 *	accessible, unambiguous base classes of
 *	\em T.
 *
 *	\tparam T
 *		The type whose publicly accessible,
 *		unambiguous base classes shall be
 *		retrieved.
 *
 *	\return
 *		A set of std::type_index objects.
 */
template <typename T>
std::unordered_set<std::type_index> public_unambiguous_bases () {
	std::unordered_set<std::type_index> retr;
	retr.insert(typeid(T));
	detail::public_unambiguous_bases<T,typename std::tr2::bases<T>::type>(retr);
	return retr;
}

}
