cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-mwindows -O3 -march=native")

project(core)

file(GLOB SOURCES src/*.cpp)
add_library(core ${SOURCES})

target_include_directories(core PUBLIC src)

target_compile_definitions(core PUBLIC -DNDEBUG -D_NDEBUG)
target_compile_options(core PUBLIC -O3)