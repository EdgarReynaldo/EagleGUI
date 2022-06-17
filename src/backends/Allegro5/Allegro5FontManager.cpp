



#include "Eagle/backends/Allegro5/Allegro5FontManager.hpp"




Allegro5FontManager(EagleGraphicsContext* window) :
      FontManager(window)
{
   CreateBuiltinFont();
   CreateDefaultFont();
}



EagleFont* Allegro5FontManager::CreateBuiltinFont() {
   return LoadFontPath("BUILTIN" , 8 , FONT_NORMAL , VIDEO_IMAGE);
}



EagleFont* Allegro5FontManager::CreateDefaultFont() {
   return LoadFontPath("DEFAULT" , default_font_size , default_font_flags , VIDEO_IMAGE);
}



EagleFont* Allegro5FontManager::LoadFontPath(std::string path , int size , int loading_flags , IMAGE_TYPE type) {

   FNTIT fntit = fonts.find(path + std::string(size) + TranslateFontFlags(loading_flags));/// Fonts are stored in the map as FONT.EXT### with the point size at the right of the string
   if (fntit != fonts.end()) {
      return fntit->second;/// Font already in font map
   }
   
   MemFile* mfile;
   MFMIT mfmit = memfiles->find(path);/// Memfiles are tracked by base file path
   if (mfmit != memfiles.end()) {
      /// This font is stored in memory - create an allegro file and then use al_load_ttf_font_f
      mfile = mfmit->second;
      
   }
   
   int flag = ALLEGRO_MEMORY_BITMAP;
   switch (type) {
      case SYSTEM_IMAGE : 
         type = VIDEO_IMAGE;
         /// Don't break here
      case VIDEO_IMAGE :
         flag = ALLEGRO_VIDEO_BITMAP;
      default : 
         al_set_new_bitmap_flags(al_get_new_bitmap_flags() & flag);
   };
   
   EagleFont* font = new Allegro5Font("Allegro5Font" , path + std::string(size));
   font->owner = owner;
   font->fontman = this;
   if (path.compare("BUILTIN") == 0) {/// BUILTIN font is never in the map
      font->srcfile = "BUILTIN";
      font = al_create_builtin_font();
      font->height = al_get_line_height(font);
      font->flags = FONT_MONOCHROME;
      font->srcflags = FONT_BUILTIN;
      fonts.insert(std::pair<std::string , EagleFont*>("BUILTIN" , font));
      return font;
   }
   else if (mfile) {
      font->LoadFromMemory(mfile , size , flag , type);
   }
   else (path.compare(0 , 7 , "DEFAULT") == 0) {
      font->srcfile = default_font_path + std::string(default_font_size) + TranslateFontFlags(flags);
      font->height = default_font_size;
      font->flags = default_font_flags;
      font->Load(default_font_path.c_str() , default_font_size , default_font_flags , VIDEO_IMAGE);
   }
   else {
         
   }
   
   
   
}


   
   
   
   
   
