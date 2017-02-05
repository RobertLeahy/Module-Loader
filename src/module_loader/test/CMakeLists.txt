add_executable(tests
	bases.cpp
	dag_resolver.cpp
	exception.cpp
	function_offer.cpp
	in_place_object.cpp
	in_place_offer.cpp
	main.cpp
	queue_offer_factory.cpp
	queue_shared_library_factory.cpp
	reference_object.cpp
	shared_library_offer_factory.cpp
	type_name.cpp
	type_traits.cpp
	unfulfilled_error.cpp
	void_object.cpp
)
target_link_libraries(tests module_loader)
target_include_directories(tests PRIVATE ${CATCH_INCLUDE_DIR})
add_subdirectory(shared_library_offer_factory)
add_custom_target(tests_run ALL
	COMMAND tests
	DEPENDS
		shared_library_offer_factory_fulfill_throws
		shared_library_offer_factory_multiple
		shared_library_offer_factory_none
		shared_library_offer_factory_success
		shared_library_offer_factory_throws
		tests
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMENT "Run test suite"
)
