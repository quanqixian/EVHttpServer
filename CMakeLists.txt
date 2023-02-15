cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(EVHttpServer VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(WITH_TESTS "Compile and run test executables" ON)
option(WITH_EXAMPLE "Compile examples" ON)
option(BUILD_SHARED_LIBS "Build lib as a shared library." ON)
option(BUILD_STATIC_LIBS "Build lib as a static library." ON)
option(BUILD_DOXYGEN_DOC "Generate API documentation using doxygen." ON)

add_subdirectory(thirdparty)
add_subdirectory(src)

if(WITH_EXAMPLE)
	add_subdirectory(example)
endif()

if(BUILD_DOXYGEN_DOC)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		add_subdirectory(doxygen)
	else()
		message(WARNING "Doxygen not found, will not build API documentation.")
	endif()
endif()
