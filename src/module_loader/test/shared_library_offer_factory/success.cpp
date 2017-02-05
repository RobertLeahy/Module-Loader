#include <module_loader/inplace_offer.hpp>
#include <module_loader/offer.hpp>
#include <module_loader/shared_library_offer_factory.hpp>
#include <memory>
#include <utility>

extern "C" {

void load (module_loader::shared_library_offer_factory & slof) {
	std::unique_ptr<module_loader::offer> ptr = std::make_unique<module_loader::inplace_offer<int,int>>();
	slof.add(std::move(ptr));
}

}
