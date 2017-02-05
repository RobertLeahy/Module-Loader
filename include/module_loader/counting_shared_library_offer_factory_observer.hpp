/**
 *	\file
 */

#pragma once

#include "shared_library_offer_factory_observer.hpp"
#include <cstddef>

namespace module_loader {

/**
 *	A \ref shared_library_offer_factory_observer
 *	which simply counts the number of times it
 *	receives each event.
 */
class counting_shared_library_offer_factory_observer : public shared_library_offer_factory_observer {
private:
	std::size_t begin_load_;
	std::size_t end_load_;
	std::size_t add_;
public:
	/**
	 *	Creates a new counting_shared_library_offer_factory_observer.
	 */
	counting_shared_library_offer_factory_observer ();
	virtual void on_begin_load (begin_load_event) override;
	virtual void on_end_load (end_load_event) override;
	virtual void on_add (add_event) override;
	/**
	 *	Determines the number of times
	 *	\ref on_begin_load has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t begin_load () const noexcept;
	/**
	 *	Determines the number of times
	 *	\ref on_end_load has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t end_load () const noexcept;
	/**
	 *	Determines the number of times
	 *	\ref on_add has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t add () const noexcept;
};

}
