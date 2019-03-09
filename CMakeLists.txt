
# This CMakeLists.txt belongs in the root of the project. This is the master cmake file



cmake_minimum_required(VERSION 3.0)

project(EagleLibrary)


option(EAGLE_SHARED_LIBRARY_BUILD "Whether to build the shared Eagle library or not" ON)
option(EAGLE_STATIC_LIBRARY_BUILD "Whether to build the static Eagle library or not" OFF)

option(DEBUG_CMAKE "Enable cmake debug output" OFF)



set(EAGLE_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(EAGLE_INCLUDE_DIR ${EAGLE_ROOT_DIR}/include)

set(EAGLE_SOURCE_FILES)
set(EAGLE_INCLUDE_FILES)

add_subdirectory(src)

if (${DEBUG_CMAKE})
   message("Sources :")
   foreach(src ${EAGLE_SOURCE_FILES})
   message(${src})
   endforeach(src)

   message("Includes :")
   foreach(inc ${EAGLE_INCLUDE_FILES})
   message(${inc})
   endforeach(inc)
endif (${DEBUG_CMAKE})

add_library(EagleObjects OBJECT ${EAGLE_SOURCE_FILES} ${EAGLE_INCLUDE_FILES})

