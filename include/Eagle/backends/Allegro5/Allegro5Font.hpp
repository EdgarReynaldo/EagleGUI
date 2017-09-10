


#ifndef Allegro5Font_HPP
#define Allegro5Font_HPP


#include "Eagle/Font.hpp"

#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"



class Allegro5Font : public EagleFont {

private :
   ALLEGRO_FONT* allegro_font;

public :
   Allegro5Font(ALLEGRO_FONT* font , std::string objname = "Nemo");
   Allegro5Font(std::string file , int size , int flags , std::string objname = "Nemo" , IMAGE_TYPE type = VIDEO_IMAGE);
   
   
   bool Load(std::string file , int size , int flags , IMAGE_TYPE type = VIDEO_IMAGE);
   void Free();
   
   bool Valid();

   virtual int Width(const char* str);
   virtual int Height();
   
   ALLEGRO_FONT* AllegroFont() {return allegro_font;}

};

#endif // Allegro5Font_HPP
