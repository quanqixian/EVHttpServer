cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(libEVHttpserver VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EVHTTPSERVER_SOURCES
    EVHttpServer.cpp
)

#   If the EVHTTPSVR_EXPORT_SYMBOL macro is defined, EVHTTPSVR_DLL_DECLARE will be
# defined as __declspec( dllexport ), which is used to export symbols when
#  compiling the EVHttpServer library.
#
add_definitions(-DEVHTTPSVR_EXPORT_SYMBOL)

if(EVHTTPSERVER_BUILD_SHARED_LIBS)
    add_library(EVHttpServer SHARED ${EVHTTPSERVER_SOURCES})
    set_target_properties(EVHttpServer PROPERTIES OUTPUT_NAME EVHttpServer)
    set_target_properties(EVHttpServer PROPERTIES INSTALL_RPATH "$ORIGIN")

    if (WIN32)
        target_link_libraries(EVHttpServer event event_core)
        target_compile_options(EVHttpServer PRIVATE "/W4")
    else()
        target_link_libraries(EVHttpServer event event_pthreads pthread)
        target_compile_options(EVHttpServer PRIVATE "-Wall")
    endif()

    if ((CMAKE_BUILD_TYPE STREQUAL Debug) AND UNIX)
        target_compile_options(EVHttpServer PRIVATE --coverage)
        target_link_options(EVHttpServer PUBLIC --coverage)
        add_custom_command(TARGET EVHttpServer PRE_BUILD 
            COMMAND find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +
            )
    endif()

    install(TARGETS EVHttpServer)
endif()

if(EVHTTPSERVER_BUILD_STATIC_LIBS)
    add_library(EVHttpServer_static STATIC ${EVHTTPSERVER_SOURCES})

    #On Windows you should probably give each library a different name, since there is a ".lib" file for both shared and static.
    #But on Linux and Mac you can even give both libraries the same name (e.g. libMyLib.a and libMyLib.so)
    if (WIN32)
        set_target_properties(EVHttpServer_static PROPERTIES OUTPUT_NAME EVHttpServer_static)
        target_link_libraries(EVHttpServer_static event event_core)
        target_compile_options(EVHttpServer_static PRIVATE "/W4")
    else()
        set_target_properties(EVHttpServer_static PROPERTIES OUTPUT_NAME EVHttpServer)
        target_link_libraries(EVHttpServer_static event event_pthreads pthread)
        target_compile_options(EVHttpServer_static PRIVATE "-Wall")
    endif()

    if ((CMAKE_BUILD_TYPE STREQUAL Debug) AND UNIX)
        target_compile_options(EVHttpServer_static PRIVATE --coverage)
        target_link_options(EVHttpServer_static PUBLIC --coverage)
        add_custom_command(TARGET EVHttpServer_static PRE_BUILD 
            COMMAND find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +
            )
    endif()

    install(TARGETS EVHttpServer_static)
endif()

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/EVHttpServer.h" TYPE INCLUDE)
