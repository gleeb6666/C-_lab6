cmake_minimum_required(VERSION 3.16)
project(Variant6Coroutines LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(variant6
    src/main.cpp
    src/config.cpp
    src/coroutines.cpp
    src/demo.cpp
    src/transfer_controller.cpp
)

target_include_directories(variant6 PRIVATE include)

if(MSVC)
    target_compile_options(variant6 PRIVATE /W4 /utf-8)
else()
    target_compile_options(variant6 PRIVATE -Wall -Wextra -Wpedantic)
endif()
