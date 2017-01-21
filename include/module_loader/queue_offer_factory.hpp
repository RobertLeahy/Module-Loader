/**
 * \file
 */

#pragma once

#include "offer.hpp"
#include "offer_factory.hpp"
#include <deque>
#include <memory>

namespace module_loader {

/**
 *	A \ref offer_factory which simply pops
 *	\ref offer objects from a queue.
 */
class queue_offer_factory : public offer_factory {
private:
	std::deque<std::unique_ptr<offer>> q_;
public:
	queue_offer_factory () = default;
	/**
	 *	Adds a \ref offer to the queue.
	 *
	 *	\param [in] req
	 *		A std::unique_ptr to the \ref offer
	 *		to add.
	 */
	void add (std::unique_ptr<offer> req);
	virtual std::unique_ptr<offer> next () override;
	virtual std::shared_ptr<offer> next_shared () override;
};

}
