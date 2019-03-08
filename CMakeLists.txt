
cmake_minimum_required(VERSION 3.0)

project(EagleLibrary)

set(EAGLE_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(EAGLE_INCLUDE_DIR ${EAGLE_ROOT_DIR}/include)

add_subdirectory(src)

