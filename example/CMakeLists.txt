set(EXAMPLES
    example01
    example02
    example03
    example04
    example05
    example06
)

set(EVHTTP_ROOT_DIR ..)
set(LIBEVENT_DIR ${EVHTTP_ROOT_DIR}/thirdparty/libevent/install/)

foreach(example ${EXAMPLES})
	add_executable(${example} ${example}/${example}.cpp ${EVHTTP_ROOT_DIR}/src/EVHttpServer.cpp)
	target_include_directories(${example} PUBLIC ${EVHTTP_ROOT_DIR}/src ${LIBEVENT_DIR}/include)
    target_link_libraries(${example} event event_pthreads pthread)
	target_link_directories(${example} PUBLIC ${LIBEVENT_DIR}/lib)
endforeach()

add_custom_target(examples ALL DEPENDS ${EXAMPLES})
