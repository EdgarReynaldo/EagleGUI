



#include "Eagle/FontManager.hpp"






FontManager::FontManager(EagleGraphicsContext* window) : 
      owner(window),
      memfiles(),
      fonts(),
      builtinfont(0),
      defaultfont(0),
      default_font_path("Data/Fonts/Verdana.ttf"),
      default_font_size(-16),
      default_font_flags(LOAD_FONT_MONOCHROME)
{}



EagleFont* FontManager::GetFont(std::string path , int size) {
   if (path.compare(0 , 7 , "BUILTIN" , std::string::npos) == 0) {
      return CreateBuiltinFont();
   }
   else if (path.compare(0 , 7 , "DEFAULT" , std::string npos) {
      return CreateDefaultFont();
   }
   else {
      return LoadFontPath(path , size , 
   }
}



void FontManager::SetDefaultFontPath(std::string path) {
   default_font_path = path;
}



void FontManager::SetDefaultFontSize(int pointsize) {
   default_font_size = pointsize;
}



void FontManager::SetDefaultFontFlags(int flags) {
   default_font_flags = flags;
}



EagleFont* FontManager::BuiltinFont() {
   return builtinfont;
}



EagleFont* FontManager::DefaultFont() {
   return defaultfont;
}



std::string FontManager::DefaultFontPath() {
   return default_font_path;
}



int FontManager::DefaultFontSize() {
   return default_font_size;
}



int FontManager::DefaultFontFlags() {
   return default_font_flags;
}



