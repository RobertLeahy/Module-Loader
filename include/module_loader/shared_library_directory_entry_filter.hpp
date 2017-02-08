/**
 *	\file
 */

#pragma once

#include "directory_entry_filter.hpp"
#include <boost/filesystem.hpp>

namespace module_loader {

/**
 *	Filters boost::filesystem::directory_entry objects
 *	such that only regular files or symlinks to regular
 *	files which have the appropriate extension to be
 *	shared libraries are included.
 */
class shared_library_directory_entry_filter : public directory_entry_filter {
public:
	virtual bool check (const boost::filesystem::directory_entry &) override;
};

}
