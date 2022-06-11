



#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP



#include "Eagle/Font.hpp"
#include "Eagle/MemFile.hpp"


#include <map>
#include <string>


class FontManager {
   
   std::map<std::string , MemFile> memfiles;
   std::map<std::string , EagleFont*> fonts;
   std::map<std::string , EagleFontFamily*> fontfamilys;
   
   
   EagleFont* builtinfont;
   EagleFont* defaultfont;
   
   virtual EagleFont* CreateBuiltinFont()=0;
   virtual EagleFont* LoadFontPath(std::string basename , std::string basepath , int loading_flags , int font_style)=0;
   virtual EagleFontFamily* LoadFontFamily(std::string basename , std::string basepath , int load_flags)=0;
};





#endif // FONT_MANAGER_HPP





#ifndef ALLEGRO5_FONT_MANAGER_HPP
#define ALLEGRO5_FONT_MANAGER_HPP


#include "Eagle/FontManager.hpp"


class Allegro5FontManager : public FontManager {
   
   EagleFont* CreateBuiltinFont();
   EagleFont* CreateBuiltinFont() {
      
   }
   
};


#endif // ALLEGRO5_FONT_MANAGER_HPP
