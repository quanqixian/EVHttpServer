cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(3rd_libevent VERSION 1.0)

find_package(Git)
if (NOT Git_FOUND)
	message(FATAL_ERROR "Git not found, can't initialize!")
endif()

#If clone is unsuccessful, try again several times
foreach(count RANGE 1 3)
	execute_process(
		COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch release-2.2.1-alpha https://github.com/libevent/libevent.git
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE result
	)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libevent" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libevent")
		message(STATUS "Clone libevent repository successed...")
		break()
	endif()
	message(WARNING "CMake step [clone] for libevent, may be an error in cloning the repository, try again...")
endforeach()

if(result)
	if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libevent")
		message(FATAL_ERROR "CMake step [clone] for libevent, may be an error in cloning the repository: ${result}")
	endif()
endif()

find_package(OpenSSL)
if (NOT OPENSSL_FOUND)
	set(EVENT__DISABLE_OPENSSL ON CACHE BOOL "Enalbe DISABLE_OPENSSL")
endif()
set(EVENT__DISABLE_MBEDTLS ON CACHE BOOL "Enalbe DISABLE_MBEDTLS")
set(THIRDPARTY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libevent)
add_subdirectory(${THIRDPARTY_SOURCE_DIR})

