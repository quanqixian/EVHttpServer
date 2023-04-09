cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Example07 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_executable(example07 example07.cpp)
target_include_directories(example07 PUBLIC ${EVHTTP_ROOT_DIR}/src)
set_target_properties(example07 PROPERTIES INSTALL_RPATH "$ORIGIN;../lib;../lib64")

if (WIN32)
    target_link_libraries(example07 event event_core EVHttpServer)
else()
    target_link_libraries(example07 event event_pthreads pthread EVHttpServer)
endif()

install(TARGETS example07)
