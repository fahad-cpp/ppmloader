cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-mwindows -O3")

project(libppmloader)

file(GLOB SOURCES src/*.cpp)
add_library(libppmloader ${SOURCES})

target_include_directories(libppmloader PUBLIC src)

target_compile_definitions(libppmloader PUBLIC -DNDEBUG -D_NDEBUG)
target_compile_options(libppmloader PUBLIC -O3)