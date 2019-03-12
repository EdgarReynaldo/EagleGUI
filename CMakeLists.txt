
# This CMakeLists.txt belongs in the root of the project. This is the master cmake file



cmake_minimum_required(VERSION 3.0)

project(EagleLibrary)


option(EAGLE_SHARED_LIBRARY_BUILD "Whether to build the shared Eagle library or not" ON)
option(EAGLE_STATIC_LIBRARY_BUILD "Whether to build the static Eagle library or not" OFF)

option(WANT_ALLEGRO5 "If you want to build the Allegro 5 backend for Eagle" OFF)

option(DEBUG_CMAKE "Enable cmake debug output" OFF)

set(ALLEGRO5DIR "" CACHE PATH "Where to find the Allegro 5 library's include and lib folders")

set (A5SearchPath "/usr/local/" "c:/mingw/" "$CACHE{ALLEGRO5DIR}/")

find_library(A5DEBUGDLL "allegro_monolith-debug"
             PATHS "${A5SearchPath}" 
             DOC "The shared Allegro 5 debugging library")

find_library(A5DEBUGDLL "allegro_monolith"
             PATHS "${A5SearchPath}"
             DOC "The shared Allegro 5 release library")

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
target_compile_options(EagleObjects PRIVATE "-std=c++11")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
   target_compile_definitions(EagleObjects PRIVATE DEBUG)
   target_compile_definitions(EagleObjects PRIVATE EAGLE_LOG)
   target_compile_definitions(EagleObjects PRIVATE EAGLE_ASSERT_EXCEPTION)
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")




if (${EAGLE_SHARED_LIBRARY_BUILD})
   add_library(EagleShared SHARED $<TARGET_OBJECTS:EagleObjects>)
endif (${EAGLE_SHARED_LIBRARY_BUILD})

if (${EAGLE_STATIC_LIBRARY_BUILD})
   add_library(EagleStatic STATIC $<TARGET_OBJECTS:EagleObjects>)
endif (${EAGLE_STATIC_LIBRARY_BUILD})



#[[ Allegro 5 backend for Eagle ]]


if (${WANT_ALLEGRO5})
   add_subdirectory(src/backends/Allegro5)
   
   if (${DEBUG_CMAKE})
      message("Eagle Allegro 5 Sources :")
      foreach(src ${EAGLE_A5_SOURCE_FILES})
         message(${src})
      endforeach(src)

      message("Eagle Allegro 5 Includes :")
      foreach(inc ${EAGLE_A5_HEADER_FILES})
         message(${inc})
      endforeach(inc)
   endif (${DEBUG_CMAKE})

   if ("$CACHE{ALLEGRO5DIR}" STREQUAL "")
      message (SEND_ERROR "WANT_ALLEGRO5 is On but the Allegro 5 directory (ALLEGRO5DIR) is not set.")
   endif ("$CACHE{ALLEGRO5DIR}" STREQUAL "")
   
   add_library(EagleA5Objects OBJECT ${EAGLE_A5_SOURCE_FILES} ${EAGLE_A5_HEADER_FILES})

   
   target_compile_options(EagleA5Objects PRIVATE "-std=c++11")
   
   if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      target_compile_definitions(EagleA5Objects PRIVATE DEBUG)
      target_compile_definitions(EagleA5Objects PRIVATE EAGLE_LOG)
      target_compile_definitions(EagleA5Objects PRIVATE EAGLE_ASSERT_EXCEPTION)
   endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   
   target_include_directories(EagleA5Objects PRIVATE ${EAGLE_INCLUDE_DIR})
   target_include_directories(EagleA5Objects PRIVATE "${EAGLE_INCLUDE_DIR}/backends/Allegro5/")
   target_include_directories(EagleA5Objects PRIVATE "${ALLEGRO5DIR}/include/")
   
   
   if (${EAGLE_SHARED_LIBRARY_BUILD})
      add_library(EagleA5Shared SHARED $<TARGET_OBJECTS:EagleA5Objects>)
      add_dependencies(EagleA5Shared EagleShared)
      target_link_libraries(EagleA5Shared EagleShared)
      if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
         target_link_libraries(EagleA5Shared "${A5DEBUGDLL}")
      elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
         target_link_libraries(EagleA5Shared "${A5DLL}")
      endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   endif (${EAGLE_SHARED_LIBRARY_BUILD})

   if (${EAGLE_STATIC_LIBRARY_BUILD})
      add_library(EagleA5Static STATIC $<TARGET_OBJECTS:EagleA5Objects>)
   endif (${EAGLE_STATIC_LIBRARY_BUILD})
   
   
endif (${WANT_ALLEGRO5})


#[[
#cd /d E:\Downloads\Programming\Tools\CMake\cmake-3.14.0-rc4\build\Utilities\Sphinx && E:\Programs\Python3pt6\Scripts\sphinx-build.exe -c #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx -d #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/doctrees -b htmlhelp #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/Help #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/htmlhelp > build-htmlhelp.log
#]]
