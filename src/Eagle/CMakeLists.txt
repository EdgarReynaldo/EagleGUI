
# src/Eagle/

## This is the main CMakeLists.txt for building the Eagle librasry, not including the backends

set(srcdirs
   "Eagle/" 
   "Eagle/Gui/" 
   "Eagle/Gui/Button/" 
   "Eagle/Gui/Decorator/" 
   "Eagle/Gui/Factory/" 
   "Eagle/Gui/Layout/" 
   "Eagle/Gui/Menu/" 
   "Eagle/Gui/Scripting/" 
   "Eagle/Gui/Text/"
   )
   
set(sourcefiles)
   
foreach(dir ${srcdirs})
   file(GLOB cppsrcs "${EAGLE_ROOT_DIR}/src/${dir}*.cpp")
   file(GLOB csrcs   "${EAGLE_ROOT_DIR}/src/${dir}*.c")
   list(APPEND sourcefiles ${cppsrcs} ${csrcs})
endforeach(dir)

set(incdirs
   "include/"
   "include/Eagle/"
   "include/Eagle/Gui/"
   "include/Eagle/Gui/Button/"
   "include/Eagle/Gui/Decorators/"
   "include/Eagle/Gui/Factory/"
   "include/Eagle/Gui/Layout/"
   "include/Eagle/Gui/Scripting/"
   "include/Eagle/Gui/Text/"
   )

set(includefiles)

foreach(dir ${incdirs})
   file (GLOB headershpp "${EAGLE_ROOT_DIR}/${dir}*.hpp")
   file (GLOB headersh   "${EAGLE_ROOT_DIR}/${dir}*.h")
   list (APPEND includefiles ${headershpp} ${headersh})
endforeach(dir)

set(EAGLE_SOURCE_FILES ${sourcefiles})
set(EAGLE_HEADER_FILES ${includefiles})

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
target_compile_options(EagleObjects PRIVATE "-pedantic" "-std=c++11" "-fPIC" "-Wall" "-Wextra" "-Wshadow")



if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   target_compile_definitions(EagleObjects PRIVATE DEBUG)
   target_compile_definitions(EagleObjects PRIVATE EAGLE_LOG)
   target_compile_definitions(EagleObjects PRIVATE EAGLE_ASSERT_EXCEPTION)
   target_compile_definitions(EagleObjects PRIVATE EAGLE_DEBUG_MUTEX_LOCKS)
   target_compile_options(EagleObjects PRIVATE "-g" "-fno-inline-functions" "-O0")
#   target_compile_options(EagleObjects PUBLIC "-fsanitize=address")
endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")

if (${EAGLE_SHARED_LIBRARY_BUILD})
   if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      add_library(EagleDebugShared SHARED $<TARGET_OBJECTS:EagleObjects>)
#####      target_link_libraries(EagleDebugShared PUBLIC asan)
      install(TARGETS EagleDebugShared LIBRARY DESTINATION bin)
   else()
      add_library(EagleShared SHARED $<TARGET_OBJECTS:EagleObjects>)
      install(TARGETS EagleShared LIBRARY DESTINATION bin)
   endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
endif (${EAGLE_SHARED_LIBRARY_BUILD})

if (${EAGLE_STATIC_LIBRARY_BUILD})
   if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      add_library(EagleDebug STATIC $<TARGET_OBJECTS:EagleObjects>)
#####      target_link_libraries(EagleDebugShared PUBLIC asan)
      install(TARGETS EagleDebug ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
   else()
      add_library(Eagle STATIC $<TARGET_OBJECTS:EagleObjects>)
      install(TARGETS Eagle ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
   endif ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
endif (${EAGLE_STATIC_LIBRARY_BUILD})


