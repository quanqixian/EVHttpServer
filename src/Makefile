CXX=g++
AR=ar
LIBEVENT_DIR=$(shell dirname `find ../thirdparty/libevent/install/  -name libevent.a`)
STATIC_LIB_NAME=libEVHttpServer.a
DYNAMIC_LIB_NAME=libEVHttpServer.so

all:static_lib dynamic_lib

.PHONY:static_lib
static_lib:./EVHttpServer.cpp 
	$(CXX) -c $^ -o EVHttpServer.o -g -std=c++11 \
        -I../thirdparty/libevent/install/include
	$(AR) crs -o $(STATIC_LIB_NAME) EVHttpServer.o 

.PHONY:dynamic_lib
dynamic_lib:./EVHttpServer.cpp 
	$(CXX) -c $^ -o EVHttpServer.o -g -std=c++11 -fPIC\
        -I../thirdparty/libevent/install/include
	$(CXX) -fPIC -shared -o $(DYNAMIC_LIB_NAME) EVHttpServer.o \
        -lpthread \
        -levent \
        -levent_pthreads \
        -L$(LIBEVENT_DIR) \
        -Wl,-rpath='$$ORIGIN'

.PHONY:clean
clean:
	rm -rf $(STATIC_LIB_NAME) $(DYNAMIC_LIB_NAME) *.o
