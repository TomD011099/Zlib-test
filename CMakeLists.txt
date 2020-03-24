cmake_minimum_required(VERSION 3.15)
project(test)

set(CMAKE_CXX_STANDARD 20)

find_package(zlib REQUIRED)

add_executable(test main.cpp)
target_include_directories(test PUBLIC
        "C:/Program Files (x86)/zlib/include"
        PRIVATE src)
target_link_libraries(test
        "C:/Program Files (x86)/zlib/lib/zlib.lib")
