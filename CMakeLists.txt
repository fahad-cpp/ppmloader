cmake_minimum_required(VERSION 3.30)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-v -O3 -march=native -flto -fuse-ld=lld -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

project(ppmloader)

file(GLOB SOURCES src/*.cpp resource.rc)

add_subdirectory(core)
add_subdirectory(vendor/FSWindow)

add_executable(ppmloader WIN32 ${SOURCES})
target_link_libraries(ppmloader PUBLIC core FSWindow)
if(MSVC)
    add_link_options(/SUBSYSTEM:WINDOWS)
else()
    add_link_options(-mwindows)
endif()

target_compile_definitions(ppmloader PUBLIC -DNDEBUG -D_NDEBUG)