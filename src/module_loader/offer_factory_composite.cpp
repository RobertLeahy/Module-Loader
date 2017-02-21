#include <module_loader/offer_factory.hpp>
#include <module_loader/offer_factory_composite.hpp>
#include <type_traits>

namespace module_loader {

static std::unique_ptr<offer> get (offer_factory & factory, const std::true_type &) {
	return factory.next();
}

static std::shared_ptr<offer> get (offer_factory & factory, const std::false_type &) {
	return factory.next_shared();
}

template <typename T>
T offer_factory_composite::next_impl () {
	typename std::is_same<std::unique_ptr<offer>,T>::type tag;
	while (!fs_.empty()) {
		auto begin = fs_.begin();
		auto retr = get(**begin,tag);
		if (retr) return retr;
		fs_.erase(begin);
	}
	return T{};
}

void offer_factory_composite::add (offer_factory & factory) {
	fs_.push_back(&factory);
}

std::unique_ptr<offer> offer_factory_composite::next () {
	return next_impl<std::unique_ptr<offer>>();
}

std::shared_ptr<offer> offer_factory_composite::next_shared () {
	return next_impl<std::shared_ptr<offer>>(); 
}

}
