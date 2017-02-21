add_executable(tests
	bases.cpp
	dag_resolver.cpp
	directory_scanning_shared_library_factory.cpp
	exception.cpp
	function_offer.cpp
	in_place_object.cpp
	in_place_offer.cpp
	main.cpp
	offer_factory_composite.cpp
	queue_offer_factory.cpp
	queue_shared_library_factory.cpp
	reference_object.cpp
	shared_library_directory_entry_filter.cpp
	shared_library_offer_factory.cpp
	type_name.cpp
	type_traits.cpp
	unfulfilled_error.cpp
	void_object.cpp
)
target_link_libraries(tests module_loader)
target_include_directories(tests PRIVATE ${CATCH_INCLUDE_DIR})
add_subdirectory(shared_library_offer_factory)
add_test(NAME tests COMMAND tests)
