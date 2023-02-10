cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(thirdparty VERSION 1.0)

add_subdirectory(libevent)

if(WITH_TESTS)
	add_subdirectory(googletest)
endif()
