
# This CMakeLists.txt belongs in the root of the project. This is the master cmake file



cmake_minimum_required(VERSION 3.0)

project(EagleLibrary)


option(EAGLE_SHARED_LIBRARY_BUILD "Whether to build the shared Eagle library or not" ON)
option(EAGLE_STATIC_LIBRARY_BUILD "Whether to build the static Eagle library or not" OFF)

option(WANT_ALLEGRO5 "If you want to build the Allegro 5 backend for Eagle" OFF)

option(DEBUG_CMAKE "Enable cmake debug output" OFF)


#set(<variable> <value>... CACHE <type> <docstring> [FORCE])

set(EAGLE_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(EAGLE_INCLUDE_DIR ${EAGLE_ROOT_DIR}/include)

add_subdirectory(src/Eagle)

if (${DEBUG_CMAKE})
   message("Sources :")
   foreach(src ${EAGLE_SOURCE_FILES})
      message(${src})
   endforeach(src)

   message("Includes :")
   foreach(inc ${EAGLE_HEADER_FILES})
      message(${inc})
   endforeach(inc)
endif (${DEBUG_CMAKE})

add_library(EagleObjects OBJECT ${EAGLE_SOURCE_FILES} ${EAGLE_HEADER_FILES})
target_include_directories(EagleObjects PRIVATE ${EAGLE_INCLUDE_DIR})


if (${EAGLE_SHARED_LIBRARY_BUILD})
   add_library(EagleShared SHARED $<TARGET_OBJECTS:EagleObjects>)
endif (${EAGLE_SHARED_LIBRARY_BUILD})

if (${EAGLE_STATIC_LIBRARY_BUILD})
   add_library(EagleStatic STATIC $<TARGET_OBJECTS:EagleObjects>)
endif (${EAGLE_STATIC_LIBRARY_BUILD})



if (${WANT_ALLEGRO5})
   add_subdirectory(src/backends/Allegro5)
endif (${WANT_ALLEGRO5})



