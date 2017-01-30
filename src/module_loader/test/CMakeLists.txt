add_executable(tests
	bases.cpp
	dag_resolver.cpp
	function_offer.cpp
	inplace_object.cpp
	inplace_offer.cpp
	main.cpp
	queue_offer_factory.cpp
	reference_object.cpp
	type_name.cpp
	type_traits.cpp
	unfulfilled_error.cpp
	void_object.cpp
)
target_link_libraries(tests module_loader)
target_include_directories(tests PRIVATE ${CATCH_INCLUDE_DIR})

add_custom_target(tests_run ALL
	COMMAND tests
	DEPENDS tests
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMENT "Run test suite"
)
