/**
 *	\file
 */

#pragma once

#include <boost/filesystem/path.hpp>
#include <stdexcept>

namespace module_loader {

/**
 *	Thrown when whereami reports an error.
 */
class wai_error : public std::runtime_error {
public:
	wai_error ();
};

/**
 *	Uses the whereami library to retrieve the path
 *	of the current executable.
 *
 *	\return
 *		The path of the current executable.
 */
boost::filesystem::path current_executable_path ();

/**
 *	Uses the whereami library to retrieve the path
 *	of the directory which contains the current
 *	executable.
 *
 *	\return
 *		The path of the directory which contains the
 *		current executable.
 */
boost::filesystem::path current_executable_directory_path ();

}
