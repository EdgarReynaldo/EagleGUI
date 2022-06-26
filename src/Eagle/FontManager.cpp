



#include "Eagle/FontManager.hpp"
#include "Eagle/GraphicsContext.hpp"





FontManager::FontManager(EagleGraphicsContext* window) : 
      owner(window),
      memfiles(),
      fonts(),
      reversefontmap(),
      builtinfont(0),
      defaultfont(0),
      default_font_path("Data/Fonts/Verdana.ttf"),
      default_font_size(-16),
      default_font_flags(FONT_MONOCHROME)
{}



EagleFont* FontManager::GetFont(std::string path , int size , int fontflags , IMAGE_TYPE memflags) {
   if (path.compare(0 , 7 , "BUILTIN" , std::string::npos) == 0) {
      return CreateBuiltinFont();
   }
   else if (path.compare(0 , 7 , "DEFAULT" , std::string::npos) == 0) {
      default_font_size = size;
      default_font_flags = fontflags;
      return CreateDefaultFont();
   }
   else {
      return LoadFontPath(path , size , fontflags , memflags);
   }
   /// Not reached
   return 0;
}




void FontManager::FreeFont(EagleFont* font) {
   std::string fpath;
   RFMIT it = reversefontmap.find(font);
   if (it != reversefontmap.end()) {
      FNTIT it2 = fonts.find(fpath);
      if (it2 != fonts.end()) {
         font->Free();
         delete font;
         reversefontmap.erase(it);
         fonts.erase(it2);
      }
   }
}



void FontManager::FreeFontFile(std::string file) {
   MFMIT it = memfiles.find(file);
   if (it != memfiles.end()) {
      MemFile* mf = it->second;
      std::string path = mf->Path();
      for (FNTIT it2 = fonts.begin() ; it2 != fonts.end() ; ++it2) {
         std::string fontname = it2->first;
         if (fontname.compare(0 , path.size() , path , std::string::npos) == 0) {
            EagleFont* f = it2->second;
            f->Free();
            delete f;
         }
      }
      mf->Clear();
      delete mf;
      memfiles.erase(it);
   }
}



void FontManager::FreeAll() {
   /// Free and destroy all fonts
   if (builtinfont) {
      builtinfont->Free();
      delete builtinfont;
      builtinfont = 0;
   }
   /// defaultfont is stored in fonts
   for (FNTIT it = fonts.begin() ; it != fonts.end() ; ++it) {
      EagleFont* font = it->second;
      font->Free();
      delete font;
   }
   fonts.clear();
   reversefontmap.clear();
   /// Free and destroy all memfiles. Must destroy memfiles after fonts.
   for (MFMIT it = memfiles.begin() ; it != memfiles.end() ; ++it) {
      MemFile* mf = it->second;
      mf->Clear();
      delete mf;
   }
   memfiles.clear();
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



