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
 *	of an object.
 *
 *	Objects are produces by \ref offer objects when
 *	they are fulfilled.
 */
class object {
public:
	/**
	 *	Represents the collection of types which this
	 *	object provides.
	 */
	using provides_type = std::unordered_set<std::type_index>;
	object () = default;
	object (const object &) = delete;
	object (object &&) = delete;
	object & operator = (const object &) = delete;
	object & operator = (object &&) = delete;
	/**
	 *	Allows derived classes to be cleaned up through
	 *	pointer or reference to base.
	 */
	virtual ~object () noexcept;
	/**
	 *	Obtains a pointer to the managed object.
	 *
	 *	\return
	 *		A pointer or \em nullptr if no object is
	 *		managed.
	 */
	virtual void * get () noexcept = 0;
	/**
	 *	Obtains a pointer to the managed object.
	 *
	 *	\return
	 *		A pointer or \em nullptr if no object is
	 *		managed.
	 */
	virtual const void * get () const noexcept = 0;
	/**
	 *	Determines the type of the managed object.
	 *
	 *	If no object is managed the std::type_info for
	 *	\em void may be returned.
	 *
	 *	\return
	 *		A std::type_info object.
	 */
	virtual const std::type_info & type () const noexcept = 0;
	/**
	 *	Retrieves a human readable name for this object.
	 *
	 *	This should return the same value which was returned
	 *	by the \ref offer which generated this object.
	 *
	 *	\return
	 *		A string.  Note that this function cannot throw
	 *		and that a reference to a std::string is returned
	 *		therefore this string must be constructed ahead
	 *		of time and stored somewhere.
	 */
	virtual const std::string & name () const noexcept = 0;
	/**
	 *	Determines which types this objects provides.
	 *
	 *	This should return the same value which was returned
	 *	by the \ref offer which generated this object.
	 *
	 *	\return
	 *		A set of types.  Note that this function cannot
	 *		throw and that a reference to a std::unordered_set
	 *		is returned therefore this set must be constructed
	 *		ahead of time and stored somewhere.  The actual
	 *		type of the object (i.e. the type yielded by
	 *		calling \ref type) should be included.
	 */
	virtual const provides_type & provides () const noexcept = 0;
};

}
