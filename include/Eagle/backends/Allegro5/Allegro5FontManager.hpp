

#ifndef ALLEGRO5_FONT_MANAGER_HPP
#define ALLEGRO5_FONT_MANAGER_HPP


#include "Eagle/FontManager.hpp"


class Allegro5FontManager : public FontManager {

public :
   explicit Allegro5FontManager(EagleGraphicsContext* window);
   ~Allegro5FontManager();
   
   virtual EagleFont* CreateBuiltinFont();
   virtual EagleFont* CreateDefaultFont();

   virtual EagleFont* LoadFontPath(std::string path , int size , int loading_flags , IMAGE_TYPE type = VIDEO_IMAGE);
};




#endif // ALLEGRO5_FONT_MANAGER_HPP





