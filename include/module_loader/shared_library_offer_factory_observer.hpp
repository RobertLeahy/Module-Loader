/**
 *	\file
 */

#pragma once

#include "offer.hpp"
#include <boost/dll/shared_library.hpp>

namespace module_loader {

/**
 *	A base class for classes which wish to receive
 *	events from a \ref shared_library_offer_factory.
 */
class shared_library_offer_factory_observer {
public:
	shared_library_offer_factory_observer () = default;
	shared_library_offer_factory_observer (const shared_library_offer_factory_observer &) = delete;
	shared_library_offer_factory_observer (shared_library_offer_factory_observer &&) = delete;
	shared_library_offer_factory_observer & operator = (const shared_library_offer_factory_observer &) = delete;
	shared_library_offer_factory_observer & operator = (shared_library_offer_factory_observer &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up
	 *	through pointer or reference to base.
	 */
	virtual ~shared_library_offer_factory_observer () noexcept;
	/**
	 *	Encapsulates all information about a begin load event.
	 */
	class begin_load_event {
	private:
		boost::dll::shared_library so_;
	public:
		begin_load_event () = delete;
		begin_load_event (const begin_load_event &) = default;
		begin_load_event (begin_load_event &&) = default;
		begin_load_event & operator = (const begin_load_event &) = default;
		begin_load_event & operator = (begin_load_event &&) = default;
		explicit begin_load_event (boost::dll::shared_library so);
		/**
		 *	Retrieves the boost::dll::shared_library object
		 *	which represents the shared library whose load
		 *	handler is about to be invoked.
		 *
		 *	\return
		 *		A boost::dll::shared_library object.
		 */
		const boost::dll::shared_library & shared_library () const noexcept;
	};
	/**
	 *	Invoked before a shared library's load handler
	 *	is invoked.
	 *
	 *	\param [in] event
	 *		An event object representing the event.
	 */
	virtual void on_begin_load (begin_load_event event) = 0;
	/**
	 *	Encapsulates all information about an end load
	 *	event.
	 */
	using end_load_event = begin_load_event;
	/**
	 *	Invoked after a shared library's load handler
	 *	is invoked.
	 *
	 *	\param [in] event
	 *		An event object representing the event.
	 */
	virtual void on_end_load (end_load_event event) = 0;
	/**
	 *	Encapsulates all information about an add event.
	 */
	class add_event : public begin_load_event {
	private:
		boost::dll::shared_library so_;
		const module_loader::offer * o_;
	public:
		add_event () = delete;
		add_event (const add_event &) = default;
		add_event (add_event &&) = default;
		add_event & operator = (const add_event &) = default;
		add_event & operator = (add_event &&) = default;
		add_event (boost::dll::shared_library so, const module_loader::offer & o);
		/**
		 *	Retrieves the offer which was added.
		 *
		 *	\return
		 *		An \ref offer object.
		 */
		const module_loader::offer & offer () const noexcept;
	};
	/**
	 *	Invoked when shared_library code adds a \ref offer
	 *	to the \ref shared_library_offer_factory.
	 *
	 *	\param [in] event
	 *		An event object representing the event.
	 */
	virtual void on_add (add_event event) = 0;
};

}
