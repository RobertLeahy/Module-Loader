/**
 *	\file
 */

#pragma once

#include "offer.hpp"
#include "offer_factory.hpp"
#include <deque>
#include <memory>

namespace module_loader {

/**
 *	Combines multiple \ref offer_factory objects into
 *	a single \ref offer_factory.
 */
class offer_factory_composite : public offer_factory {
private:
	std::deque<offer_factory *> fs_;
	template <typename T>
	T next_impl ();
public:
	/**
	 *	Adds an \ref offer_factory.
	 *
	 *	\ref offer_factory objects shall have their
	 *	\ref offer objects generated in FIFO order:
	 *	I.e. the first \ref offer_factory added shall
	 *	have its \ref offer objects generated first.
	 *
	 *	\param [in] factory
	 *		The \ref offer_factory to add.
	 */
	void add (offer_factory & factory);
	virtual std::unique_ptr<offer> next () override;
	virtual std::shared_ptr<offer> next_shared () override;
};

}
