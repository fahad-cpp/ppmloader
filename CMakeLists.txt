cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-mwindows -O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

project(ppmloader)

file(GLOB SOURCES src/*.cpp)

add_subdirectory(core)

add_executable(ppmloader ${SOURCES})

target_link_libraries(ppmloader PUBLIC libppmloader)

target_compile_definitions(ppmloader PUBLIC -DNDEBUG -D_NDEBUG)