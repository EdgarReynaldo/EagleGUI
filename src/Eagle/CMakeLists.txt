
## This is the main CMakeLists.txt for building the Eagle librasry, not including the backends

set(srcdirs
   "." 
   "Gui" 
   "Gui/Button" 
   "Gui/Decorator" 
   "Gui/Factory" 
   "Gui/Layout" 
   "Gui/Scripting" 
   "Gui/Text"
   )
   
set(sourcefiles)
   
foreach(dir ${srcdirs})
   file(GLOB cppsrcs "${EAGLE_ROOT_DIR}/src/${dir}/*.cpp")
   file(GLOB csrcs   "${EAGLE_ROOT_DIR}/src/${dir}/*.c")
   list(APPEND sourcefiles ${cppsrcs} ${csrcs})
endforeach(dir)

set(incdirs
   "."
   "Eagle"
   "Eagle/Gui"
   "Eagle/Gui/Button"
   "Eagle/Gui/Decorators"
   "Eagle/Gui/Factory"
   "Eagle/Gui/Layout"
   "Eagle/Gui/Scripting"
   "Eagle/Gui/Text"
   )

set(includefiles)

foreach(dir ${incdirs})
   file (GLOB headershpp "${EAGLE_INCLUDE_DIR}/include/${dir}/*.hpp")
   file (GLOB headersh   "${EAGLE_INCLUDE_DIR}/include/${dir}/*.h")
   list (APPEND includefiles ${headershpp} ${headersh})
endforeach(dir)
   
message(${sourcefiles})
message(${includefiles})
