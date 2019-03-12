
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

set(EAGLE_A5_SOURCE_FILES ${sourcefiles} PARENT_SCOPE)
set(EAGLE_A5_HEADER_FILES ${includefiles} PARENT_SCOPE)
