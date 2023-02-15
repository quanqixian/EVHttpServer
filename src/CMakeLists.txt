cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(libEVHttpserver VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTPSERVER_SOURCES
	EVHttpServer.cpp
)

if(BUILD_SHARED_LIBS)
	set(SHARED_LIB EVHttpServer_shared)
	add_library(${SHARED_LIB} SHARED ${EVHTTPSERVER_SOURCES})
	set_target_properties(${SHARED_LIB} PROPERTIES OUTPUT_NAME EVHttpServer)

    if (WIN32)
        target_link_libraries(${SHARED_LIB} event event_core)
    else()
        target_link_libraries(${SHARED_LIB} event event_pthreads pthread)
    endif()

endif()

if(BUILD_STATIC_LIBS)
	set(STATIC_LIB EVHttpServer_static)
	add_library(${STATIC_LIB} STATIC ${EVHTTPSERVER_SOURCES})

    #On Windows you should probably give each library a different name, since there is a ".lib" file for both shared and static.
    #But on Linux and Mac you can even give both libraries the same name (e.g. libMyLib.a and libMyLib.so)
    if (WIN32)
        set_target_properties(${STATIC_LIB} PROPERTIES OUTPUT_NAME EVHttpServer_static)
        target_link_libraries(${STATIC_LIB} event event_core)
    else()
        set_target_properties(${STATIC_LIB} PROPERTIES OUTPUT_NAME EVHttpServer)
        target_link_libraries(${STATIC_LIB} event event_pthreads pthread)
    endif()

endif()
