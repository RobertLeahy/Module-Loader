/**
 * \file
 */

#pragma once

#include "object.hpp"
#include "offer.hpp"
#include "offer_factory.hpp"
#include "optional.hpp"
#include "resolver_observer.hpp"
#include "unfulfilled_error.hpp"
#include <cstddef>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace module_loader {

/**
 *	Attempts to resolve \ref offer objects
 *	by forming a directed acyclic graph
 *	(i.e. DAG) which is then topologically
 *	sorted to obtain an order in which the
 *	objects may be constructed.
 */
class dag_resolver {
private:
	offer_factory & of_;
	resolver_observer * ro_;
	class node {
	public:
		using children_type = std::vector<node *>;
	private:
		std::shared_ptr<module_loader::offer> offer_;
		std::vector<std::vector<node *>> depends_on_;
		children_type depended_on_by_;
		object * object_;
	public:
		node () = delete;
		node (const node &) = delete;
		node (node &&) = delete;
		node & operator = (const node &) = delete;
		node & operator = (node &&) = delete;
		explicit node (std::shared_ptr<module_loader::offer>);
		bool ordered_before (const node &, const std::type_index &) const noexcept;
		module_loader::offer & offer () noexcept;
		const module_loader::offer & offer () const noexcept;
		std::shared_ptr<module_loader::offer> offer_shared () const noexcept;
		optional<unfulfilled_error::entry> check_fulfilled () const;
		void resolve (std::size_t i, node &);
		const children_type & children () const noexcept;
		std::unique_ptr<object> create ();
		bool leaf () const noexcept;
		template <typename Set>
		bool next (const Set &) const noexcept;
	};
	std::vector<std::unique_ptr<node>> nodes_;
	std::unordered_map<std::type_index,std::vector<node *>> provides_map_;
	std::vector<std::unique_ptr<object>> objects_;
	void do_resolve (node &, std::size_t, node &);
	std::unique_ptr<object> do_create (node &);
	void get_offers ();
	void create_graph ();
	void check_graph ();
	void topological_sort ();
	void create ();
public:
	dag_resolver () = delete;
	dag_resolver (const dag_resolver &) = delete;
	dag_resolver (dag_resolver &&) = delete;
	dag_resolver & operator = (const dag_resolver &) = delete;
	dag_resolver & operator = (dag_resolver &&) = delete;
	/**
	 *	Destroys all managed objects.
	 *
	 *	Objects shall be destroyed in the opposite order
	 *	of that in which they were created.
	 */
	~dag_resolver () noexcept;
	/**
	 *	Creates a dag_resolver.
	 *
	 *	\param [in] of
	 *		The \ref offer_factory which shall be used
	 *		to obtain \ref offer objects.
	 *	\param [in] ro
	 *		An optional pointer to a \ref resolver_observer
	 *		which shall receive events emitted by the
	 *		newly-created object.  If \em nullptr no
	 *		\ref resolver_observer will receive these events.
	 *		Defaults to \em nullptr.
	 */
	explicit dag_resolver (offer_factory & of, resolver_observer * ro = nullptr);
	/**
	 *	Creates a dag_resolver.
	 *
	 *	\param [in] of
	 *		The \ref offer_factory which shall be used
	 *		to obtain \ref offer objects.
	 *	\param [in] ro
	 *		A \ref resolver_observer which shall receive
	 *		events emitted by the newly-created object.
	 */
	dag_resolver (offer_factory & of, resolver_observer & ro);
	/**
	 *	Destroys all managed objects.
	 *
	 *	Objects shall be destroyed in the opposite order
	 *	of that in which they were created.
	 */
	void clear () noexcept;
	/**
	 *	Pulls \ref offer objects from the associated
	 *	\ref offer_factory to form a dependency graph
	 *	and then attempts to resolve this dependency
	 *	graph by topologically sorting it.
	 */
	void resolve ();
};

}
