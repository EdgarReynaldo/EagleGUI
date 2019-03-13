
# src/Eagle/backends\Allegro5

## This is the main CMakeLists.txt for building the Eagle Allegro 5 backend librasry

set(srcdirs
   "backends/Allegro5/"
   )
   
set(sourcefiles)
   
foreach(dir ${srcdirs})
   file(GLOB cppsrcs "${EAGLE_ROOT_DIR}/src/${dir}*.cpp")
   file(GLOB csrcs   "${EAGLE_ROOT_DIR}/src/${dir}*.c")
   list(APPEND sourcefiles ${cppsrcs} ${csrcs})
endforeach(dir)

set(incdirs
   "Eagle/backends/Allegro5/"
   )

set(includefiles)

list (APPEND includefiles "${EAGLE_INCLUDE_DIR}/Eagle/backends/Allegro5backend.hpp")

foreach(dir ${incdirs})
   file (GLOB headershpp "${EAGLE_INCLUDE_DIR}/${dir}*.hpp")
   file (GLOB headersh   "${EAGLE_INCLUDE_DIR}/${dir}*.h")
   list (APPEND includefiles ${headershpp} ${headersh})
endforeach(dir)

set(EAGLE_A5_SOURCE_FILES ${sourcefiles})
set(EAGLE_A5_HEADER_FILES ${includefiles})

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

   
add_library(EagleA5Objects OBJECT ${EAGLE_A5_SOURCE_FILES} ${EAGLE_A5_HEADER_FILES})

add_dependencies(EagleA5Objects EagleObjects)

target_include_directories(EagleA5Objects PRIVATE ${EAGLE_INCLUDE_DIR})
target_include_directories(EagleA5Objects PRIVATE "${EAGLE_INCLUDE_DIR}/backends/Allegro5/")
target_include_directories(EagleA5Objects PRIVATE "${ALLEGRO5_INCLUDEDIR}")

target_compile_options(EagleA5Objects PRIVATE "-std=c++11")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   target_compile_definitions(EagleA5Objects PRIVATE DEBUG)
   target_compile_definitions(EagleA5Objects PRIVATE EAGLE_LOG)
   target_compile_definitions(EagleA5Objects PRIVATE EAGLE_ASSERT_EXCEPTION)
endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

if (${EAGLE_SHARED_LIBRARY_BUILD})
   add_library(EagleA5Shared SHARED $<TARGET_OBJECTS:EagleA5Objects>)
   add_dependencies(EagleA5Shared EagleShared)
   target_link_libraries(EagleA5Shared EagleShared)
   if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      target_link_libraries(EagleA5Shared "${ALLEGRO5_DEBUGLIB}")
   elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
      target_link_libraries(EagleA5Shared "${ALLEGRO5_LIB}")
   endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   target_link_libraries(EagleA5Shared "${PHYSFS_LIB}")
endif (${EAGLE_SHARED_LIBRARY_BUILD})

if (${EAGLE_STATIC_LIBRARY_BUILD})
   add_library(EagleA5Static STATIC $<TARGET_OBJECTS:EagleA5Objects>)
endif (${EAGLE_STATIC_LIBRARY_BUILD})
