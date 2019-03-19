
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Font.hpp"
#include "Eagle/ObjectRegistry.hpp"
#include "Eagle/StringWork.hpp"



/*
EagleFont::EagleFont() :
      EagleObject("EagleFont" , "Nemo"),
      height(0),
      srcfile("") 
{
   
}



EagleFont::EagleFont(std::string name) :
      EagleObject("EagleFont" , name),
      height(0),
      srcfile("") 
{
   
}
*/


EagleFont::EagleFont(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      height(0),
      srcfile("") 
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








