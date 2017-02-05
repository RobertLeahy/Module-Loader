#include <module_loader/queue_shared_library_factory.hpp>
#include <utility>

namespace module_loader {

void queue_shared_library_factory::add (boost::dll::shared_library so) {
	q_.push_back(std::move(so));
}

boost::dll::shared_library queue_shared_library_factory::next () {
	if (q_.empty()) return boost::dll::shared_library{};
	auto retr = std::move(q_.front());
	q_.pop_front();
	return retr;
}

}
