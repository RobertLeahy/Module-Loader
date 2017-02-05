#include <module_loader/in_place_offer.hpp>
#include <module_loader/object.hpp>
#include <module_loader/offer.hpp>
#include <module_loader/offer_decorator.hpp>
#include <module_loader/shared_library_offer_factory.hpp>
#include <memory>
#include <stdexcept>
#include <utility>

extern "C" {

void load (module_loader::shared_library_offer_factory & slof) {
	using pointer_type = std::unique_ptr<module_loader::offer>;
	class offer : public module_loader::offer_decorator<pointer_type> {
	private:
		using base = offer_decorator<pointer_type>;
	public:
		using base::base;
		virtual std::unique_ptr<module_loader::object> fulfill (const fulfill_type &) override {
			throw std::runtime_error("Test");
		}
		virtual std::shared_ptr<module_loader::object> fulfill_shared (const fulfill_type &) override {
			throw std::runtime_error("Test");
		}
	};
	pointer_type ptr = std::make_unique<offer>(
		std::make_unique<module_loader::in_place_offer<int,int>>()
	);
	slof.add(std::move(ptr));
}

}
