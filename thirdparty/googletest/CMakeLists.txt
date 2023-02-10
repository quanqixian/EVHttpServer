cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(3rd_googletest VERSION 1.0)

find_package(Git)
if (NOT Git_FOUND)
	message(FATAL_ERROR "Git not found, can't initialize!")
endif()

execute_process(
	COMMAND ${GIT_EXECUTABLE} clone --depth 1 --branch release-1.10.0 https://github.com/google/googletest.git ./googletest
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	RESULT_VARIABLE result
	)
if(result)
	message(WARNING "CMake step [clone] for googletest maybe error: ${result}")
endif()

set(THIRDPARTY_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/install)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

execute_process(COMMAND ${CMAKE_COMMAND} -B build -S . -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install 
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
)
if(result)
	message(FATAL_ERROR "CMake step [configure] for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build build 
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
)
if(result)
	message(FATAL_ERROR "CMake step [build] for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --install build 
	RESULT_VARIABLE result
	WORKING_DIRECTORY ${THIRDPARTY_SOURCE_DIR}
)
if(result)
	message(FATAL_ERROR "CMake step [install] for googletest failed: ${result}")
endif()
