cmake_minimum_required(VERSION 3.13) 

project(EXAMPLES)

file(GLOB_RECURSE EXAMPLESRC CONFIGURE_DEPENDS "*.cpp")
set(EXAMPLESRC ${EXAMPLESRC} PARENT_SCOPE)


foreach(EX IN LISTS EXAMPLESRC)
   get_filename_component(SRC ${EX} NAME)
   message(STATUS ${SRC} ${EX} SRC EX)
   add_executable(${SRC} WIN32 ${EX})
   if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_compile_options(${SRC} PUBLIC "-g")
   endif()
   target_include_directories(${SRC} PUBLIC "../include" "/usr/include")
endforeach()

