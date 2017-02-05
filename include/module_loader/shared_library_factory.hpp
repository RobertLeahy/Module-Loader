/**
 *	\file
 */

#pragma once

#include <boost/dll/shared_library.hpp>

namespace module_loader {

/**
 *	Provides an interface through which a number of
 *	boost::dll::shared_library object may be acequired.
 */
class shared_library_factory {
public:
	shared_library_factory () = default;
	shared_library_factory (const shared_library_factory &) = delete;
	shared_library_factory (shared_library_factory &&) = delete;
	shared_library_factory & operator = (const shared_library_factory &) = delete;
	shared_library_factory & operator = (shared_library_factory &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up
	 *	through pointer or reference to base.
	 */
	virtual ~shared_library_factory () noexcept;
	/**
	 *	Retrieves the next boost::dll::shared_library
	 *	in the sequence if there is one.
	 *
	 *	\return
	 *		A boost::dll::shared_library which refers
	 *		to a DLL/DSO if one is available, a
	 *		boost::dll::shared_library which does not
	 *		refer to a DLL/DSO otherwise. 
	 */
	virtual boost::dll::shared_library next () = 0;
};

}
