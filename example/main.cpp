#include <boost/filesystem.hpp>
#include <module_loader/dag_resolver.hpp>
#include <module_loader/directory_scanning_shared_library_factory.hpp>
#include <module_loader/directory_scanning_shared_library_factory_observer.hpp>
#include <module_loader/function_offer.hpp>
#include <module_loader/offer_factory_composite.hpp>
#include <module_loader/queue_offer_factory.hpp>
#include <module_loader/resolver_observer.hpp>
#include <module_loader/shared_library_offer_factory.hpp>
#include <module_loader/shared_library_offer_factory_observer.hpp>
#include <module_loader/type_name.hpp>
#include <module_loader/whereami.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <string>

namespace {

class observer
	:	public module_loader::directory_scanning_shared_library_factory_observer,
		public module_loader::shared_library_offer_factory_observer,
		public module_loader::resolver_observer
{
public:
	virtual void on_begin_directory (begin_directory_event event) override {
		std::cout << "Beginning scan of " << event.path().string() << std::endl;
	}
	virtual void on_end_directory (end_directory_event event) override {
		std::cout << "Finished scanning " << event.path().string() << std::endl;
	}
	virtual void on_load (load_event event) override {
		std::cout << "Loaded " << event.shared_library().location().string() << std::endl;
	}
	virtual void on_begin_load (begin_load_event event) override {
		std::cout << "Invoking load handler for " << event.shared_library().location().string() << std::endl;
	}
	virtual void on_end_load (end_load_event event) override {
		std::cout << "Load handler for " << event.shared_library().location().string() << " returned" << std::endl;
	}
	virtual void on_add (add_event event) override {
		std::cout << "Offer of " << module_loader::type_name(event.offer().type()) << std::endl;
	}
	virtual void on_resolve (resolve_event event) override {
		std::cout << "Resolve request for " << module_loader::type_name(event.request().type()) << " by " << event.requester().name() << " with " << event.provider().name() << std::endl;
	}
	virtual void on_create (create_event event) override {
		std::cout << "Created " << event.object().name() << std::endl;
	}
	virtual void on_destroy (destroy_event event) override {
		std::cout << "Destroyed " << event.object().name() << std::endl;
	}
};

}

static void main_impl(int, const char **) {
	observer o;
	//	Scans a directory and loads all DLLs/SOs
	//	allowed by the filter
	module_loader::directory_scanning_shared_library_factory shared_library_factory(o);
	shared_library_factory.add(module_loader::current_executable_directory_path() / "example_shared_libraries");
	//	Looks into loaded shared libraries,
	//	invoking a function called "load" to
	//	obtain offers
	module_loader::shared_library_offer_factory so_offer_factory(shared_library_factory,o);
	module_loader::queue_offer_factory queue_offer_factory;
	//	We make a request (without offering anything)
	//	for a std::ostream and a std::istream, we
	//	count on a shared library supplying both of
	//	these
	std::ostream * os = nullptr;
	std::istream * is = nullptr;
	queue_offer_factory.add(
		module_loader::make_unique_function_offer<std::ostream, std::istream>(
			[&] (std::ostream & os_dep, std::istream & is_dep) noexcept {
				os = &os_dep;
				is = &is_dep;
			},
			"example"
		)
	);
	module_loader::offer_factory_composite offer_factory;
	offer_factory.add(queue_offer_factory);
	offer_factory.add(so_offer_factory);
	//	Resolves offers and creates objects
	module_loader::dag_resolver resolver(offer_factory,o);
	resolver.resolve();
	//	These will always be true: resolver.resolve() wouldn't
	//	return if the dependencies couldn't be resolved (it
	//	would throw)
	assert(os);
	assert(is);
	//	Reads the entire std::istream and dumps it into the
	//	std::ostream
	std::string s;
	while (std::getline(*is,s)) {
		*os << s << '\n';
	}
	std::cout << "EOF" << std::endl;
}

int main (int argc, const char ** argv) {
	try {
		try {
			main_impl(argc,argv);
		} catch (const std::exception & ex) {
			std::cerr << "ERROR: " << ex.what() << std::endl;
			throw;
		} catch (...) {
			std::cerr << "ERROR" << std::endl;
			throw;
		}
	} catch (...) {
		return EXIT_FAILURE;
	}
}
