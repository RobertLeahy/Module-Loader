/**
 *	\file
 */

#pragma once

#include "directory_entry_filter.hpp"
#include "directory_scanning_shared_library_factory_observer.hpp"
#include "optional.hpp"
#include "shared_library_factory.hpp"
#include <boost/dll/shared_library.hpp>
#include <boost/filesystem.hpp>
#include <set>

namespace module_loader {

/**
 *	Scans directories loading all shared libraries
 *	found therein and yielding them.
 */
class directory_scanning_shared_library_factory : public shared_library_factory {
private:
	//	Using set rather than unordered_set so
	//	that the directories are traversed in
	//	a sensible/consistent order
	using paths_type = std::set<boost::filesystem::path>;
	paths_type paths_;
	boost::filesystem::directory_iterator dir_;
	optional<paths_type::const_iterator> path_;
	directory_entry_filter * filter_;
	directory_scanning_shared_library_factory_observer * o_;
	bool next_path ();
	bool next_entry ();
	bool next_library ();
	void dispatch_begin_directory () const;
	void dispatch_end_directory () const;
	void dispatch_load (const boost::dll::shared_library &) const;
public:
	/**
	 *	Creates a directory_scanning_shared_library_factory which
	 *	does not scan any directories and which filters files
	 *	it loads as shared libraries using
	 *	\ref shared_library_directory_entry_filter.
	 */
	directory_scanning_shared_library_factory ();
	/**
	 *	Creates a directory_scanning_shared_library_factory which
	 *	does not scan any directories and which filters the files
	 *	it attempts to load as shared libraries using a certain
	 *	\ref directory_entry_filter.
	 *
	 *	\param [in] filter
	 *		The \ref directory_entry_filter.
	 */
	explicit directory_scanning_shared_library_factory (directory_entry_filter & filter);
	/**
	 *	Creates a directory_scanning_shared_library_factory which
	 *	does not scan any directories, which filters files it loads
	 *	as shared libraries using \ref shared_library_directory_entry_filter,
	 *	and which dispatches events to a certain
	 *	\ref directory_scanning_shared_library_factory_observer.
	 *
	 *	\param [in] o
	 *		The \ref directory_scanning_shared_library_factory_observer.
	 */
	explicit directory_scanning_shared_library_factory (directory_scanning_shared_library_factory_observer & o);
	/**
	 *	Creates a directory_scanning_shared_library_factory which
	 *	does not scan any directory, which filters the files
	 *	it attempts to load as shared libraries using a certain
	 *	\ref directory_entry_filter, and which dispatches events
	 *	to a certain \ref directory_scanning_shared_library_factory_observer.
	 *
	 *	\param [in] filter
	 *		The \ref directory_entry_filter.
	 *	\param [in] o
	 *		The \ref directory_scanning_shared_library_factory_observer.
	 */
	directory_scanning_shared_library_factory (directory_entry_filter & filter, directory_scanning_shared_library_factory_observer & o);
	virtual boost::dll::shared_library next () override;
	/**
	 *	Adds a directory to scan.
	 *
	 *	\param [in] path
	 *		A path to the directory to scan.
	 *
	 *	\return
	 *		\em true if \em path was added, \em false
	 *		otherwise.
	 */
	bool add (boost::filesystem::path path);
};

}
