



#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP



#include "Eagle/Font.hpp"
#include "Eagle/MemFile.hpp"


#include <map>
#include <string>




/**! @fn SetDefaultFontPath @fn SetDefaultFontSize @fn SetDefaultFontFlags
 *   @brief Set the default font path, size, or flags
 *
 *   The default font path, size, and flags will be used whenever a new display is created.
 *   You can retrieve the default font from a display @ref EagleGraphicsContext with the @ref DefaultFont function
 */


const char* default_font_path = "Data/Fonts/Verdana.ttf";
int default_font_size = -16;
int default_font_flags = LOAD_FONT_MONOCHROME;

class FontManager {
   
   EagleGraphicsContext* owner;

   std::map<std::string , MemFile> memfiles;
   std::map<std::string , EagleFont*> fonts;

   EagleFont* builtinfont;
   EagleFont* defaultfont;
   


   virtual EagleFont* CreateBuiltinFont()=0;
   virtual EagleFont* CreateDefaultFont()=0;
   
   
public :
   
   virtual EagleFont* LoadFontPath(std::string basename , std::string basepath , int loading_flags , int font_style)=0;

   void SetDefaultFontPath(std::string path);///< Set the default font path
   void SetDefaultFontPointSize(int pointsize);///< Set the default font size
   void SetDefaultFontFlags(int flags);///< Set the default font flags

   std::string DefaultFontPath();///< Get the default font path
   int DefaultFontPoint();///< Get the default font point size
   int DefaultFontFlags();///< Get the default font flags
};





#endif // FONT_MANAGER_HPP





#ifndef ALLEGRO5_FONT_MANAGER_HPP
#define ALLEGRO5_FONT_MANAGER_HPP


#include "Eagle/FontManager.hpp"


class Allegro5FontManager : public FontManager {
   
   virtual EagleFont* CreateBuiltinFont();
   virtual EagleFont* CreateDefaultFont();
EagleFont* Allegro5FontManager::CreateBuiltinFont() {
   EagleFont* fnt = new Allegro5Font(al_create_builtin_font() , "BuiltinFont");
   allegro_file = 0;
}
EagleFont* Allegro5FontManager::CreateDefaultFont() {
   EagleFont* fnt = new Allegro5Font("Allegro5Font" , "DefaultFont");
}
   
};


#endif // ALLEGRO5_FONT_MANAGER_HPP
