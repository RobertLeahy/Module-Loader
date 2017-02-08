/**
 *	\file
 */

#pragma once

#include <boost/filesystem.hpp>

namespace module_loader {

/**
 *	An interface which may be implemented to filter
 *	boost::filesystem::directory_entry objects.
 */
class directory_entry_filter {
public:
	directory_entry_filter () = default;
	directory_entry_filter (const directory_entry_filter &) = delete;
	directory_entry_filter (directory_entry_filter &&) = delete;
	directory_entry_filter & operator = (const directory_entry_filter &) = delete;
	directory_entry_filter & operator = (directory_entry_filter &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up through
	 *	pointer or reference to base.
	 */
	virtual ~directory_entry_filter () noexcept;
	/**
	 *	Checks to see if a directory entry should be included
	 *	in or excluded from the set formed by the
	 *	directory_entry_filter.
	 *
	 *	\param [in] entry
	 *		The entry to check.
	 *
	 *	\return
	 *		\em true if \em entry is included, \em false
	 *		otherwise.
	 */
	virtual bool check (const boost::filesystem::directory_entry & entry) = 0;
};

}
