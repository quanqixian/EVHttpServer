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
		COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch release-2.1.12-stable https://github.com/libevent/libevent.git
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

set(THIRDPARTY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libevent)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install)

if(NOT ${CMAKE_GENERATOR_PLATFORM} STREQUAL "")
	execute_process(COMMAND ${CMAKE_COMMAND} -B build -S .
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install
		-DEVENT__DISABLE_OPENSSL=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -A
		${CMAKE_GENERATOR_PLATFORM}
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
		)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -B build -S .
		-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install -DEVENT__DISABLE_OPENSSL=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		RESULT_VARIABLE result
		WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
		)
endif()
if(result)
	message(FATAL_ERROR "CMake step [configure] for libevent failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build build --config ${CMAKE_BUILD_TYPE}
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
)
if(result)
	message(FATAL_ERROR "CMake step [build] for libevent failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --install build --config ${CMAKE_BUILD_TYPE}
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
)
if(result)
	message(FATAL_ERROR "CMake step [install] for libevent failed: ${result}")
endif()
