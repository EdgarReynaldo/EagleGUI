



#include "Eagle/backends/Allegro5/Allegro5FontManager.hpp"
#include "Eagle/backends/Allegro5/Allegro5Font.hpp"
#include "allegro5/allegro.h"

#include <string>


Allegro5FontManager::Allegro5FontManager(EagleGraphicsContext* window) :
      FontManager(window)
{
   builtinfont = CreateBuiltinFont();
   defaultfont = CreateDefaultFont();
}



Allegro5FontManager::~Allegro5FontManager() {
   FreeAll();
}



EagleFont* Allegro5FontManager::CreateBuiltinFont() {
   return LoadFontPath("BUILTIN" , 8 , FONT_NORMAL , VIDEO_IMAGE);
}



EagleFont* Allegro5FontManager::CreateDefaultFont() {
   return LoadFontPath("DEFAULT" , default_font_size , default_font_flags , VIDEO_IMAGE);
}



EagleFont* Allegro5FontManager::LoadFontPath(std::string path , int size , int fontflags , IMAGE_TYPE type) {

   if (path.compare("DEFAULT") == 0) {
      path = default_font_path;
   }

   FNTIT fntit = fonts.find(path + std::to_string(size) + TranslateFontFlags(fontflags));/// Fonts are stored in the map as FONT.EXT### with the point size at the right of the string
   if (fntit != fonts.end()) {
      return fntit->second;/// Font already in font map
   }
   
   MemFile* mfile = 0;
   MFMIT mfmit = memfiles.find(path);/// Memfiles are tracked by base file path
   if (mfmit != memfiles.end()) {
      /// This font is stored in memory - create an allegro file and then use al_load_ttf_font_f
      mfile = mfmit->second;
   }
   else {// mfmit == memfiles.end()
      if (path.compare("BUILTIN") != 0) {
         mfile = new MemFile(path);
         bool loaded = mfile->ReadFileIntoMemory();
         if (!loaded) {
            EAGLE_ASSERT(loaded);
            return 0;
         }
         memfiles.insert(std::pair<std::string , MemFile*>(path , mfile));
      }
   }
   
   if (type == SYSTEM_IMAGE) {type = VIDEO_IMAGE;}
   if (type == MEMORY_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   }
   else if (type == VIDEO_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   }
   
   
   Allegro5Font* font = new Allegro5Font("Allegro5Font" , path);
   font->owner = owner;
   font->fontman = this;
   if (path.compare("BUILTIN") == 0) {/// BUILTIN font is never in the memfile map
      font->SetShortName("BUILTIN");
      font->srcfile = "BUILTIN";
      font->allegro_font = al_create_builtin_font();
      font->height = al_get_font_line_height(font->allegro_font);
      font->fontflags = FONT_MONOCHROME;
      font->srcflags = FONT_BUILTIN;
      fonts.insert(std::pair<std::string , EagleFont*>("BUILTIN" , font));
      reversefontmap.insert(std::pair<EagleFont* , std::string>(font , "BUILTIN"));
   }
   else if (mfile) {
      bool success = font->LoadFromMemory(mfile , size , fontflags , type);
      if (!success) {
         EAGLE_ASSERT(success);
         return 0;
      }
      std::string fullfontname = mfile->Path() + std::to_string(size) + TranslateFontFlags(fontflags);
      fonts.insert(std::pair<std::string , EagleFont*>(fullfontname , font));
      reversefontmap.insert(std::pair<EagleFont* , std::string>(font , mfile->Path()));
   }
   return font;
}


   
   
   
   
   
