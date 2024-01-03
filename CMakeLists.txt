cmake_minimum_required(VERSION 3.5)

project(library LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(library main.cpp)

install(TARGETS library
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
