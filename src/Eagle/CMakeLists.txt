


set(srcdirs
   "Eagle" 
   "Eagle/Gui" 
   "Eagle/Gui/Button" 
   "Eagle/Gui/Decorator" 
   "Eagle/Gui/Factory" 
   "Eagle/Gui/Layout" 
   "Eagle/Gui/LayoutOld" 
   "Eagle/Gui/Scripting" 
   "Eagle/Gui/Text"
   )

file(GLOB cppsrcs "${CMAKE_SOURCE_DIR}/src/${srcdirs}/*.cpp")
file(GLOB csrcs   "${CMAKE_SOURCE_DIR}/src/${srcdirs}/*.c")

set (sourcefiles ${cppsrcs} ${csrcs} )

set(incdirs
   "Eagle"
   "Eagle/Gui"
   "Eagle/backends/Allegro5"
   "Eagle/Gui/Button"
   "Eagle/Gui/Decorators"
   "Eagle/Gui/Factory"
   "Eagle/Gui/Layout"
   "Eagle/Gui/Scripting"
   "Eagle/Gui/Text"
   )

file (GLOB headershpp "${CMAKE_SOURCE_DIR}/include/${incdirs}/*.hpp")
file (GLOB headersh   "${CMAKE_SOURCE_DIR}/include/${incdirs}/*.h")

set (headerfiles ${headershpp} ${headersh})
