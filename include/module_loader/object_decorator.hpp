/**
 *	\file
 */

#pragma once

#include "object.hpp"
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace module_loader {

/**
 *	A convenience base class for implementing classes
 *	which decorate \ref object objects.
 *
 *	\tparam
 *		A smart pointer type to use to manage the lifetime
 *		of the managed \ref object.
 */
template <typename Pointer>
class object_decorator : public module_loader::object {
private:
	Pointer inner_;
protected:
	/**
	 *	Destroys the managed \ref object.
	 */
	void reset () noexcept {
		inner_.reset();
	}
public:
	object_decorator () = delete;
	/**
	 *	Creates a new object_decorator.
	 *
	 *	\param [in] inner
	 *		A smart pointer which manages a
	 *		\ref object.
	 */
	explicit object_decorator (Pointer inner) noexcept(std::is_nothrow_move_constructible<Pointer>::value)
		:	inner_(std::move(inner))
	{	}
	virtual void * get () noexcept override {
		return inner_->get();
	}
	virtual const void * get () const noexcept override {
		return inner_->get();
	}
	virtual const std::type_info & type () const noexcept override {
		return inner_->type();
	}
	virtual const std::string & name () const noexcept override {
		return inner_->name();
	}
	virtual const provides_type & provides () const noexcept override {
		return inner_->provides();
	}
	/**
	 *	Retrieves the managed \ref object.
	 *
	 *	\return
	 *		An \ref object.
	 */
	module_loader::object & object () noexcept {
		return *inner_;
	}
	const module_loader::object & object () const noexcept {
		return *inner_;
	}
};

}
