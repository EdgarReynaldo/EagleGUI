
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Font.hpp"

#include "Eagle/StringWork.hpp"



EagleFont* default_font = 0;


EagleFont::EagleFont() :
      EagleObject(StringPrintF("EagleFont object at %p" , this)),
      height(0),
      srcfile("") 
{
   
}



EagleFont::EagleFont(std::string name) :
      EagleObject(name),
      height(0),
      srcfile("") 
{
   
}



std::ostream& EagleFont::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("EagleFont object %s at %p. Height = %d. Source file = \"%s\"",
                      GetName().c_str() , this , height , srcfile.c_str());
   return os;
}



EagleFont* GetFont(std::string font_name) {
   EagleFont* font = dynamic_cast<EagleFont*>(GetFirstObjectByName(font_name));
   return font;
}


