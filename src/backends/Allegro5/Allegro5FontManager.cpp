
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5FontManager.cpp
 * @brief Implementation of the font manager using Allegro 5
 *
 */




#include "Eagle/backends/Allegro5/Allegro5FontManager.hpp"
#include "Eagle/backends/Allegro5/Allegro5Font.hpp"
#include "Eagle/backends/Allegro5/Allegro5MemFile.hpp"
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
   const std::string fullfontname = std::to_string(size) + path + TranslateFontFlags(fontflags);
   
   FNTIT fntit = fonts.find(fullfontname);///< Fonts are stored in the map as ###Font.path$FLAGS$ with the point size at the left of the string
   if (fntit != fonts.end()) {
      return fntit->second;/// Font already in font map
   }
   
   Allegro5MemFile* mfile = 0;
   MFMIT mfmit = memfiles.find(path);/// Memfiles are tracked by base file path
   if (mfmit != memfiles.end()) {
      /// This font is stored in memory - create an allegro file and then use al_load_ttf_font_f
      mfile = dynamic_cast<Allegro5MemFile*>(mfmit->second);
   }
   else {// mfmit == memfiles.end()
      if (path.compare("BUILTIN") != 0) {
         mfile = new Allegro5MemFile(path);
         bool loaded = mfile->ReadFileIntoMemory();
         if (!loaded) {
            EagleWarn() << "Failed to read font into memory from path '" << path << "'" << std::endl;
            delete mfile;
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
      fonts.insert(std::pair<std::string , EagleFont*>(fullfontname , font));
      reversefontmap.insert(std::pair<EagleFont* , std::string>(font , mfile->Path()));
   }
   return font;
}


   
   
   
   
   
