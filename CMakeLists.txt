cmake_minimum_required(VERSION 4.0.0)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(MyProject
    main.cpp
    src/FileSystem.cpp
)
