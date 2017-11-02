


#include "Eagle/backends/Allegro5/Allegro5Font.hpp"
#include "Eagle/StringWork.hpp"


Allegro5Font::Allegro5Font(ALLEGRO_FONT* font , std::string objname) :
   EagleFont("Allegro5Font" , objname),
   allegro_font(font)
{}



Allegro5Font::Allegro5Font(std::string file , int size , int flags , std::string objname , IMAGE_TYPE type) :
      EagleFont("Allegro5Font" , objname),
      allegro_font(0)
{
   Load(file , size , flags , type);
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


   bool ttf = false;
   if (file.find_last_of(".ttf") != std::string::npos) {
      // found a .ttf font TODO fix this is a bit simple check not quite robust enough
      ttf = true;
   }
   // string::npos
   /// size_type find_last_of( const char* str, size_type index = npos );
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
   }
   else {
      EagleError() << "Failed to load font " << file << " from disk." << std::endl;
   }
   return allegro_font;
}



void Allegro5Font::Free() {
   if (allegro_font) {
      al_destroy_font(allegro_font);
      allegro_font = 0;
   }
}



bool Allegro5Font::Valid() {
   return allegro_font;
}



int Allegro5Font::Width(std::string str) {
   if (!allegro_font) {return 0;}
   std::vector<std::string> lines = SplitByNewLines(str);
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
   int nlines = CountNewLines(str);
   int th = nlines*Height() + (nlines-1)*ls;
   return th;
}




