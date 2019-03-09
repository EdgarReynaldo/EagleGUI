
# src/Eagle/

## This is the main CMakeLists.txt for building the Eagle librasry, not including the backends

set(srcdirs
   "Eagle/" 
   "Eagle/Gui/" 
   "Eagle/Gui/Button/" 
   "Eagle/Gui/Decorator/" 
   "Eagle/Gui/Factory/" 
   "Eagle/Gui/Layout/" 
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




