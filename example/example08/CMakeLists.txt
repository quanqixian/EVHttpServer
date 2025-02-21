cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Example08 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

add_executable(example08  example08.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp)
target_include_directories(example08 PUBLIC ${EVHTTP_ROOT_DIR}/src)
set_target_properties(example08 PROPERTIES INSTALL_RPATH "$ORIGIN;../${CMAKE_INSTALL_LIBDIR}")

if (WIN32)
    target_link_libraries(example08 event event_core)
else()
    target_link_libraries(example08 event event_pthreads pthread)
endif()
target_link_directories(example08 PUBLIC ${CMAKE_BINARY_DIR}/src)

message(NOTICE "Copy the html files used in the example08 to the build directory...")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/html08/ DESTINATION ${PROJECT_BINARY_DIR}/html08)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pic08/ DESTINATION ${PROJECT_BINARY_DIR}/pic08)
install(TARGETS example08)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/html08" TYPE BIN)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pic08" TYPE BIN)
