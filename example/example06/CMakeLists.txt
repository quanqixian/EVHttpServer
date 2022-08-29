set(EVHTTP_ROOT_DIR ../../)
set(LIBEVENT_DIR ${EVHTTP_ROOT_DIR}/thirdparty/libevent/install/)

add_executable(example06  example06.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp)
target_include_directories(example06 PUBLIC ${EVHTTP_ROOT_DIR}/src ${LIBEVENT_DIR}/include)
target_link_libraries(example06 event event_pthreads pthread)
target_link_directories(example06 PUBLIC ${LIBEVENT_DIR}/lib)
