

#ifndef ALLEGRO5_FONT_MANAGER_HPP
#define ALLEGRO5_FONT_MANAGER_HPP


#include "Eagle/FontManager.hpp"


class Allegro5FontManager : public FontManager {

//   EagleGraphicsContext* owner;

//   std::map<std::string , MemFile> memfiles;
//   std::map<std::string , EagleFont*> fonts;

//   typedef std::map<std::string , Memfile> MFMAP;
//   typedef std::map<std::string , Memfile>::iterator MFMIT;
//   typedef std::map<std::string , EagleFont*> FNTMAP;
//   typedef std::map<std::string , EagleFont*>::iterator FNTIT;

//   EagleFont* builtinfont;
//   EagleFont* defaultfont;

public :
   Allegro5FontManager(EagleGraphicsContext* window) explicit;
   
   virtual EagleFont* CreateBuiltinFont();
   virtual EagleFont* CreateDefaultFont();

   virtual EagleFont* LoadFontPath(std::string path , int size , int loading_flags , IMAGE_TYPE type = VIDEO_IMAGE);
};




#endif // ALLEGRO5_FONT_MANAGER_HPP





