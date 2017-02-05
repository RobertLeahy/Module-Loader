/**
 *	\file
 */

#pragma once

#include "shared_library_factory.hpp"
#include <boost/dll/shared_library.hpp>
#include <deque>

namespace module_loader {

/**
 *	A \ref shared_library_factory which simply
 *	pops boost::dll::shared_library objects
 *	from a queue.
 */
class queue_shared_library_factory : public shared_library_factory {
private:
	std::deque<boost::dll::shared_library> q_;
public:
	queue_shared_library_factory () = default;
	/**
	 *	Adds a boost::dll::shared_library to the queue.
	 *
	 *	\param [in] so
	 *		A boost::dll::shared_library.
	 */
	void add (boost::dll::shared_library so);
	virtual boost::dll::shared_library next () override;
};

}
