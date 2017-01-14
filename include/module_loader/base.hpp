/**
 *	\file
 */

#pragma once

#include "bases.hpp"
#include "type_name.hpp"
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_set>
#include <utility>

namespace module_loader {

/**
 *	A convenience base class for implementing
 *	\ref object or \ref offer.
 *
 *	\tparam T
 *		The type being represented or offered.
 *	\tparam Base
 *		\ref object or \ref offer.
 */
template <typename T, typename Base>
class base : public Base {
template <typename, typename>
friend class base;
public:
	using provides_type = typename Base::provides_type;
private:
	std::string name_;
	provides_type provides_;
	static constexpr bool is_nothrow = std::is_same<T,void>::value &&
		std::is_nothrow_default_constructible<provides_type>::value &&
		std::is_nothrow_move_constructible<provides_type>::value;
	static provides_type get_provides (std::true_type) noexcept(is_nothrow) {
		return provides_type{};
	}
	static provides_type get_provides (std::false_type) {
		return public_unambiguous_bases<T>();
	}
	static provides_type get_provides () noexcept(is_nothrow) {
		return get_provides(typename std::is_same<T,void>::type{});
	}
public:
	base ()
		:	name_(type_name<T>()),
			provides_(get_provides())
	{	}
	template <typename U, typename V>
	explicit base (base<U,V> & other) noexcept(
		std::is_nothrow_default_constructible<std::string>::value &&
		std::is_nothrow_default_constructible<provides_type>::value &&
		std::is_nothrow_default_constructible<Base>::value
	) {
		using std::swap;
		swap(name_,other.name_);
		swap(provides_,other.provides_);
	}
	explicit base (std::string name) noexcept(is_nothrow && std::is_nothrow_move_constructible<std::string>::value)
		:	name_(std::move(name)),
			provides_(get_provides())
	{	}
	virtual const std::string & name () const noexcept override {
		return name_;
	}
	virtual const provides_type & provides () const noexcept override {
		return provides_;
	}
};

}