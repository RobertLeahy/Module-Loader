#include <module_loader/reference_offer.hpp>
#include <module_loader/shared_library_offer_factory.hpp>
#include <iostream>

extern "C" {

void load (module_loader::shared_library_offer_factory & slof) {
	slof.add(module_loader::make_unique_reference_offer(std::cout));
	slof.add(module_loader::make_unique_reference_offer(std::cin));
}

}
