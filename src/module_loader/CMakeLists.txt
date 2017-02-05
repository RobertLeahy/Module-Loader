add_library(module_loader SHARED
	counting_resolver_observer.cpp
	counting_shared_library_offer_factory_observer.cpp
	dag_resolver.cpp
	exception.cpp
	not_a_dag_error.cpp
	object.cpp
	offer.cpp
	offer_factory.cpp
	queue_offer_factory.cpp
	queue_shared_library_factory.cpp
	request.cpp
	resolver_observer.cpp
	shared_library_factory.cpp
	shared_library_offer_factory.cpp
	shared_library_offer_factory_observer.cpp
	type_name.cpp
	unfulfilled_error.cpp
	void_object.cpp
	whereami.cpp
)
target_link_libraries(module_loader ${Boost_LIBRARIES})
add_subdirectory(test)
