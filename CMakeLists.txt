
# This CMakeLists.txt belongs in the root of the project. This is the master cmake file

cmake_minimum_required(VERSION 3.13)

set(EAGLE_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(EAGLE_INCLUDE_DIR ${EAGLE_ROOT_DIR}/include)



project(EagleLibrary)

option(EAGLE_SHARED_LIBRARY_BUILD "Whether to build the shared Eagle library or not" ON)
option(EAGLE_STATIC_LIBRARY_BUILD "Whether to build the static Eagle library or not" OFF)

option(WANT_ALLEGRO5 "If you want to build the Allegro 5 backend for Eagle" ON)
option(WANT_EXAMPLES "If you want to build the example programs" ON)

option(DEBUG_CMAKE "Enable cmake debug output" OFF)

set(CMAKE_INSTALL_PREFIX "/usr")

set (A5DepSearchPath "/usr" CACHE PATH "Where to find the dependencies for the Allegro5 backend")
set(ALLEGRO5_INCLUDE_DIR "${A5DepSearchPath}/include/" CACHE PATH "Where to find the Allegro 5 library's include folder" FORCE)
set(ALLEGRO5_LIB_DIR "${A5DepSearchPath}/lib/" CACHE PATH "Where to find the Allegro 5 library's lib folder" FORCE)

find_library(ALLEGRO5_DEBUGLIB "allegro_monolith-debug.dll"
             PATHS "${ALLEGRO5_LIB_DIR}" 
             DOC "The shared Allegro 5 debugging library")

find_library(ALLEGRO5_LIB "allegro_monolith.dll"
             PATHS "${ALLEGRO5_LIB_DIR}"
             DOC "The shared Allegro 5 release library")
find_library(PHYSFS_LIB "physfs.dll"
             PATHS "${A5DepSearchPath}/lib/"
             DOC "The shared physfs library")


add_subdirectory(src/Eagle)

#[[ Allegro 5 backend for Eagle ]]



if (WANT_ALLEGRO5)
   if ("${PHYSFS_LIB}" STREQUAL "NOTFOUND" OR
       "${ALLEGRO5_LIB}" STREQUAL "NOTFOUND")
      message (SEND_ERROR "WANT_ALLEGRO5 is On but the Allegro 5 some of the Allegro 5 dependencies were not found.")
   endif ("${PHYSFS_LIB}" STREQUAL "NOTFOUND" OR
          "${ALLEGRO5_LIB}" STREQUAL "NOTFOUND")

   add_subdirectory(src/backends/Allegro5)
endif (WANT_ALLEGRO5)

if (WANT_EXAMPLES)
   if (NOT WANT_ALLEGRO5)
      message(SEND_ERROR "Examples need the Allegro 5 Eagle driver")
   endif (NOT WANT_ALLEGRO5)

   add_subdirectory(examples)
   if (WANT_ALLEGRO5)
      foreach(EX IN LISTS EXAMPLESRC)
         get_filename_component(SRC ${EX} NAME_WLE)

         set(EXNAME ${SRC})
         string(APPEND EXNAME "_A5")

         set(EXNAMEDEBUG ${SRC})
         string(APPEND EXNAMEDEBUG "_A5D")

         set(EXNAMESTATIC ${SRC}})
         string(APPEND EXNAMESTATIC "_A5S")

         set(EXNAMESTATICDEBUG ${SRC})
         string(APPEND EXNAMESTATICDEBUG "_A5DS")

         set(OPTS "-Wall" "-Wextra" "-Wshadow")
         set(WINSTATICLIBS "-static" "-ljpeg" "-ldumb" "-lwebp" "-lFLAC" "-ltheora" "-lvorbisfile" "-lvorbis" "-logg" "-lphysfs" "-lfreetype" "-lpng16" "-lzlibstatic" "-ldsound" "-lgdiplus" "-luuid" "-lkernel32" "-lwinmm" "-lpsapi" "-lopengl32" "-lglu32" "-luser32" "-lcomdlg32" "-lgdi32" "-lshell32" "-lole32" "-ladvapi32" "-lws2_32" "-lshlwapi" "-lpthread" "-static-libstdc++" "-static-libgcc")

         if (CMAKE_BUILD_TYPE STREQUAL "Debug")

            if (EAGLE_SHARED_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAMEDEBUG} " ${EX}")
               add_executable(${EXNAMEDEBUG} ${EX})
               target_compile_options(${EXNAMEDEBUG} PUBLIC ${OPTS} "-g")
               target_include_directories(${EXNAMEDEBUG} PUBLIC "e:/usr/include")
               target_link_directories(${EXNAMEDEBUG} PUBLIC "e:/usr/lib")
               target_link_libraries(${EXNAMEDEBUG} PUBLIC EagleA5DebugShared EagleDebugShared "-lallegro_monolith-debug.dll")
               install(TARGETS ${EXNAMEDEBUG} RUNTIME DESTINATION bin)
            endif (EAGLE_SHARED_LIBRARY_BUILD)

            if (EAGLE_STATIC_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAMESTATICDEBUG} " ${EX}")
               add_executable(${EXNAMESTATICDEBUG} ${EX})
               target_compile_options(${EXNAMESTATICDEBUG} PUBLIC ${OPTS} "-g") 
               target_include_directories(${EXNAMESTATICDEBUG} PUBLIC "e:/usr/include")
               target_link_directories(${EXNAMESTATICDEBUG} PUBLIC "e:/usr/lib")
               target_link_libraries(${EXNAMESTATICDEBUG} PUBLIC EagleA5Debug EagleDebug "-lallegro_monolith-debug-static" ${WINSTATICLIBS})
               install(TARGETS ${EXNAMESTATICDEBUG} RUNTIME DESTINATION bin)
            endif (EAGLE_STATIC_LIBRARY_BUILD)

         endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

         if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")

            if (EAGLE_SHARED_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAME} " ${EX}")
               add_executable(${EXNAME} ${EX})
               target_compile_options(${EXNAME} PUBLIC ${OPTS}) 
               target_include_directories(${EXNAME} PUBLIC "e:/usr/include")
               target_link_directories(${EXNAME} PUBLIC "e:/usr/lib")
               target_link_libraries(${EXNAME} PUBLIC EagleA5Shared EagleShared "-lallegro_monolith.dll")
               install(TARGETS ${EXNAME} RUNTIME DESTINATION bin)
            endif (EAGLE_SHARED_LIBRARY_BUILD)

            if (EAGLE_STATIC_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAMESTATIC} " ${EX}")
               add_executable(${EXNAMESTATIC} ${EX})
               target_compile_options(${EXNAMESTATIC} PUBLIC ${OPTS}) 
               target_include_directories(${EXNAMESTATIC} PUBLIC "e:/usr/include")
               target_link_directories(${EXNAMESTATIC} PUBLIC "e:/usr/lib")
               target_link_libraries(${EXNAMESTATIC} PUBLIC EagleA5 Eagle "-lallegro_monolith-static" ${WINSTATICLIBS})
               install(TARGETS ${EXNAMESTATIC} RUNTIME DESTINATION bin)
            endif (EAGLE_STATIC_LIBRARY_BUILD)

         endif (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")

      endforeach()
   endif (WANT_ALLEGRO5)
endif (WANT_EXAMPLES)

install(DIRECTORY include DESTINATION "${CMAKE_INSTALL_PREFIX}")

#[[
#cd /d E:\Downloads\Programming\Tools\CMake\cmake-3.14.0-rc4\build\Utilities\Sphinx && E:\Programs\Python3pt6\Scripts\sphinx-build.exe -c #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx -d #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/doctrees -b htmlhelp #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/Help #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/htmlhelp > build-htmlhelp.log
#]]
