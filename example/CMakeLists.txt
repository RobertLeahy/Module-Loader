add_executable(example
	main.cpp
)
target_link_libraries(example module_loader)
add_library(offers_cout_and_cin SHARED
	shared_library.cpp
)
target_link_libraries(offers_cout_and_cin module_loader)
set_target_properties(offers_cout_and_cin PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/example_shared_libraries"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/example_shared_libraries"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example_shared_libraries"
)
