# 1.Overview

[Here](./example08.cpp) is an example of send mjpg stream using EVHttpServer.

# 2.Steps

## 2.1 Prepare

Before running the program, make sure that the `html08` and `pic08` directory is at the same level as example08.

## 2.2 Run the program

```shell
$ ./example08
[I] EVHttpServer.cpp:219 [start] ThreadPool create thread num:5
```

## 2.3 Enter web page

Enter ip:`8080` in the browser, the port number can be modified in the code.

Then mjpg stream will be send to browers.

Closing the webpage will stop streaming.



