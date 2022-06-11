
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Font.hpp"
#include "Eagle/ObjectRegistry.hpp"
#include "Eagle/StringWork.hpp"

#include <sstream>



std::string TranslateFontFlags(FONT_LOADING_FLAGS flags) {
   if (flags == LOAD_FONT_NORMAL) {
      return "FONT_NORMAL";
   }
   int flagcount = 0;
   std::stringstream ss;
   if (flags & FONT_NO_KERNING) {
      ss << "NO_KERNING";
      flagcount++;
   }
   if (flags & FONT_MONOCHROME) {
      if (flagcount) {
         ss << " | ";
      }
      ss << "MONOCHROME";
      flagcount++;
   }
   if (flags & FONT_NOAUTOHINT) {
      if (flagcount) {
         ss << " | ";
      }
      ss << "NOAUTOHINT";
      flagcount++;
   }
   if (flags & FONT_NOPREMULTALPHA) {
      if (flagcount) {
         ss << " | ";
      }
      ss << "NOPREMULTALPHA";
   }
   return ss.str();
}



EagleFont::EagleFont(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      owner(0),
      fontman(0),
      height(0),
      srcfile(""),
      fontflags(FONT_NORMAL),
      styleflags(FONT_ROMAN),
      srcflags(FONT_UNKNOWN),
      memtype(0)
{
   
}



std::ostream& EagleFont::DescribeTo(std::ostream& os , Indenter indent) const {
   EagleObjectInfo i = InfoById(GetEagleId());
   os << indent << StringPrintF("%s. Height = %d. Source file = \"%s\"",
                      i.FullName() , height , srcfile.c_str());
   return os;
}



EagleFont* GetFont(std::string font_name) {
   EagleFont* font = dynamic_cast<EagleFont*>(GetFirstObjectByName(font_name));
   return font;
}



std::string default_font_path = "data/fonts/verdana.ttf";



int default_font_size = -20;



int default_font_flags = 0;



void SetDefaultFontPath(std::string path) {
   default_font_path = path;
}



void SetDefaultFontSize(int size) {
   default_font_size = size;
}



void SetDefaultFontFlags(int flags) {
   default_font_flags = flags;
}



std::string DefaultFontPath() {
   return default_font_path;
}



int DefaultFontSize() {
   return default_font_size;
}



int DefaultFontFlags() {
   return default_font_flags;
}




EagleFontFamily::~EagleFontFamily() {
   Free();
}



void EagleFontFamily::Free() {
   for (unsigned int i = 0 ; i < 4 ; ++i) {
      if (fonts[i]) {
         fonts[i]->owner->Free(fonts[i]);
      }
   }
}



bool EagleFontFamily::LoadFontFamily(std::string basename , std::string dir , int point) {
   Free();
   fonts[FONT_ROMAN]       = Load(basename ,                dir , point , FONT_ROMAN);
   fonts[FONT_BOLD]        = Load(basename + "Bold" ,       dir , point , FONT_BOLD);
   fonts[FONT_ITALIC]      = Load(basename + "Italic" ,     dir , point , FONT_ITALIC);
   fonts[FONT_BOLD_ITALIC] = Load(basename + "BoldItalic" , dir , point , FONT_BOLD_ITALIC);
   
   return fonts[FONT_ROMAN] && fonts[FONT_BOLD] && fonts[FONT_ITALIC] && fonts[FONT_BOLD_ITALIC];

}



bool LoadFont(std::string basename , std::string dir , int point , FONT_STYLE_FLAGS style) {
   if (fonts[style]) {fonts[style]->owner->Free(fonts[style])}
   fonts[style] = 0;
   fonts[style] = Load(basename , dir , point , style);
   return fonts[style];
}



EagleFont* EagleFontFamily::GetFontType(FONT_STYLE_FLAGS kind) {
   return fonts[kind];
}








