/**
 *	\file
 */

#pragma once

#include "object_base.hpp"
#include "offer.hpp"
#include <string>
#include <type_traits>
#include <utility>

namespace module_loader {

/**
 *	A tag type used to indicate that an inplace_object
 *	should be constructed with a default name.
 */
class default_name_t {	};
/**
 *	An instance of \em default_name_t which may be used
 *	to indicate that an inplace_object should be
 *	constructed with a default name.
 */
constexpr default_name_t default_name;

/**
 *	Encapsulates an object which is constructed
 *	and contained within its associated \ref object.
 *
 *	\tparam T
 *		The type of object.
 */
template <typename T>
class inplace_object : public object_base<T> {
private:
	T obj_;
public:
	/**
	 *	Creates an inplace_object which contains
	 *	a \em T.
	 *
	 *	\tparam Ts
	 *		The types of arguments to forward through to
	 *		a constructor of \em T.
	 *
	 *	\param [in] tag
	 *		A dummy parameter to indicate that this constructor
	 *		should be chosen by overload resolution.
	 *	\param [in] args
	 *		Arguments to forward through to a constructor of
	 *		\em T.
	 */
	template <typename... Ts>
	explicit inplace_object (default_name_t tag, Ts &&... args) noexcept(
		std::is_nothrow_constructible<T,Ts...>::value &&
		std::is_nothrow_default_constructible<object_base<T>>::value
	)	:	obj_(std::forward<Ts>(args)...)
	{
		(void)tag;
	}
	/**
	 *	Creates an inplace_object which contains a \em T
	 *	and which has a custom name.
	 *
	 *	\tparam Ts
	 *		The types of arguments to forward through to
	 *		a constructor of \em T.
	 *
	 *	\param [in] name
	 *		The name of this object.
	 *	\param [in] args
	 *		Arguments to forward through to a constructor of
	 *		\em T.
	 */
	template <typename... Ts>
	explicit inplace_object (std::string name, Ts &&... args) noexcept(
		std::is_nothrow_constructible<T,Ts...>::value &&
		std::is_nothrow_constructible<object_base<T>,std::string>::value
	)	:	object_base<T>(std::move(name)),
			obj_(std::forward<Ts>(args)...)
	{	}
	/**
	 *	Creates an inplace_object which contains a \em T
	 *	and draws its name et cetera from an offer.
	 *
	 *	\tparam Ts
	 *		The types of arguments to forward through to
	 *		a constructor of \em T.
	 *
	 *	\param [in] offer
	 *		The offer.
	 *	\param [in] args
	 *		Arguments to forward through to a constructor of
	 *		\em T.
	 */
	template <typename... Ts>
	explicit inplace_object (offer & offer, Ts &&... args)
		:	object_base<T>(offer),
			obj_(std::forward<Ts>(args)...)
	{	}
	virtual void * get () noexcept override {
		return &obj_;
	}
	virtual const void * get () const noexcept override {
		return &obj_;
	}
};

}
