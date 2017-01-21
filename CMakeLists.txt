cmake_minimum_required(VERSION 2.6)
project(module_loader LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

#	External libraries
find_package(Catch REQUIRED)

function(add_linker_options)
	foreach(arg ${ARGV})
		set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${arg}")
		set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES}" PARENT_SCOPE)
	endforeach(arg)
endfunction()

#	Where our project's headers live
include_directories(include)

#	General command line arguments to the compiler that should be present
#	on all platforms
add_compile_options(-std=c++1z)
if(DEFINED RELEASE AND RELEASE)
	add_compile_options(-O2)
else()
	add_compile_options(-O0 -g -fno-inline -fno-omit-frame-pointer -Wall -Wpedantic -Wextra -Werror)
	#	Address Sanitizer (non-Windows only)
	if(NOT WIN32)
		#	Address Sanitizer needs to be able to be turned off due
		#	to the fact the AMD OpenCL CPU SDK doesn't play nice with
		#	it
		if(NOT DEFINED USE_ADDRESS_SANITIZER OR USE_ADDRESS_SANITIZER)
			add_compile_options(-fsanitize=address)
			add_linker_options(-fsanitize=address)
		endif()
	endif()
	#	Suppress some of Clang's excessive warnings
	if (DEFINED CMAKE_CXX_COMPILER_ID AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-Wno-unused-private-field)
	endif()
endif()

add_library(module_loader SHARED
	src/module_loader/object.cpp
	src/module_loader/offer.cpp
	src/module_loader/offer_factory.cpp
	src/module_loader/queue_offer_factory.cpp
	src/module_loader/request.cpp
	src/module_loader/type_name.cpp
)
target_link_libraries(module_loader)

add_executable(tests
	src/module_loader/test/bases.cpp
	src/module_loader/test/function_offer.cpp
	src/module_loader/test/inplace_object.cpp
	src/module_loader/test/inplace_offer.cpp
	src/module_loader/test/main.cpp
	src/module_loader/test/queue_offer_factory.cpp
	src/module_loader/test/reference_object.cpp
	src/module_loader/test/type_name.cpp
	src/module_loader/test/type_traits.cpp
)
target_link_libraries(tests module_loader)
target_include_directories(tests PRIVATE ${CATCH_INCLUDE_DIR})

add_custom_target(tests_run ALL
	COMMAND tests
	DEPENDS tests
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
	COMMENT "Run test suite"
)
