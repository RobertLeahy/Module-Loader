/**
 *	\file
 */

#pragma once

#include "object.hpp"
#include "offer.hpp"
#include "request.hpp"

namespace module_loader {

/**
 *	An abstract base class which may be derived
 *	from to receive events from a resolver.
 */
class resolver_observer {
public:
	resolver_observer () = default;
	resolver_observer (const resolver_observer &) = delete;
	resolver_observer (resolver_observer &&) = delete;
	resolver_observer & operator = (const resolver_observer &) = delete;
	resolver_observer & operator = (resolver_observer &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up
	 *	through pointer or reference to base.
	 */
	virtual ~resolver_observer () noexcept;
	/**
	 *	Encapsulates information about a resolve
	 *	event.
	 */
	class resolve_event {
	private:
		const offer * requester_;
		const offer * provider_;
		const module_loader::request * request_;
	public:
		resolve_event () = delete;
		resolve_event (const resolve_event &) = default;
		resolve_event (resolve_event &&) = default;
		resolve_event & operator = (const resolve_event &) = default;
		resolve_event & operator = (resolve_event &&) = default;
		/**
		 *	Creates a resolve_event.
		 *
		 *	\param [in] requester
		 *		The \ref offer whose request is being
		 *		resolved.
		 *	\param [in] provider
		 *		The \ref offer which provides the type
		 *		requested.
		 *	\param [in] request
		 *		The request being resolved.
		 */
		resolve_event (const offer & requester, const offer & provider, const module_loader::request & request) noexcept;
		/**
		 *	Retrieves the \ref offer which made the
		 *	request which is being resolved.
		 *
		 *	\return
		 *		An \ref offer.
		 */
		const offer & requester () const noexcept;
		/**
		 *	Retrieves the \ref offer which provides
		 *	the type which was requested.
		 *
		 *	\return
		 *		An \ref offer.
		 */
		const offer & provider () const noexcept;
		/**
		 *	Retrieves the \ref request which was
		 *	resolved.
		 *
		 *	\return
		 *		A \ref request.
		 */
		const module_loader::request & request () const noexcept;
	};
	/**
	 *	Invoked when the resolve event occurs.
	 *
	 *	The resolve event occurs whenever a \ref offer
	 *	which provides a type requested by a second
	 *	\ref offer is found.
	 *
	 *	Note that due to the nature of requests there
	 *	may be many resolve events for a single \ref request
	 *	object.
	 *
	 *	\param [in] event
	 *		An event object representing the event.
	 */
	virtual void on_resolve (resolve_event event) = 0;
	/**
	 *	Encapsulates information about a create event.
	 */
	class create_event {
	private:
		const module_loader::offer * offer_;
		const module_loader::object * object_;
	public:
		create_event () = delete;
		create_event (const create_event &) = default;
		create_event (create_event &&) = default;
		create_event & operator = (const create_event &) = default;
		create_event & operator = (create_event &&) = default;
		/**
		 *	Creates a create_event.
		 *
		 *	\param [in] offer
		 *		The offer which was fulfilled.
		 *	\param [in] object
		 *		The resulting object.
		 */
		create_event (const module_loader::offer & offer, const module_loader::object & object) noexcept;
		/**
		 *	Retrieves the offer which was fulfilled.
		 *
		 *	\return
		 *		A \ref offer.
		 */
		const module_loader::offer & offer () const noexcept;
		/**
		 *	Retrieves the object which was created.
		 *
		 *	\return
		 *		A \ref object.
		 */
		const module_loader::object & object () const noexcept;
	};
	/**
	 *	Invoked when the create event occurs.
	 *
	 *	The create event occurs immediately after any \ref object
	 *	is created.
	 *
	 *	\param [in] event
	 *		An event object representing the event.
	 */
	virtual void on_create (create_event event) = 0;
	/**
	 *	Encapsulates information about a destroy event.
	 */
	class destroy_event {
	private:
		const module_loader::object * object_;
	public:
		destroy_event () = delete;
		destroy_event (const destroy_event &) = default;
		destroy_event (destroy_event &&) = default;
		destroy_event & operator = (const destroy_event &) = default;
		destroy_event & operator = (destroy_event &&) = default;
		/**
		 *	Creates a destroy_event.
		 *
		 *	\param [in] object
		 *		The object which is to be destroyed.
		 */
		explicit destroy_event (const module_loader::object & object) noexcept;
		/**
		 *	Retrieves the object which is about to
		 *	be destroyed.
		 *
		 *	\return
		 *		A \ref object.
		 */
		const module_loader::object & object () const noexcept;
	};
	/**
	 *	Invoked when the destroy event occurs.
	 *
	 *	The destroy event occurs immediately before any
	 *	\ref object is destroyed.
	 *
	 *	\param [in] event
	 *		An event object representing the event.
	 */
	virtual void on_destroy (destroy_event event) = 0;
};

}
