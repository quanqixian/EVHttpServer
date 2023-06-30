cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Example06 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_executable(example06  example06.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp)
target_include_directories(example06 PUBLIC ${EVHTTP_ROOT_DIR}/src)
set_target_properties(example06 PROPERTIES INSTALL_RPATH "$ORIGIN;../${CMAKE_INSTALL_LIBDIR}")
install(TARGETS example06)

if (WIN32)
    target_link_libraries(example06 event)
else()
    target_link_libraries(example06 event event_pthreads pthread)
endif()
target_link_directories(example06 PUBLIC ${CMAKE_BINARY_DIR}/src)

message(NOTICE "Copy the html files used in the example to the build directory...")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/html/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/html)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/html" TYPE BIN)
