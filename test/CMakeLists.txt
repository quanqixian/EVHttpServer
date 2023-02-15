cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(EVHttpServerTests VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

add_executable(unitTest  unitTest.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp) 
target_include_directories(unitTest PUBLIC ${EVHTTP_ROOT_DIR}/src)

if (WIN32)
    target_link_libraries(unitTest event event_core EVHttpServer gtest)
else()
    target_link_libraries(unitTest event event_pthreads pthread EVHttpServer gtest)
endif()

target_link_directories(unitTest PUBLIC ${CMAKE_BINARY_DIR}/src)

add_test(NAME unitTest COMMAND unitTest)
