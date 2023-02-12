cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(3rd_libevent VERSION 1.0)

find_package(Git)
if (NOT Git_FOUND)
	message(FATAL_ERROR "Git not found, can't initialize!")
endif()

execute_process(
	COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch release-2.1.12-stable https://github.com/libevent/libevent.git
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	RESULT_VARIABLE result
	)
if(result)
	message(WARNING "CMake step [clone] for libevent maybe error: ${result}")
endif()

set(THIRDPARTY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libevent)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install)

execute_process(COMMAND ${CMAKE_COMMAND} -B build -S .
	-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install -DEVENT__DISABLE_OPENSSL=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
)
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