


#include "Eagle/backends/Allegro5/Allegro5Font.hpp"
#include "Eagle/StringWork.hpp"


Allegro5Font::Allegro5Font(ALLEGRO_FONT* font , std::string objname) :
   EagleFont("Allegro5Font" , objname),
   allegro_font(font),
   allegro_file(0)
{}



Allegro5Font::Allegro5Font(std::string file , int size , int flags , std::string objname , IMAGE_TYPE type) :
      EagleFont("Allegro5Font" , objname),
      allegro_font(0),
      allegro_file(0)
{
   Load(file , size , flags , type);
}



bool Allegro5Font::LoadFromMemory(MemFile* memfile , int pt , int flags , IMAGE_TYPE type) {
   Free();
   
   if (!memfile) {
      EAGLE_ASSERT(memfile);
      return false;
   }
   if (!memfile->Begin()) {
      EAGLE_ASSERT(memfile->Begin());
      return false;
   }
   
   allegro_file = al_open_memfile((void*)memfile->Begin() , (int64_t)memfile->Size() , "r");
   if (!allegro_file) {
      return false;
   }
   
   if (type == SYSTEM_IMAGE) {type = VIDEO_IMAGE;}
   
   if (type == VIDEO_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   }
   else if (type == MEMORY_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   }
   memtype = type;
   
   allegro_font = al_load_ttf_font_f(allegro_file , src.c_str() , pt , flags);
   if (!allegro_font) {
      EAGLE_ASSERT(allegro_font);
      return false;
   }
   srcfile = memfile->Path();
   fontflags = flags;
   srcflags = FONT_MEMFILE;
   styleflags = FONT_ROMAN;
}



bool Allegro5Font::Load(std::string file , int size , int flags , IMAGE_TYPE type) {
   
   Free();

   if (type == SYSTEM_IMAGE) {type = VIDEO_IMAGE;}
   
   if (type == VIDEO_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   }
   else if (type == MEMORY_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   }
   memtype = type;

   /// We support loading ttfs, bmp, and pcx bitmap fonts
   bool ttf = false;
   bool bmp = false;
   bool pcx = false;
   srcfile = file;
   /// index of extension minus 1
   size_t idx = srcfile.find_last_of(".")
   if (idx == std::string::npos) {
      EagleError() << "Extension not found." << std::endl;
      return false;
   }
   std::string ext = srcfile.substr(idx + 1 , std::string::npos);
   if (ext.compare("ttf") == 0) {
      ttf = true;
   }
   else if (ext.compare("BMP") == 0 || ext.compare("bmp") == 0) {
      bmp = true;
   }
   else if (ext.compare("pcx") == 0) {
      pcx = true;
   }
   
   
   // string::npos
   /// size_type find_last_of( const char* str, size_type index = npos );
   srcflags = FONT_SRCFILE;
   if (ttf) {
      allegro_font = al_load_ttf_font(file.c_str() , size , flags);
   }
   else {
      allegro_font = al_load_font(file.c_str() , size , flags);
   }
   if (allegro_font) {
      EagleInfo() << StringPrintF("Loaded font %s from disk." , file.c_str()) << std::endl;
      height = size;
      srcfile = file;
      fontflags = flags;
   }
   else {
      EagleError() << "Failed to load font " << file << " from disk." << std::endl;
   }
   return allegro_font;
}



bool Adopt(ALLEGRO_FONT* f) {
   if (!f) {
      EAGLE_ASSERT(f);
      return false;
   }
   Free();
   allegro_font = f;
   height = al_get_font_line_height(f);
   srcfile = "";
   fontflags = FONT_NORMAL;
   styleflags = FONT_ROMAN;
   srcflags = FONT_UNKNOWN;
   memtype = SYSTEM_IMAGE;
   return Valid();
}



void Allegro5Font::Free() {
   if (allegro_font) {
      al_destroy_font(allegro_font);
      allegro_font = 0;
   }
   if (allegro_file) {
      al_fclose(allegro_file);
      allegro_file = 0;
   }
}



bool Allegro5Font::Valid() {
   return allegro_font;
}



int Allegro5Font::Width(std::string str) {
   if (!allegro_font) {return 0;}
   std::vector<std::string> lines = SplitByNewLinesChomp(str);
   int maxw = 0;
   for (unsigned int i = 0 ; i < lines.size() ; ++i) {
      int w = al_get_text_width(allegro_font , lines[i].c_str());
      if (w > maxw) {maxw = w;}
   }
   return maxw;
}



int Allegro5Font::Height() {
   if (!allegro_font) {return 0;}
   int fh = al_get_font_line_height(allegro_font);
   return fh;
}



int Allegro5Font::Height(std::string str , int ls) {
   int nlines = CountLines(str);
   int th = nlines*Height() + (nlines-1)*ls;
   return th;
}



int Allegro5Font::VHeight(std::string str , int letter_spacing) {
   int h = 0;
   ALLEGRO_FONT* f = AllegroFont();
   if (!f) {
      return -1;
   }
   if (!str.size()) {
      return 0;
   }
   if (letter_spacing < 0) {
      letter_spacing = al_get_font_line_height(f)/8;
   }
   ALLEGRO_GLYPH g;
   for (unsigned int i = 0 ; i < str.size() ; ++i) {
      memset(&g , 0 , sizeof(g));
      if (al_get_glyph(f , 0 , str[i] , &g)) {
         h += g.h;
         if ((int)i < (int)str.size() - 1) {
            h += letter_spacing;
         }
      }
   }
   return h;
}



int Allegro5Font::VWidth(std::string str , int line_spacing) {
   
   ALLEGRO_FONT* f = AllegroFont();

   if (!f) {return -1;}

   if (!str.size()) {return 0;}

   std::vector<std::string> lines = SplitByNewLinesChomp(str);

   if (!lines.size()) {return 0;}
   
   int w = 0;
   int maxw = 0;
   
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      maxw = 0;
      
      ALLEGRO_GLYPH g;
      for (unsigned int k = 0 ; k < lines[i].size() ; ++k) {
         memset(&g , 0 , sizeof(g));
         if (al_get_glyph(f , 0 , lines[i][k] , &g)) {
            if (g.w > maxw) {
               maxw = g.w;
            }
         }
      }
      w += maxw;
      if (i < ((int)lines.size() - 1)) {
         w += line_spacing;
      }
   }
   return w;
}





