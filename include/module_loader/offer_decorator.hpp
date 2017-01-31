/**
 *	\file
 */

#pragma once

#include "object.hpp"
#include "offer.hpp"
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace module_loader {

/**
 *	A convenience class to be used as a base
 *	class when implementing classes which
 *	decorate \ref offer.
 *
 *	\tparam
 *		The smart pointer which shall be used
 *		to manage the lifetime of the inner
 *		\ref offer.
 */
template <typename Pointer>
class offer_decorator : public module_loader::offer {
private:
	Pointer inner_;
protected:
	/**
	 *	Destroys the managed \ref offer.
	 */
	void reset () noexcept {
		inner_.reset();
	}
public:
	offer_decorator () = delete;
	/**
	 *	Creates an offer_decorator.
	 *
	 *	\param [in] inner
	 *		A smart pointer to the \ref offer which
	 *		shall be wrapped.
	 */
	explicit offer_decorator (Pointer inner) noexcept(std::is_nothrow_move_constructible<Pointer>::value)
		:	inner_(std::move(inner))
	{	}
	virtual const std::type_info & type () const noexcept override {
		return inner_->type();
	}
	virtual const std::string & name () const noexcept override {
		return inner_->name();
	}
	virtual const provides_type & provides () const noexcept override {
		return inner_->provides();
	}
	virtual const requests_type & requests () const noexcept override {
		return inner_->requests();
	}
	virtual std::unique_ptr<object> fulfill (const fulfill_type & objects) override {
		return inner_->fulfill(objects);
	}
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type & objects) override {
		return inner_->fulfill_shared(objects);
	}
	/**
	 *	Retrieves the managed \ref offer.
	 *
	 *	\return
	 *		An \ref offer object.
	 */
	const module_loader::offer & offer () const noexcept {
		return *inner_;
	}
	module_loader::offer & offer () noexcept {
		return *inner_;
	}
};

}
