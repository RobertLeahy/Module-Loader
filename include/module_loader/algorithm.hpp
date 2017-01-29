/**
 *	\file
 */

#pragma once

namespace module_loader {

/**
 *	Copies from the input sequence to the output
 *	sequence ending either with the first element
 *	for which a unary predicate returns \em true,
 *	or the end of the sequence, whichever comes
 *	first.
 *
 *	\tparam InputIt
 *		The type of iterators to the input sequence.
 *	\tparam OutputIt
 *		The type of iterator to the output sequence.
 *	\tparam UnaryPredicate
 *		The type of predicate which shall be applied
 *		to elements of the input sequence to determine
 *		when copying shall end.
 *
 *	\param [in] begin
 *		An iterator to the beginning of the input
 *		sequence.
 *	\param [in] end
 *		An iterator to one past the end of the input
 *		sequence.
 *	\param [in] d_first
 *		An iterator to the beginning of the output
 *		sequence.
 *	\param [in] pred
 *		A unary predicate which shall be applied to
 *		each element of the input sequence before
 *		it is copied.  If the predicate returns
 *		\em false copying shall continue, otherwise
 *		copying shall end after that element is
 *		copied.
 *
 *	\return
 *		\em d_first after having been incremented
 *		and dereferenced an appropriate number of
 *		times.
 */
template <typename InputIt, typename OutputIt, typename UnaryPredicate>
OutputIt copy_until_inclusive (InputIt begin, InputIt end, OutputIt d_first, UnaryPredicate pred) {
	for (; begin != end; ++begin, ++d_first) {
		bool done(pred(*begin));
		*d_first = *begin;
		if (done) break;
	}
	return d_first;
}

}
