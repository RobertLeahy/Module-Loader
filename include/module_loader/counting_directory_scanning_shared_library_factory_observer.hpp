/**
 *	\file
 */

#pragma once

#include "directory_scanning_shared_library_factory_observer.hpp"
#include <cstddef>

namespace module_loader {

/**
 *	A \ref directory_scanning_shared_library_factory_observer
 *	which simply counts the number of times it receives
 *	each event.
 */
class counting_directory_scanning_shared_library_factory_observer : public directory_scanning_shared_library_factory_observer {
private:
	std::size_t begin_directory_;
	std::size_t end_directory_;
	std::size_t load_;
public:
	/**
	 *	Creates a new \ref counting_directory_scanning_shared_library_factory_observer.
	 */
	counting_directory_scanning_shared_library_factory_observer ();
	virtual void on_begin_directory (begin_directory_event) override;
	virtual void on_end_directory (end_directory_event) override;
	virtual void on_load (load_event) override;
	/**
	 *	Determines the number of times
	 *	\ref on_begin_directory has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t begin_directory () const noexcept;
	/**
	 *	Determines the number of times
	 *	\ref on_end_directory has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t end_directory () const noexcept;
	/**
	 *	Determines the number of times
	 *	\ref on_load has been invoked.
	 *
	 *	\return
	 *		The count.
	 */
	std::size_t load () const noexcept;
};

}
