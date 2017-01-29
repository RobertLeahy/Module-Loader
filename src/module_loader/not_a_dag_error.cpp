#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/tuple/tuple.hpp>
#include <module_loader/not_a_dag_error.hpp>
#include <algorithm>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace module_loader {

static std::string get_what (const not_a_dag_error::cycles_type & cycles) {
	if (cycles.empty()) throw std::logic_error("Expected at least one cycle");
	std::ostringstream ss;
	ss << "Dependency graph not a directed acyclic graph (DAG) due to the following cycles:";
	std::for_each(
		boost::make_zip_iterator(
			boost::make_tuple(
				cycles.begin(),
				boost::make_counting_iterator(std::size_t(0))
			)
		),
		boost::make_zip_iterator(
			boost::make_tuple(
				cycles.end(),
				boost::make_counting_iterator(cycles.size())
			)
		),
		[&] (const auto & tuple) {
			auto && cycle = boost::get<0>(tuple);
			if (cycle.empty()) throw std::logic_error("Empty cycle");
			auto i = boost::get<1>(tuple);
			ss << "\n#" << (i + 1) << ": ";
			for (auto && ptr : cycle) {
				ss << ptr->name() << " => ";
			}
			ss << cycle.front()->name();
		}
	);
	return ss.str();
}

not_a_dag_error::not_a_dag_error (cycles_type cycles)
	:	resolver_error(get_what(cycles)),
		cycles_(std::move(cycles))
{	}

const not_a_dag_error::cycles_type & not_a_dag_error::cycles () const noexcept {
	return cycles_;
}

}
