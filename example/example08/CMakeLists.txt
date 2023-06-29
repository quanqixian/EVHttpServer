cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Example08 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_executable(example08  example08.cpp)
target_include_directories(example08 PUBLIC ${EVHTTP_ROOT_DIR}/src)
set_target_properties(example08 PROPERTIES INSTALL_RPATH "$ORIGIN;../${CMAKE_INSTALL_LIBDIR}")

if (WIN32)
	target_link_libraries(example08 event event_core EVHttpServer)
else()
	target_link_libraries(example08 event event_core event_pthreads pthread EVHttpServer)
endif()

install(TARGETS example08)
message(NOTICE "Copy the html files used in the example to the build directory...")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/html/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/html)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/html" TYPE BIN)
