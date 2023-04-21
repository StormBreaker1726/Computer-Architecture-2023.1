cmake_minimum_required(VERSION 3.10)

project(Computer-Architecture)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB source_files
        "*.h"
        "*.hpp"
        "*.cpp"
        "source/*.cpp"
        "include/*.h"
        "include/*.hpp"
        "include/plf/*.h"
        )
add_executable(taca ${source_files})

include_directories(./include)