cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Examples VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EXAMPLES
    example01
    example02
    example03
    example04
    example05
)

set(EVHTTP_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

foreach(example ${EXAMPLES})
	add_executable(${example} ${example}/${example}.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp)
	target_include_directories(${example} PUBLIC ${EVHTTP_ROOT_DIR}/src)
	set_target_properties(${example} PROPERTIES INSTALL_RPATH "$ORIGIN;../${CMAKE_INSTALL_LIBDIR}")
    if (WIN32)
        target_link_libraries(${example} event event_core)
    else()
        target_link_libraries(${example} event event_pthreads pthread)
    endif()

	install(TARGETS ${example})
endforeach()

add_custom_target(examples ALL DEPENDS ${EXAMPLES})

add_subdirectory(example06)
add_subdirectory(example07)
add_subdirectory(example08)

