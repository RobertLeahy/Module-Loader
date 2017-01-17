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
 *	An \ref object which wraps a reference.
 *
 *	Note that the lifetime of the referred to
 *	object is not bound to the lifetime of a
 *	reference_object and must be managed
 *	externally in some way.
 *
 *	\tparam T
 *		The type to which a reference shall be
 *		stored.
 */
template <typename T>
class reference_object : public object_base<T> {
public:
	/**
	 *	A reference to type \em T.
	 */
	using reference_type = std::add_lvalue_reference_t<T>;
private:
	using base = object_base<T>;
	reference_type obj_;
public:
	reference_object () = delete;
	/**
	 *	Creates a new reference_object which wraps
	 *	a reference to a certain object and which
	 *	has a default name.
	 *
	 *	\param [in] obj
	 *		The object.
	 */
	explicit reference_object (reference_type obj) noexcept(
		std::is_nothrow_default_constructible<base>::value
	) : obj_(obj) {	}
	/**
	 *	Creates a new reference_object which wraps
	 *	a reference to a certain object and which
	 *	has a custom name.
	 *
	 *	\param [in] name
	 *		The name.
	 *	\param [in] obj
	 *		The object.
	 */
	reference_object (std::string name, reference_type obj) noexcept(
		std::is_nothrow_constructible<base,std::string>::value
	) : base(std::move(name)), obj_(obj) {	}
	/**
	 *	Creates a new reference_object which wraps
	 *	a reference to a certain object and which
	 *	draws its name from a certain offer.
	 *
	 *	\param [in] offer
	 *		The offer.
	 *	\param [in] obj
	 *		The object.
	 */
	reference_object (offer & offer, reference_type obj) : base(offer), obj_(obj) {	}
	virtual void * get () noexcept override {
		return &obj_;
	}
	virtual const void * get () const noexcept override {
		return &obj_;
	}
};

}
