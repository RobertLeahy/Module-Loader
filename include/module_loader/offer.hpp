/**
 *	\file
 */

#pragma once

#include "object.hpp"
#include "request.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>

namespace module_loader {

/**
 *	An abstract base class representing the concept
 *	of an offered object.  An offer encapsulates a
 *	possible future object which will become available
 *	if certain objects are supplied to it.
 *
 *	Note that while an offer conceptually represents a
 *	future object and has objects which it requests it
 *	need not do either, in fact in order for a set of
 *	offers to be resolved at least one offer will need
 *	to have no requirements as any cycles in the
 *	dependency graph make it impossible to resolve.
 */
class offer {
public:
	/**
	 *	Represents the collection of types which this
	 *	offer will provide if its requirements are
	 *	fulfilled.
	 */
	using provides_type = std::unordered_set<std::type_index>;
	/**
	 *	Represents the collection of types which this
	 *	offer requires to be fulfilled.
	 */
	using requests_type = std::vector<request>;
	/**
	 *	Represents a collection of fulfilled requirements.
	 */
	using fulfill_type = std::vector<std::pair<void **,std::size_t>>;
	offer () = default;
	offer (const offer &) = delete;
	offer (offer &&) = delete;
	offer & operator = (const offer &) = delete;
	offer & operator = (offer &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up through
	 *	pointer or reference to base.
	 */
	virtual ~offer () noexcept;
	/**
	 *	Retrieves a std::type_info object which represents
	 *	the type of object this offer will provide if it is
	 *	fulfilled.
	 *
	 *	\return
	 *		A pointer to a std::type_info object.  If this is
	 *		the std::type_info object for \em void then this
	 *		is simply a request for objects.
	 */
	virtual const std::type_info & type () const noexcept = 0;
	/**
	 *	Retrieves a human readable name for this object.
	 *
	 *	Note that while this may simply be a human readable
	 *	version of the object's type additional information
	 *	describing the source might be useful.
	 *
	 *	\return
	 *		A string.  Note that this function cannot throw
	 *		and that a reference to a std::string is returned
	 *		therefore this string must be constructed ahead
	 *		of time and stored somewhere.
	 */
	virtual const std::string & name () const noexcept = 0;
	/**
	 *	Determines which types this offer will provide if
	 *	its requirements are fulfilled.
	 *
	 *	\return
	 *		A set of types.  Note that this function cannot
	 *		throw and that a reference to a std::unordered_set
	 *		is returned therefore this set must be constructed
	 *		ahead of time and stored somewhere.  The actual type
	 *		of the object (i.e. the type yielded by calling
	 *		\ref type) should be included.
	 */
	virtual const provides_type & provides () const noexcept = 0;
	/**
	 *	Determines which types this offer requests to be
	 *	fulfilled and in what number.
	 *
	 *	Note that the returned collection is ordered, the
	 *	order in which requests are returned is the order
	 *	in which they will be provided if the offer is
	 *	fulfilled.
	 *
	 *	\return
	 *		An ordered collection of \ref request objects.
	 *		Note that this function cannot throw and that a
	 *		reference to a std::vector is returned therefore
	 *		this vector must be constructed ahead of time
	 *		and stored somewhere.  If the offer does not
	 *		require anything and is available immediately
	 *		an empty collection should be returned.
	 */
	virtual const requests_type & requests () const noexcept = 0;
	/**
	 *	Fulfills the offer, providing the requested objects
	 *	and obtaining an object in return.
	 *
	 *	\param [in] objects
	 *		A collection of objects where each corresponds
	 *		to a request returned by \ref requests.  Each
	 *		item in this collection is a pair: A pointer
	 *		to a collection of pointers to objects, and a
	 *		size indicating how many pointers are available
	 *		through the aforementioned pointer.  It is up
	 *		to the callee to know how to cast these pointers.
	 *		If this method is invoked and the number given
	 *		as the second element of any of these pairs is
	 *		not within the bounds given by the corresponding
	 *		\ref request returned by \ref requests then
	 *		the behavior of this method is undefined.
	 *
	 *	\return
	 *		A smart pointer which manages the lifetime of
	 *		the resulting object.  Returning a smart pointer
	 *		which does not manage an object may result in
	 *		undefined behavior.
	 */
	virtual std::unique_ptr<object> fulfill (const fulfill_type & objects) = 0;
	virtual std::shared_ptr<object> fulfill_shared (const fulfill_type & objects) = 0;
};

}

