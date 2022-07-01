CXX=g++
LIBEVENT_DIR=$(shell dirname `find ../thirdparty/libevent/install/  -name libevent.a`)

TARGET=test
$(TARGET): ./example01.cpp ../src/EVHttpServer.cpp 
	$(CXX) $^ -o $@ -g -std=c++11\
        -lpthread  \
		-levent    \
		-levent_pthreads \
        -I../src \
        -I../thirdparty/libevent/install/include \
		-L$(LIBEVENT_DIR) \
		-Wl,-rpath='$$ORIGIN:$$ORIGIN/$(LIBEVENT_DIR)'

.PHONY:clean
clean:
	rm -rf $(TARGET)
