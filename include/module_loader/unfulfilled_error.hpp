/**
 *	\file
 */

#pragma once

#include "offer.hpp"
#include "request.hpp"
#include "resolver_error.hpp"
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace module_loader {

/**
 *	Thrown when one or more \ref request objects
 *	associated with one or more \ref offer objects
 *	cannot be fulfilled.
 */
class unfulfilled_error : public resolver_error {
public:
	/**
	 *	Associates an \ref offer object with all its
	 *	\ref request objects which were not fulfilled.
	 */
	class entry {
	public:
		/**
		 *	Contains details of an unfulfilled \ref request
		 *	object.
		 */
		class request_details {
		public:
			/**
			 *	A collection of pointers to \ref offer objects.
			 */
			using fulfilled_by_type = std::vector<std::shared_ptr<offer>>;
		private:
			std::size_t i_;
			module_loader::request request_;
			fulfilled_by_type fulfilled_by_;
		public:
			request_details () = delete;
			request_details (const request_details &) = default;
			request_details (request_details &&) = default;
			request_details & operator = (const request_details &) = default;
			request_details & operator = (request_details &&) = default;
			/**
			 *	Creates a new request_details object.
			 *
			 *	\param [in] i
			 *		The zero-relative index of the \ref request in
			 *		the collection returned by \ref offer::requests.
			 *	\param [in] req
			 *		The request.
			 *	\param [in] fulfilled_by
			 *		A collection of pointers to \ref offer objects which
			 *		fulfilled the represented \ref request.
			 */
			request_details (std::size_t i, module_loader::request req, fulfilled_by_type fulfilled_by);
			/**
			 *	Returns the zero-relative index of the \ref request
			 *	in the collection returned by \ref offer::requests
			 *	of the associated \ref offer.
			 *
			 *	\return
			 *		An index.
			 */
			std::size_t index () const noexcept;
			/**
			 *	Retrieves the \ref request.
			 *
			 *	\return
			 *		A \ref request.
			 */
			const module_loader::request & request () const noexcept;
			/**
			 *	Retrieves a collection of pointers to \ref offer
			 *	objects which fulfilled the associated \ref request.
			 *
			 *	Note that due to the fact \ref request objects may
			 *	represent a request for multiple objects \ref offer
			 *	objects may fulfill a \ref request without that
			 *	\ref request itself being fulfilled: There may have
			 *	been too few or too many matching \ref offer
			 *	objects.
			 */
			const fulfilled_by_type & fulfilled_by () const noexcept;
			void to_string (std::ostream &) const;
		};
		/**
		 *	A vector of \ref request_details.
		 */
		using requests_type = std::vector<request_details>;
	private:
		std::shared_ptr<module_loader::offer> offer_;
		requests_type requests_;
	public:
		entry () = delete;
		entry (const entry &) = default;
		entry (entry &&) = default;
		entry & operator = (const entry &) = default;
		entry & operator = (entry &&) = default;
		/**
		 *	Creates a new entry.
		 *
		 *	\param [in] offer
		 *		A pointer to the \ref offer.
		 *	\param [in] requests
		 *		A vector containing \ref request_details objects
		 *		which represent all \ref request objects associated
		 *		with the newly-created object's associated \ref offer
		 *		which could not be fulfilled.  Must contain at least
		 *		one object.
		 */
		entry (std::shared_ptr<module_loader::offer> offer, requests_type requests);
		/**
		 *	Retrieves the \ref request_details objects representing
		 *	the requests which could not be fulfilled.
		 *
		 *	\return
		 *		A vector of \ref request_details objects.
		 */
		const requests_type & requests () const noexcept;
		/**
		 *	Retrieves the associated \ref offer object.
		 *
		 *	\return
		 *		An \ref offer object.
		 */
		const module_loader::offer & offer () const noexcept;
		void to_string (std::ostream &) const;
	};
	/**
	 *	A collection of \ref entry objects.
	 */
	using entries_type = std::vector<entry>;
private:
	entries_type entries_;
public:
	unfulfilled_error () = delete;
	unfulfilled_error (const unfulfilled_error &) = default;
	unfulfilled_error (unfulfilled_error &&) = default;
	unfulfilled_error & operator = (const unfulfilled_error &) = default;
	unfulfilled_error & operator = (unfulfilled_error &&) = default;
	/**
	 *	Creates a new unfulfilled_error.
	 *
	 *	\param [in] entries
	 *		A collection of \ref entry objects representing
	 *		the unfulfilled \ref request objects.  There
	 *		must be at least one element in this collection.
	 */
	explicit unfulfilled_error (entries_type entries);
	/**
	 *	Retrieves the \ref entry objects which
	 *	encapsulate the cause of this error.
	 *
	 *	\return
	 *		A collection of \ref entry objects.
	 */
	const entries_type & entries () const noexcept;
};

}
