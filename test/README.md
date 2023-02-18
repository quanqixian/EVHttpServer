# 1.How to Generate a Code Coverage Report

## 1.1 system environment

Generating code coverage reports is only supported on Linux systems. You need to ensure that the following tools have been installed on the system:

- **lcov** ：lcov is a graphical front-end for GCC's coverage testing tool gcov.

- **genhtml** ：generate HTML view from LCOV coverage data files

## 1.2 Generate Code Coverage Report Steps

1. Clone the repository. 

```shell
git clone https://github.com/quanqixian/EVHttpServer.git
```

2. Generate the necessary build files. 

   In this step, ` -DCMAKE_BUILD_TYPE=Debug ` needs to be specified. 

```cmake
cd EVHttpServer
cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/path/to/install  -DCMAKE_BUILD_TYPE=Debug 
```

3. Compile the source code. 

```cmake
cmake --build build
```

4. Run tests and generate code coverage reports. 

    cmake --build build -t coverage

5. Install . 

```cmake
cmake --install build
```

Now you can view the report in `${DCMAKE_INSTALL_PREFIX}/doc/coverage` directory.

