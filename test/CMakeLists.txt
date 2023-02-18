cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(EVHttpServerTests VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

function(AddCoverage target)
	if(NOT UNIX)
		message(STATUS "Will not build coverage report, only support in unix system")
        return()
	endif()

    find_program(LCOV_PATH lcov)
    if(${LCOV_PATH} STREQUAL "LCOV_PATH-NOTFOUND")
		message(STATUS "lcov not found, will not build coverage report.")
        return()
	endif()

    find_program(GENHTML_PATH genhtml)
    if(${GENHTML_PATH} STREQUAL "GENHTML_PATH-NOTFOUND")
		message(STATUS "genhtml not found, will not build coverage report.")
        return()
	endif()

    add_custom_target(coverage
        COMMENT "Running coverage for ${target}..."
        COMMAND ${LCOV_PATH} -d . --zerocounters
        COMMAND $<TARGET_FILE:${target}>
        COMMAND ${LCOV_PATH} -d . --capture -o coverage.info
        COMMAND ${LCOV_PATH} -r coverage.info '/usr/include/*' -o filtered.info
        COMMAND ${GENHTML_PATH} -o coverage filtered.info --legend
        COMMAND rm -rf coverage.info filtered.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endfunction()

add_executable(unitTest  unitTest.cpp) 
target_include_directories(unitTest PUBLIC ${EVHTTP_ROOT_DIR}/src)

if (WIN32)
    target_link_libraries(unitTest event event_core EVHttpServer gtest)
else()
    target_link_libraries(unitTest event event_pthreads pthread EVHttpServer gtest)
endif()

add_test(NAME unitTest COMMAND unitTest)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    AddCoverage(unitTest)
	install(DIRECTORY "${CMAKE_BINARY_DIR}/coverage" TYPE DOC)
endif()
