#include <module_loader/queue_offer_factory.hpp>
#include <utility>

namespace module_loader {

void queue_offer_factory::add (std::unique_ptr<offer> req) {
	q_.push_back(std::move(req));
}

std::unique_ptr<offer> queue_offer_factory::next () {
	if (q_.empty()) return std::unique_ptr<offer>{};
	auto retr = std::move(q_.front());
	q_.pop_front();
	return retr;
}

std::shared_ptr<offer> queue_offer_factory::next_shared () {
	auto ptr = next();
	if (!ptr) return std::shared_ptr<offer>{};
	return std::shared_ptr<offer>(ptr.release());
}

}
