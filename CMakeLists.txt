
# This CMakeLists.txt belongs in the root of the project. This is the master cmake file

cmake_minimum_required(VERSION 3.1.13)

if (CMAKE_MAJOR_VERSION LESS_EQUAL 3 AND CMAKE_MINOR_VERSION LESS_EQUAL 13)
   function(target_link_directories)# LINKTARGET DIRS)
      foreach(ARG ${ARGV2})
         target_link_options(${ARGV1} PUBLIC "-L${ARG}")
      endforeach()
   endfunction()
   message(STATUS "target_link_directories function added")
endif()

project(EagleLibrary)

message(STATUS "Getting ready to build Eagle")

set(EAGLE_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(EAGLE_INCLUDE_DIR ${EAGLE_ROOT_DIR}/include)

option(EAGLE_SHARED_LIBRARY_BUILD "Whether to build the shared Eagle library or not" ON)
if (UNIX)
   message(STATUS "Static build is disabled on Linux")
else ()
   option(EAGLE_STATIC_LIBRARY_BUILD "Whether to build the static Eagle library or not" OFF)
endif()

option(WANT_ALLEGRO5 "If you want to build the Allegro 5 backend for Eagle" ON)
option(WANT_EXAMPLES "If you want to build the example programs (At least one driver must be built first)" ON)

option(DEBUG_CMAKE "Enable cmake debug output" OFF)
option(DEBUG_ALLEGRO5 "Enable debugging of allegro 5" OFF)

option(EAGLE_DEBUG_MUTEX_LOCKS "Enable debug logging of mutex locks on all threads created with Eagle" OFF)

set (A5_MONOLITH "allegro_monolith" CACHE STRING "The allegro 5 monolith base name")
set (A5LIBS
"allegro_acodec"
"allegro_audio"
"allegro_color"
"allegro_font"
"allegro_image"
"allegro_memfile"
"allegro_physfs"
"allegro_primitives"
"allegro_ttf"
"allegro_video"
"allegro"
"allegro_main"
CACHE STRING "A list of the allegro addon libraries")

# Set search directories

set (A5DEPDIR "/usr" CACHE PATH "Where to find the Allegro 5 library")

if (A5DEPDIR)
   set(A5INCDIR "${A5DEPDIR}/include/" CACHE PATH "Where to find the Allegro 5 library's include folder" FORCE)
   set(A5LIBDIR "${A5DEPDIR}/lib/"     CACHE PATH "Where to find the Allegro 5 library's lib folder" FORCE)
else()
   set(A5INCDIR "/usr/include/" CACHE PATH "Where to find the Allegro 5 library's include folder")
   set(A5LIBDIR "/usr/lib/"     CACHE PATH "Where to find the Allegro 5 library's lib folder")
endif()

# Find allegro5.h header

unset(A5_INCLUDE_DIRECTORY CACHE)
find_path (A5_INCLUDE_DIRECTORY "allegro5/allegro5.h" PATHS ${A5INCDIR} NO_DEFAULT_PATH)
if (A5_INCLUDE_DIRECTORY)
   message(STATUS "Allegro 5 header found at ${A5INCDIR}/allegro5")
endif()
find_path (A5_INCLUDE_DIRECTORY "allegro5/allegro5.h" PATHS "/usr/include/allegro5" "/usr/local/include/allegro5")
if (NOT A5_INCLUDE_DIRECTORY)
   message(SEND_ERROR "Failed to find allegro5.h")
endif()


set(LIBEXT ".so")
if (WIN32)
   set(LIBEXT ".dll.a")
endif()

if (DEBUG_ALLEGRO5)
   string(PREPEND LIBEXT "-debug")
endif()

string(APPEND ${A5_MONOLITH} ${LIBEXT})

message(STATUS ${A5LIBS})

# Find Allegro 5 monolith

unset(A5_MONOLITH_LIB CACHE)
find_library(A5_MONOLITH_LIB "lib${A5_MONOLITH}" PATHS ${A5LIBDIR} NO_DEFAULT_PATH)
find_library(A5_MONOLITH_LIB "lib${A5_MONOLITH}" PATHS "/usr/lib" "/usr/local/lib" "/usr/lib/x86_64-linux-gnu")

unset(ALLEGRO_LINK_LIBS CACHE)
if (NOT A5_MONOLITH_LIB)
   # Find allegro addons in case monolith is not there
   message(STATUS "Allegro 5 monolith library not found. Falling back to addons.")
   foreach(A5LIB ${A5LIBS})
      unset(A5LIBVAR CACHE)
      find_library(A5LIBVAR "lib${A5LIB}${LIBEXT}" PATHS ${A5LIBDIR} NO_DEFAULT_PATH)
      find_library(A5LIBVAR "lib${A5LIB}${LIBEXT}" PATHS "/usr/lib" "/usr/local/lib" "/usr/lib/x86_64-linux-gnu")
      message(STATUS ${A5LIBVAR})
      if(NOT A5LIBVAR)
         message(SEND_ERROR "Failed to find addon lib ${A5LIB}")
      endif()
      list(APPEND ALLEGRO_LINK_LIBS ${A5LIBVAR})
   endforeach()
else ()
   list(APPEND ALLEGRO_LINK_LIBS ${A5_MONOLITH_LIB})
endif()

if (DEBUG_CMAKE)
   message(STATUS "ALLEGRO_LINK_LIBS = ${ALLEGRO_LINK_LIBS}")
endif()

#Finding PhysFS

unset(PHYSFS_LIB CACHE)
if (WIN32)
   find_library(PHYSFS_LIB "physfs.dll"
                PATHS "${A5LIBDIR}" "/usr/lib" "/usr/local/lib"
                DOC "The shared physfs library")
endif()

if (UNIX)
   find_library(PHYSFS_LIB "physfs"
                PATHS "${A5LIBDIR}" "/usr/lib" "/usr/local/lib"
                DOC "The shared physfs library")
endif()

list(APPEND ALLEGRO_LINK_LIBS ${PHYSFS_LIB})

add_subdirectory(src/Eagle)

#[[ Allegro 5 backend for Eagle ]]

set(LIBCOUNT 0)
list(LENGTH ALLEGRO_LINK_LIBS LIBCOUNT)

if (WANT_ALLEGRO5)
   if (NOT LIBCOUNT)
      message(SEND_ERROR "Could not find allegro libs and WANT_ALLEGRO5 is on")
   endif()
   if (${PHYSFS_LIB} STREQUAL "NOTFOUND")
      message(SEND_ERROR "WANT_ALLEGRO5 is on but Physfs library was not found")
   endif()
   add_subdirectory(src/backends/Allegro5)
endif (WANT_ALLEGRO5)

if (WANT_EXAMPLES)
   if (NOT WANT_ALLEGRO5)
      message(SEND_ERROR "Examples need the Allegro 5 Eagle driver at this time.")
   endif (NOT WANT_ALLEGRO5)

   add_subdirectory(examples)
   if (WANT_ALLEGRO5)
      foreach(EX IN LISTS EXAMPLESRC)
         get_filename_component(SRC ${EX} NAME_WE)

         set(EXNAME ${SRC})
         string(APPEND EXNAME "_A5")

         set(EXNAMEDEBUG ${SRC})
         string(APPEND EXNAMEDEBUG "_A5D")

         set(EXNAMESTATIC ${SRC})
         string(APPEND EXNAMESTATIC "_A5S")

         set(EXNAMESTATICDEBUG ${SRC})
         string(APPEND EXNAMESTATICDEBUG "_A5DS")

         set(OPTS "-std=c++11" "-pedantic" "-Wall" "-Wextra" "-Wshadow")
         if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            list(APPEND OPTS "-g" "-fno-inline" "-O0")
         endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
         
         
         set(WINSTATICLIBS "-static" "-lOpenAL32" "-ljpeg" "-ldumb" "-lwebp" "-lFLAC" "-ltheora" "-lvorbisfile" "-lvorbis" "-logg" "-lfreetype" "-lpng16" "-lzlibstatic" "-ldsound" "-lgdiplus" "-luuid" "-lkernel32" "-lwinmm" "-lpsapi" "-lopengl32" "-lglu32" "-luser32" "-lcomdlg32" "-lgdi32" "-lshell32" "-lole32" "-ladvapi32" "-lws2_32" "-lshlwapi" "-lpthread" "-static-libstdc++" "-static-libgcc")

         if (CMAKE_BUILD_TYPE STREQUAL "Debug")
            if (EAGLE_SHARED_LIBRARY_BUILD)
               message(STATUS "Building example ${EXNAMEDEBUG} ${EX}")
               add_executable(${EXNAMEDEBUG} ${EX})
               target_compile_options(${EXNAMEDEBUG} PUBLIC ${OPTS})
               if (EAGLE_DEBUG_MUTEX_LOCKS)
                  target_compile_definitions(${EXNAMEDEBUG} PRIVATE EAGLE_DEBUG_MUTEX_LOCKS)
               endif(EAGLE_DEBUG_MUTEX_LOCKS)
               target_include_directories(${EXNAMEDEBUG} PUBLIC "${EAGLE_INCLUDE_DIR}" "${A5_INCLUDE_DIRECTORY}")
               target_link_directories(${EXNAMEDEBUG} PUBLIC "${A5LIBDIR}")
               target_link_libraries(${EXNAMEDEBUG} PUBLIC EagleA5DebugShared EagleDebugShared)
               target_link_libraries(${EXNAMEDEBUG} PUBLIC ${ALLEGRO_LINK_LIBS})
               install(TARGETS ${EXNAMEDEBUG} RUNTIME DESTINATION bin)
            endif (EAGLE_SHARED_LIBRARY_BUILD)

            if (EAGLE_STATIC_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAMESTATICDEBUG} " ${EX}")
               add_executable(${EXNAMESTATICDEBUG} ${EX})
               target_compile_definitions(${EXNAMESTATICDEBUG} PRIVATE "ALLEGRO_STATICLINK" "AL_LIBTYPE_STATIC")
               target_compile_options(${EXNAMESTATICDEBUG} PUBLIC ${OPTS}) 
               target_include_directories(${EXNAMESTATICDEBUG} PUBLIC "${EAGLE_INCLUDE_DIR}" "${A5_INCLUDE_DIRECTORY}")
               target_link_directories(${EXNAMESTATICDEBUG} PUBLIC "${A5LIBDIR}")
               target_link_libraries(${EXNAMESTATICDEBUG} PUBLIC EagleA5Debug EagleDebug ${ALLEGRO_LINK_LIBS} ${WINSTATICLIBS})
               install(TARGETS ${EXNAMESTATICDEBUG} RUNTIME DESTINATION bin)
            endif (EAGLE_STATIC_LIBRARY_BUILD)

         endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

         if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")

            if (EAGLE_SHARED_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAME} " ${EX}")
               add_executable(${EXNAME} ${EX})
               target_compile_options(${EXNAME} PUBLIC ${OPTS}) 
               target_include_directories(${EXNAME} PUBLIC "${EAGLE_INCLUDE_DIR}" "${A5_INCLUDE_DIRECTORY}")
               target_link_directories(${EXNAME} PUBLIC "${ALLEGRO5_LIB_DIR}")
               target_link_libraries(${EXNAME} PUBLIC EagleA5Shared EagleShared)
               target_link_libraries(${EXNAME} PUBLIC ${ALLEGRO_LINK_LIBS})
               install(TARGETS ${EXNAME} RUNTIME DESTINATION bin)
            endif (EAGLE_SHARED_LIBRARY_BUILD)

            if (EAGLE_STATIC_LIBRARY_BUILD)
               message(STATUS "Building example " ${EXNAMESTATIC} " ${EX}")
               add_executable(${EXNAMESTATIC} ${EX})
               target_compile_definitions(${EXNAMESTATIC} PRIVATE "ALLEGRO_STATICLINK" "AL_LIBTYPE_STATIC")
               target_compile_options(${EXNAMESTATIC} PUBLIC ${OPTS}) 
               target_include_directories(${EXNAMESTATIC} PUBLIC "${EAGLE_INCLUDE_DIR}" "${A5_INCLUDE_DIRECTORY}")
               target_link_directories(${EXNAMESTATIC} PUBLIC "${ALLEGRO5_LIB_DIR}")
               target_link_libraries(${EXNAMESTATIC} PUBLIC EagleA5 Eagle ${ALLEGRO_LINK_LIBS} ${WINSTATICLIBS})
               install(TARGETS ${EXNAMESTATIC} RUNTIME DESTINATION bin)
            endif (EAGLE_STATIC_LIBRARY_BUILD)

         endif (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")

      endforeach()
   endif (WANT_ALLEGRO5)
   install(FILES "Verdana.ttf" DESTINATION bin)
   install(DIRECTORY "Data/Images/" DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Data/Images")
   install(DIRECTORY "Data/Fonts/" DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Data/Fonts")
endif (WANT_EXAMPLES)

install(DIRECTORY include DESTINATION "${CMAKE_INSTALL_PREFIX}")

#[[
#cd /d E:\Downloads\Programming\Tools\CMake\cmake-3.14.0-rc4\build\Utilities\Sphinx && E:\Programs\Python3pt6\Scripts\sphinx-build.exe -c #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx -d #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/doctrees -b htmlhelp #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/Help #E:/Downloads/Programming/Tools/CMake/cmake-3.14.0-rc4/build/Utilities/Sphinx/htmlhelp > build-htmlhelp.log
#]]
