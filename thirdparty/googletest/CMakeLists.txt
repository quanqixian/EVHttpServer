cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(3rd_googletest VERSION 1.0)

find_package(Git)
if (NOT Git_FOUND)
	message(FATAL_ERROR "Git not found, can't initialize!")
endif()

if (WIN32)
    set(GTEST_BRANCH release-1.8.0)
else()
    set(GTEST_BRANCH release-1.10.0)
endif()

#If clone is unsuccessful, try again several times
foreach(count RANGE 1 3)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch ${GTEST_BRANCH} https://github.com/google/googletest.git ./googletest
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE result
	)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest" AND IS_DIRECTORY
			"${CMAKE_CURRENT_SOURCE_DIR}/googletest")
		message(STATUS "Clone googletest repository successed...")
		break()
	endif()
	message(WARNING "CMake step [clone] for googletest, may be an error in cloning the repository, try again...")
endforeach()

if(result)
	if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
		message(FATAL_ERROR "CMake step [clone] for googletest, may be an error in cloning the repository: ${result}")
	endif()
endif()

set(THIRDPARTY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest)

add_subdirectory(${THIRDPARTY_SOURCE_DIR})
