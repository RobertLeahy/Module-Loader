/**
 *	\file
 */

#pragma once

#include <boost/dll/shared_library.hpp>
#include <boost/filesystem.hpp>

namespace module_loader {

/**
 *	A base class which may be derived from to
 *	receive events from a \ref directory_scanning_shared_library_factory.
 */
class directory_scanning_shared_library_factory_observer {
public:
	directory_scanning_shared_library_factory_observer () = default;
	directory_scanning_shared_library_factory_observer (const directory_scanning_shared_library_factory_observer &) = delete;
	directory_scanning_shared_library_factory_observer (directory_scanning_shared_library_factory_observer &&) = delete;
	directory_scanning_shared_library_factory_observer & operator = (const directory_scanning_shared_library_factory_observer &) = delete;
	directory_scanning_shared_library_factory_observer & operator = (directory_scanning_shared_library_factory_observer &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up through
	 *	pointer or reference to base.
	 */
	virtual ~directory_scanning_shared_library_factory_observer () noexcept;
	/**
	 *	Encapsulates all information about a begin
	 *	directory event.
	 */
	class begin_directory_event {
	private:
		boost::filesystem::path path_;
	public:
		begin_directory_event () = delete;
		begin_directory_event (const begin_directory_event &) = default;
		begin_directory_event (begin_directory_event &&) = default;
		begin_directory_event & operator = (const begin_directory_event &) = default;
		begin_directory_event & operator = (begin_directory_event &&) = default;
		explicit begin_directory_event (boost::filesystem::path);
		/**
		 *	Retrieves a boost::filesystem::path which
		 *	represents the path to the directory.
		 *
		 *	\return
		 *		A boost::filesystem::path.
		 */
		const boost::filesystem::path & path () const noexcept;
	};
	/**
	 *	Invoked when the begin directory event occurs.
	 *
	 *	The begin directory event occurs before the
	 *	\ref directory_scanning_shared_library_factory
	 *	scans the directory which is the subject of
	 *	the event.
	 *
	 *	\param [in] event
	 *		An object which represents the event.
	 */
	virtual void on_begin_directory (begin_directory_event event) = 0;
	/**
	 *	Encapsulates all information about an end
	 *	directory event.
	 */
	using end_directory_event = begin_directory_event;
	/**
	 *	Invoked when the end directory event occurs.
	 *
	 *	The end directory event occurs after the
	 *	\ref directory_scanning_shared_library_factory
	 *	scans the directory which is the subject of
	 *	the event.
	 *
	 *	\param [in] event
	 *		An object which represents the event.
	 */
	virtual void on_end_directory (end_directory_event event) = 0;
	/**
	 *	Encapsulates all information about a load
	 *	event.
	 */
	class load_event {
	private:
		boost::dll::shared_library so_;
	public:
		load_event () = delete;
		load_event (const load_event &) = default;
		load_event (load_event &&) = default;
		load_event & operator = (const load_event &) = default;
		load_event & operator = (load_event &&) = default;
		explicit load_event (boost::dll::shared_library);
		/**
		 *	Retrieves a boost::dll::shared_library object
		 *	which represents the shared library which is the
		 *	subject of the event.
		 *
		 *	\return
		 *		A boost::dll::shared_library.
		 */
		const boost::dll::shared_library & shared_library () const noexcept;
	};
	/**
	 *	Invoked when a shared library is loaded into
	 *	memory.
	 *
	 *	\param [in] event
	 *		An object which represents the event.
	 */
	virtual void on_load (load_event event) = 0;
};

}
