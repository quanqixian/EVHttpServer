cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(libEVHttpserver VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTPSERVER_SOURCES
	EVHttpServer.cpp
)

if(EVHTTPSERVER_BUILD_SHARED_LIBS)
	add_library(EVHttpServer_shared SHARED ${EVHTTPSERVER_SOURCES})
	set_target_properties(EVHttpServer_shared PROPERTIES OUTPUT_NAME EVHttpServer)

    if (WIN32)
        target_link_libraries(EVHttpServer_shared event event_core)
    else()
        target_link_libraries(EVHttpServer_shared event event_pthreads pthread)
    endif()

	install(TARGETS EVHttpServer_shared)
endif()

if(EVHTTPSERVER_BUILD_STATIC_LIBS)
	add_library(EVHttpServer_static STATIC ${EVHTTPSERVER_SOURCES})

    #On Windows you should probably give each library a different name, since there is a ".lib" file for both shared and static.
    #But on Linux and Mac you can even give both libraries the same name (e.g. libMyLib.a and libMyLib.so)
    if (WIN32)
        set_target_properties(EVHttpServer_static PROPERTIES OUTPUT_NAME EVHttpServer_static)
        target_link_libraries(EVHttpServer_static event event_core)
    else()
        set_target_properties(EVHttpServer_static PROPERTIES OUTPUT_NAME EVHttpServer)
        target_link_libraries(EVHttpServer_static event event_pthreads pthread)
    endif()

	install(TARGETS EVHttpServer_static)
endif()

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/EVHttpServer.h" TYPE INCLUDE)
