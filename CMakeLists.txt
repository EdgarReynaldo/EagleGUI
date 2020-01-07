
# This CMakeLists.txt belongs in the root of the project. This is the master cmake file

cmake_minimum_required(VERSION 3.0)

set(EAGLE_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(EAGLE_INCLUDE_DIR ${EAGLE_ROOT_DIR}/include)



project(EagleLibrary)

option(EAGLE_SHARED_LIBRARY_BUILD "Whether to build the shared Eagle library or not" ON)
option(EAGLE_STATIC_LIBRARY_BUILD "Whether to build the static Eagle library or not" OFF)

option(WANT_ALLEGRO5 "If you want to build the Allegro 5 backend for Eagle" OFF)

option(DEBUG_CMAKE "Enable cmake debug output" OFF)

set (A5DepSearchPath "${EAGLE_ROOT_DIR}/../Allegro525/" CACHE PATH "Where to find the dependencies for the Allegro5 backend")
set(ALLEGRO5_INCLUDE_DIR "$CACHE{A5DepSearchPath}/include/" CACHE PATH "Where to find the Allegro 5 library's include folder")
set(ALLEGRO5_LIB_DIR "$CACHE{A5DepSearchPath}/lib/" CACHE PATH "Where to find the Allegro 5 library's lib folder")

find_library(ALLEGRO5_DEBUGLIB "allegro_monolith-debug"
             PATHS "$CACHE{ALLEGRO5_LIB_DIR}" 
             DOC "The shared Allegro 5 debugging library")

find_library(ALLEGRO5_LIB "allegro_monolith"
             PATHS "$CACHE{ALLEGRO5_LIB_DIR}"
             DOC "The shared Allegro 5 release library")
find_library(PHYSFS_LIB "physfs.dll"
             PATHS "${A5DepSearchPath}/lib/"
             DOC "The shared physfs library")


add_subdirectory(src/Eagle)

#[[ Allegro 5 backend for Eagle ]]


if (${WANT_ALLEGRO5})
   if ("${PHYSFS_LIB}" STREQUAL "NOTFOUND" OR
       "${ALLEGRO5_LIB}" STREQUAL "NOTFOUND")
      message (SEND_ERROR "WANT_ALLEGRO5 is On but the Allegro 5 some of the Allegro 5 dependencies were not found.")
   endif ("${PHYSFS_LIB}" STREQUAL "NOTFOUND" OR
          "${ALLEGRO5_LIB}" STREQUAL "NOTFOUND")

   add_subdirectory(src/backends/Allegro5)
endif (${WANT_ALLEGRO5})

install(DIRECTORY include DESTINATION "${CMAKE_INSTALL_PREFIX}")

#[[
#cd /d E:\Downloads\Programming\Tools\CMake\cmake-3.14.0-rc4\build\Utilities\Sphinx && E:\Programs\Python3pt6\Scripts\sphinx-build.exe -c #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx -d #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/doctrees -b htmlhelp #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/Help #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/htmlhelp > build-htmlhelp.log
#]]
