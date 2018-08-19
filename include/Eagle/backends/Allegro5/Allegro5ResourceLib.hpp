



#ifndef Allegro5ResourceLib_HPP
#define Allegro5ResourceLib_HPP


#include "Eagle/ResourceLib.hpp"


#include <set>
#include <vector>


/**
enum RESOURCE_TYPE {
   RT_UNKNOWN   = 0,
   RT_IMAGE     = 1,/// bmp png jpg tga
   RT_FONT      = 2,/// ttf bmp
   RT_AUDIO     = 3,/// ogg wav
   RT_VIDEO     = 4,/// ogv
   RT_ARCHIVE   = 5,/// zip, 7z
   RT_BINFILE   = 6,/// dat bin
   RT_TEXTFILE  = 7,/// txt
   NUM_RT_TYPES = 8
};
*/


class Allegro5ResourceLibrary : public ResourceLibrary {

protected :
   
   static Allegro5ResourceLibrary* me;
   
public :

   Allegro5ResourceLibrary();



   virtual std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt);

   
   static Allegro5ResourceLibrary* Instance() {return me;}
};



#endif // Allegro5ResourceLib_HPP

