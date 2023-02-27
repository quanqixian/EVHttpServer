cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(EVHttpServer VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if(NOT WIN32)
    option(EVHTTPSERVER_WITH_TESTS "Compile and run test executables" ON)
endif()

option(EVHTTPSERVER_WITH_EXAMPLE "Compile examples" ON)
option(EVHTTPSERVER_BUILD_SHARED_LIBS "Build lib as a shared library." ON)
option(EVHTTPSERVER_BUILD_STATIC_LIBS "Build lib as a static library." ON)
option(EVHTTPSERVER_BUILD_DOXYGEN_DOC "Generate API documentation using doxygen." ON)

add_subdirectory(thirdparty)
add_subdirectory(src)

if(EVHTTPSERVER_WITH_EXAMPLE)
	add_subdirectory(example)
endif()

if(EVHTTPSERVER_WITH_TESTS)
	enable_testing()
	include(CTest)
	add_subdirectory(test)
endif()

if(EVHTTPSERVER_BUILD_DOXYGEN_DOC)
	find_package(Doxygen)
	if(DOXYGEN_FOUND)
		add_subdirectory(doxygen)
	else()
		message(WARNING "Doxygen not found, will not build API documentation.")
	endif()
endif()
