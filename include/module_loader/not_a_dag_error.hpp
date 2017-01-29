/**
 *	\file
 */

#pragma once

#include "offer.hpp"
#include "resolver_error.hpp"
#include <memory>
#include <vector>

namespace module_loader {

/**
 *	Thrown by \ref dag_resolver when it
 *	encounters a dependency graph which is
 *	not a directed acyclic graph (i.e. DAG).
 */
class not_a_dag_error : public resolver_error {
public:
	/**
	 *	A collection of \ref offer objects which form
	 *	a cycle.
	 */
	using cycle_type = std::vector<std::shared_ptr<offer>>;
	/**
	 *	A collection of cycles.
	 */
	using cycles_type = std::vector<cycle_type>;
private:
	cycles_type cycles_;
public:
	not_a_dag_error () = delete;
	not_a_dag_error (const not_a_dag_error &) = default;
	not_a_dag_error (not_a_dag_error &&) = default;
	not_a_dag_error & operator = (const not_a_dag_error &) = default;
	not_a_dag_error & operator = (not_a_dag_error &&) = default;
	/**
	 *	Creates a new not_a_dag_error.
	 *
	 *	\param [in] cycles
	 *		A collection of cycles found in the dependency
	 *		graph. 
	 */
	explicit not_a_dag_error (cycles_type cycles);
	/**
	 *	Retrieves the cycles which were found in the
	 *	dependency graph.
	 *
	 *	\return
	 *		A collection of cycles.
	 */
	const cycles_type & cycles () const noexcept;
};

}
