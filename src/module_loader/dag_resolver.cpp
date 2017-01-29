#include <boost/function_output_iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <module_loader/algorithm.hpp>
#include <module_loader/dag_resolver.hpp>
#include <module_loader/not_a_dag_error.hpp>
#include <module_loader/offer.hpp>
#include <module_loader/resolver_observer.hpp>
#include <module_loader/type_name.hpp>
#include <module_loader/unfulfilled_error.hpp>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace module_loader {

namespace {

enum class provides_category {
	both,
	requests,
	provides
};

provides_category get_provides_category (const offer & o, const std::type_index & ti) noexcept {
	bool provides = o.provides().count(ti) != 0;
	if (!provides) return provides_category::requests;
	auto && rs = o.requests();
	auto end = rs.end();
	bool requests = std::find_if(rs.begin(),end,[&] (const auto & request) noexcept {
		return std::type_index(request.type()) == ti;
	}) != end;
	if (!requests) return provides_category::provides;
	return provides_category::both;
}

}

dag_resolver::node::node (std::shared_ptr<module_loader::offer> offer)
	:	offer_(std::move(offer)),
		object_(nullptr)
{
	depends_on_.resize(offer_->requests().size());
}

bool dag_resolver::node::ordered_before (const node & other, const std::type_index & ti) const noexcept {
	auto cat_a = get_provides_category(*offer_,ti);
	if (cat_a == provides_category::provides) return false;
	auto cat_b = get_provides_category(*other.offer_,ti);
	if (cat_b == provides_category::provides) return false;
	if (cat_a == provides_category::requests) return cat_b != provides_category::requests;
	//	Both provide and request the type-in-question,
	//	we decide that whichever has the higher upper
	//	bound is ordered before so that it has a greater
	//	chance of being fulfilled
	auto && a_reqs = offer_->requests();
	auto && b_reqs = other.offer_->requests();
	auto func = [&] (const auto & offer) noexcept {
		return std::type_index(offer.type()) == ti;
	};
	//	We assume these are found otherwise we'd have
	//	returned above
	auto && req_a = *std::find_if(a_reqs.begin(),a_reqs.end(),func);
	auto && req_b = *std::find_if(b_reqs.begin(),b_reqs.end(),func);
	if (req_a.upper_bound() != req_b.upper_bound()) return req_a.upper_bound() > req_b.upper_bound();
	//	Otherwise whichever has the lower lower bound
	//	goes last
	return req_a.lower_bound() > req_b.lower_bound();
}

offer & dag_resolver::node::offer () noexcept {
	return *offer_;
}

const offer & dag_resolver::node::offer () const noexcept {
	return *offer_;
}

std::shared_ptr<offer> dag_resolver::node::offer_shared () const noexcept {
	return offer_;
}

optional<unfulfilled_error::entry> dag_resolver::node::check_fulfilled () const {
	using entry = unfulfilled_error::entry;
	entry::requests_type requests;
	auto && rs = offer_->requests();
	std::for_each(
		boost::make_zip_iterator(
			boost::make_tuple(
				rs.begin(),
				depends_on_.begin(),
				boost::counting_iterator<std::size_t>(0)
			)
		),
		boost::make_zip_iterator(
			boost::make_tuple(
				rs.end(),
				depends_on_.end(),
				boost::make_counting_iterator(rs.size())
			)
		),
		[&] (const auto & tuple) {
			auto && r = boost::get<0>(tuple);
			auto && nodes = boost::get<1>(tuple);
			auto && i = boost::get<2>(tuple);
			auto n = nodes.size();
			if ((n >= r.lower_bound()) && (n <= r.upper_bound())) return;
			entry::request_details::fulfilled_by_type fulfilled_by;
			fulfilled_by.reserve(nodes.size());
			std::transform(nodes.begin(),nodes.end(),std::back_inserter(fulfilled_by),[] (const auto & ptr) noexcept {
				return ptr->offer_;
			});
			requests.emplace_back(i,r,std::move(fulfilled_by));
		}
	);
	if (requests.empty()) return nullopt;
	return entry(offer_,std::move(requests));
}

[[noreturn]]
static void out_of_range () {
	throw std::out_of_range("Index out of bounds");
}

void dag_resolver::node::resolve (std::size_t i, node & depends_on) {
	if (i >= depends_on_.size()) out_of_range();
	depends_on.depended_on_by_.push_back(this);
	depends_on_[i].push_back(&depends_on);
}

const dag_resolver::node::children_type & dag_resolver::node::children () const noexcept {
	return depended_on_by_;
}

std::unique_ptr<object> dag_resolver::node::create () {
	std::vector<std::vector<void *>> objs;
	objs.reserve(depends_on_.size());
	std::transform(depends_on_.begin(),depends_on_.end(),std::back_inserter(objs),[] (const auto & nodes) {
		std::vector<void *> retr;
		retr.reserve(nodes.size());
		std::transform(nodes.begin(),nodes.end(),std::back_inserter(retr),[] (auto ptr) {
			auto obj = ptr->object_;
			if (obj) return obj->get();
			throw std::logic_error("Incorrect construction order");
		});
		return retr;
	});
	offer::fulfill_type fulfill;
	fulfill.reserve(depends_on_.size());
	std::transform(objs.begin(),objs.end(),std::back_inserter(fulfill),[] (auto & vec) noexcept {
		return std::make_pair(vec.data(),vec.size());
	});
	auto retr = offer_->fulfill(fulfill);
	if (!retr) throw std::logic_error("module_loader::offer::fulfill returned std::unique_ptr which does not manage a pointee");
	object_ = retr.get();
	return retr;
}

bool dag_resolver::node::leaf () const noexcept {
	return std::all_of(depends_on_.begin(),depends_on_.end(),[] (const auto & vec) noexcept {
		return vec.empty();
	});
}

template <typename Set>
bool dag_resolver::node::next (const Set & set) const noexcept {
	return std::all_of(depends_on_.begin(),depends_on_.end(),[&] (const auto & vec) noexcept {
		return std::all_of(vec.begin(),vec.end(),[&] (auto ptr) noexcept {
			return set.count(ptr) != 0;
		});
	});
}

void dag_resolver::do_resolve (node & depends, std::size_t i, node & depends_on) {
	depends.resolve(i,depends_on);
	if (!ro_) return;
	auto && o = depends.offer();
	auto && rs = o.requests();
	if (i >= rs.size()) out_of_range();
	resolver_observer::resolve_event e(o,depends_on.offer(),rs[i]);
	ro_->on_resolve(std::move(e));
}

std::unique_ptr<object> dag_resolver::do_create (node & n) {
	auto retr = n.create();
	if (!ro_) return retr;
	resolver_observer::create_event e(n.offer(),*retr);
	ro_->on_create(std::move(e));
	return retr;
}

void dag_resolver::get_offers () {
	for (;;) {
		auto ptr = of_.next_shared();
		if (!ptr) break;
		auto node_ptr = std::make_unique<node>(std::move(ptr));
		auto && node = *node_ptr;
		nodes_.push_back(std::move(node_ptr));
		for (auto && t : node.offer().provides()) {
			//	This will default construct if it does not
			//	already exist, which is exactly what we
			//	want
			auto && v = provides_map_[t];
			//	Simplest case: This is the only offer
			//	(so far) which provides the type
			if (v.empty()) {
				v.push_back(&node);
				continue;
			}
			//	This is the complex case: Other offers
			//	provide the type.  We attempt to reconcile
			//	this
			v.insert(
				std::lower_bound(v.begin(),v.end(),&node,[&] (const auto & a, const auto & b) noexcept {
					return a->ordered_before(*b,t);
				}),
				&node
			);
		}
	}
}

void dag_resolver::create_graph () {
	for (auto && ptr : nodes_) {
		auto && offer = ptr->offer();
		auto && rs = offer.requests();
		std::for_each(
			boost::make_zip_iterator(
				boost::make_tuple(
					rs.begin(),
					boost::counting_iterator<std::size_t>(0)
				)
			),
			boost::make_zip_iterator(
				boost::make_tuple(
					rs.end(),
					boost::make_counting_iterator(rs.size())
				)
			),
			[&] (auto && tuple) {
				auto && r = boost::get<0>(tuple);
				auto && i = boost::get<1>(tuple);
				auto && t = r.type();
				auto iter = provides_map_.find(t);
				if (iter == provides_map_.end()) return;
				auto end = iter->second.end();
				auto begin = iter->second.begin();
				if (offer.provides().count(t) != 0) begin = std::find(begin,end,ptr.get()) + 1;
				auto upper = r.upper_bound();
				std::size_t dist(end - begin);
				dist = std::min(upper, dist);
				end = begin + dist;
				std::for_each(begin,end,[&] (auto node) {	this->do_resolve(*ptr,i,*node);	});
			}
		);
	}
}

void dag_resolver::check_graph () {
	unfulfilled_error::entries_type entries;
	for (auto && ptr : nodes_) {
		auto entry = ptr->check_fulfilled();
		if (entry) entries.push_back(std::move(*entry));
	}
	if (!entries.empty()) throw unfulfilled_error(std::move(entries));
}

void dag_resolver::topological_sort () {
	//	Initialize the stack with leaf nodes
	std::vector<node *> stack;
	for (auto && ptr : nodes_) {
		if (ptr->leaf()) stack.push_back(ptr.get());
	}
	std::unordered_map<node *,std::size_t> order;
	not_a_dag_error::cycles_type cycles;
	std::size_t i(0);
	while (order.size() != nodes_.size()) {
		//	Here we handle the case where there were
		//	no leaf nodes: We just pick a node that
		//	doesn't have an order yet arbitrarily and
		//	pretend it's a leaf.  The fact that it isn't
		//	a leaf is irrelevant because we'll pick up
		//	cycles and that will prevent this algorithm
		//	from exiting successfully but will allow us
		//	to report all cycles
		if (stack.empty()) {
			auto iter = std::find_if(nodes_.begin(),nodes_.end(),[&] (const auto & ptr) noexcept {
				return order.count(ptr.get()) == 0;
			});
			stack.push_back(iter->get());
		}
		do {
			//	We don't pop immediately because we
			//	need to preserve a trace in case we
			//	encounter a cycle and need to output
			//	it to the list of cycles, so we attempt
			//	to add to the order map.  If this
			//	succeeds (i.e. the node was not already
			//	in the collection) then it remains in
			//	the stack as part of the trace, otherwise
			//	we're backtracking looking for nodes and
			//	we can actually pop
			auto & node = *stack.back();
			auto pair = order.emplace(&node,i);
			if (!pair.second) {
				stack.pop_back();
				continue;
			}
			++i;
			for (auto && ptr : node.children()) {
				//	Detect cycles
				if (order.count(ptr) != 0) {
					not_a_dag_error::cycle_type cycle;
					copy_until_inclusive(
						stack.rbegin(),
						stack.rend(),
						boost::make_function_output_iterator([&] (auto ptr) mutable {
							cycle.push_back(ptr->offer_shared());
						}),
						[&] (auto p) noexcept {	return p == ptr;	}
					);
					cycles.push_back(std::move(cycle));
					continue;
				}
				//	Skip nodes until we're ready
				//	to visit them
				if (!ptr->next(order)) continue;
				//	Enqueue to be visited
				stack.push_back(ptr);
			}
		} while (!stack.empty());
	}
	if (!cycles.empty()) throw not_a_dag_error(std::move(cycles));
	//	Sort the nodes into the correct order
	//	for object construction
	std::sort(nodes_.begin(),nodes_.end(),[&] (const auto & a, const auto & b) noexcept {
		return order[a.get()] < order[b.get()];
	});
}

void dag_resolver::create () {
	objects_.reserve(nodes_.size());
	std::transform(nodes_.begin(),nodes_.end(),std::back_inserter(objects_),[&] (const auto & ptr) {
		return this->do_create(*ptr);
	});
}

dag_resolver::dag_resolver (offer_factory & of, resolver_observer * ro) : of_(of), ro_(ro) {	}

dag_resolver::dag_resolver (offer_factory & of, resolver_observer & ro) : of_(of), ro_(&ro) {	}

dag_resolver::~dag_resolver () noexcept {
	clear();
}

void dag_resolver::clear () noexcept {
	//	This destroys objects in the reverse
	//	of the order in which they were constructed)
	while (!objects_.empty()) {
		if (ro_) {
			auto && obj = *objects_.back();
			resolver_observer::destroy_event e(obj);
			ro_->on_destroy(std::move(e));
		}
		objects_.pop_back();
	}
}

void dag_resolver::resolve () {
	try {
		clear();
		get_offers();
		create_graph();
		check_graph();
		topological_sort();
		create();
	} catch (...) {
		clear();
		throw;
	}
}

}
