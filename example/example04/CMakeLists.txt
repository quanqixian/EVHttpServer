cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Example04 VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

set(targetName example04)
add_executable(${targetName}  ${targetName}.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp)
target_include_directories(${targetName} PUBLIC ${EVHTTP_ROOT_DIR}/src)
set_target_properties(${targetName} PROPERTIES INSTALL_RPATH "$ORIGIN;../${CMAKE_INSTALL_LIBDIR}")

if (WIN32)
	target_link_libraries(${targetName} event EVHttpServer)
else()
	target_link_libraries(${targetName} event event_pthreads pthread EVHttpServer)
endif()

install(TARGETS ${targetName})
