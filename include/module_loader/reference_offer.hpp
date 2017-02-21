/**
 *	\file
 */

#pragma once

#include "object.hpp"
#include "offer.hpp"
#include "offer_base.hpp"
#include "reference_object.hpp"
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

namespace module_loader {

/**
 *	A \ref offer which offers a reference to a
 *	certair object which already exists.
 *
 *	The wrapped object must continue to exist until
 *	this object is destroyed.
 *
 *	\tparam T
 *		The type to which a reference shall be
 *		offered.
 */
template <typename T>
class reference_offer : public offer_base<T> {
public:
	/**
	 *	A reference to type \em T.
	 */
	using reference_type = std::add_lvalue_reference_t<T>;
private:
	using base = offer_base<T>;
	reference_type ref_;
public:
	using requests_type = typename base::requests_type;
private:
	requests_type rs_;
public:
	using fulfill_type = typename base::fulfill_type;
	/**
	 *	Creates a new reference_offer which uses a default
	 *	name and holds a reference to a certain object.
	 *
	 *	\param [in] ref
	 *		The reference to wrap.
	 */
	explicit reference_offer (reference_type ref) : ref_(ref) {	}
	/**
	 *	Creates a new reference_offer which uses a custom
	 *	name and holds a reference to a certain object.
	 *
	 *	\param [in] ref
	 *		The reference to wrap.
	 *	\param [in] name
	 *		The custom name.
	 */
	reference_offer (reference_type ref, std::string name)
		:	base(std::move(name)),
			ref_(ref)
	{	}
	virtual std::unique_ptr<object> fulfill (const fulfill_type &) override {
		return std::make_unique<reference_object<T>>(*this,ref_);
	}
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type &) override {
		return std::make_shared<reference_object<T>>(*this,ref_);
	}
	virtual const requests_type & requests () const noexcept override {
		return rs_;
	}
};

/**
 *	A helper function which creates a \ref reference_offer
 *	which offers a certain object.
 *
 *	\tparam T
 *		The type of object which shall be referred to.
 *
 *	\param [in] ref
 *		The reference to wrap.
 *
 *	\return
 *		A smart pointer to an \ref offer.
 */
template <typename T>
std::unique_ptr<offer> make_unique_reference_offer (T & ref) {
	return std::make_unique<reference_offer<T>>(ref);
}
template <typename T>
std::shared_ptr<offer> make_shared_reference_offer (T & ref) {
	return std::make_shared<reference_offer<T>>(ref);
}
/**
 *	A helper function which creates a \ref reference_offer
 *	which offers a certain object.
 *
 *	\tparam T
 *		The type of object which shall be referred to.
 *
 *	\param [in] ref
 *		The reference to wrap.
 *	\param [in] name
 *		A custom name for the resulting \ref offer.
 *
 *	\return
 *		A smart pointer to an \ref offer.
 */
template <typename T>
std::unique_ptr<offer> make_unique_reference_offer (T & ref, std::string name) {
	return std::make_unique<reference_offer<T>>(ref,std::move(name));
}
template <typename T>
std::shared_ptr<offer> make_shared_reference_offer (T & ref, std::string name) {
	return std::make_shared<reference_offer<T>>(ref,std::move(name));
}

}
