/**
 *	\file
 */

#pragma once

#include <string>
#include <unordered_set>
#include <typeindex>
#include <typeinfo>

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
	 *		A pointer to a std::type_info object.  Note that
	 *		this may be \em nullptr in which case this offer
	 *		is simply a request for objects which does not
	 *		actually offer anything.
	 */
	virtual const std::type_info * type () const noexcept = 0;
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
};

}

