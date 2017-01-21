/**
 *	\file
 */

#pragma once

#include "offer.hpp"
#include <memory>

namespace module_loader {

/**
 *	Provides an interface through which a number of
 *	\ref offer objects may be acquired.
 */
class offer_factory {
public:
	offer_factory () = default;
	offer_factory (const offer_factory &) = delete;
	offer_factory (offer_factory &&) = delete;
	offer_factory & operator = (const offer_factory &) = delete;
	offer_factory & operator = (offer_factory &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up
	 *	through pointer or reference to base.
	 */
	virtual ~offer_factory () noexcept;
	/**
	 *	Retrieves the next \ref offer in the sequence
	 *	if there is one.
	 *
	 *	\return
	 *		A smart pointer to the next \ref offer
	 *		or nullptr if there is no such \ref offer.
	 */
	virtual std::unique_ptr<offer> next () = 0;
	virtual std::shared_ptr<offer> next_shared () = 0;
};

}
