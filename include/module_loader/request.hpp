/**
 *	\file
 */

#pragma once

#include <cstddef>
#include <limits>
#include <typeinfo>

namespace module_loader {

/**
 *	Represents a request for a certain number of
 *	objects of a certain type.
 */
class request {
private:
	const std::type_info * type_;
	std::size_t lo_;
	std::size_t hi_;
public:
	/**
	 *	A value which functions as infinity for the
	 *	purposes of specifying an upper bound on the
	 *	number of requested objects.
	 */
	static constexpr std::size_t infinity = std::numeric_limits<std::size_t>::max();
	/**
	 *	Creates a new request.
	 *
	 *	\param [in] type
	 *		A std::type_info representing the
	 *		requested type.
	 *	\param [in] lo
	 *		The lower bound (inclusive) on the
	 *		number of objects requested.  Defaults
	 *		to 1.
	 *	\param [in] hi
	 *		The upper bound (inclusive) on the
	 *		number of objects requested.  Defaults
	 *		to 1.
	 */
	explicit request (const std::type_info & type, std::size_t lo = 1, std::size_t hi = 1) noexcept;
	request () = delete;
	request (const request &) = default;
	request (request &&) = default;
	request & operator = (const request &) = default;
	request & operator = (request &&) = default;
	/**
	 *	Retrieves the requested type.
	 *
	 *	\return
	 *		A std::type_info.
	 */
	const std::type_info & type () const noexcept;
	/**
	 *	Retrieves the inclusive lower bound on the
	 *	number of objects requested.
	 *
	 *	\return
	 *		An integer.
	 */
	std::size_t lower_bound () const noexcept;
	/**
	 *	Retrieves the exclusive upper bound on the
	 *	number of objects requested.
	 *
	 *	\return
	 *		An integer.
	 */
	std::size_t upper_bound () const noexcept;
};

}
