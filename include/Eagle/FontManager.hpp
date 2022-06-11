



#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP



#include "Eagle/Font.hpp"
#include "Eagle/MemFile.hpp"


#include <map>
#include <string>


//   EagleFont*  default_font;
//   std::string default_font_path;
//   int         default_font_size;
//   int         default_font_flags;


/**! @fn SetDefaultFontPath @fn SetDefaultFontSize @fn SetDefaultFontFlags
 *   @brief Set the default font path, size, or flags
 *
 *   The default font path, size, and flags will be used whenever a new display is created.
 *   You can retrieve the default font from a display @ref EagleGraphicsContext with the @ref DefaultFont function
 */

void SetDefaultFontPath(std::string path);///< Set the default font path
void SetDefaultFontPointSize(int pointsize);///< Set the default font size
void SetDefaultFontFlags(int flags);///< Set the default font flags

std::string DefaultFontPath();///< Get the default font path
int DefaultFontPoint();///< Get the default font point size
int DefaultFontFlags();///< Get the default font flags


class FontManager {
   
   EagleGraphicsContext* owner;
   std::map<std::string , MemFile> memfiles;
   std::map<std::string , EagleFont*> fonts;
   
   virtual EagleFont* CreateBuiltinFont()=0;
   virtual EagleFont* CreateDefaultFont()=0;
   
   EagleFont* builtinfont;
   EagleFont* defaultfont;
   
public :
   
   virtual EagleFont* LoadFontPath(std::string basename , std::string basepath , int loading_flags , int font_style)=0;
};





#endif // FONT_MANAGER_HPP





#ifndef ALLEGRO5_FONT_MANAGER_HPP
#define ALLEGRO5_FONT_MANAGER_HPP


#include "Eagle/FontManager.hpp"


class Allegro5FontManager : public FontManager {
   
   virtual EagleFont* CreateBuiltinFont();
   virtual EagleFont* CreateDefaultFont();
EagleFont* Allegro5FontManager::CreateBuiltinFont() {
   
}
EagleFont* Allegro5FontManager::CreateDefaultFont() {
   
}
   
};


#endif // ALLEGRO5_FONT_MANAGER_HPP
