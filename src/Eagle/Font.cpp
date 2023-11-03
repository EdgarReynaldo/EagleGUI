
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
#include "Eagle/FontManager.hpp"
#include "Eagle/GraphicsContext.hpp"





#include <sstream>



std::string TranslateFontFlags(int fontflags) {
   if (fontflags == FONT_NORMAL) {
      return "FONT_NORMAL";
   }
   int flagcount = 0;
   std::stringstream ss;
   if (fontflags & FONT_NO_KERNING) {
      ss << "NO_KERNING";
      flagcount++;
   }
   if (fontflags & FONT_MONOCHROME) {
      if (flagcount) {
         ss << " | ";
      }
      ss << "MONOCHROME";
      flagcount++;
   }
   if (fontflags & FONT_NOAUTOHINT) {
      if (flagcount) {
         ss << " | ";
      }
      ss << "NOAUTOHINT";
      flagcount++;
   }
   if (fontflags & FONT_NOPREMULTALPHA) {
      if (flagcount) {
         ss << " | ";
      }
      ss << "NOPREMULTALPHA";
   }
   return ss.str();
}



int UntranslateFontFlags(std::string flagstr) {
   int flags = 0;
   if (flagstr.find_first_of("FONT_NORMAL" , 0) != std::string::npos) {
      return 0;
   }
   if (flagstr.find_first_of("FONT_NO_KERNING" , 0) != std::string::npos) {
      flags |= FONT_NO_KERNING;
   }
   if (flagstr.find_first_of("FONT_MONOCHROME" , 0) != std::string::npos) {
      flags |= FONT_MONOCHROME;
   }
   if (flagstr.find_first_of("FONT_NOAUTOHINT" , 0) != std::string::npos) {
      flags |= FONT_NOAUTOHINT;
   }
   if (flagstr.find_first_of("FONT_NOPREMULTALPHA" , 0) != std::string::npos) {
      flags |= FONT_NOPREMULTALPHA;
   }
   return flags;
}



/// -------------------------------       EagleFont      ----------------------------------------------



bool EagleFont::LoadFromArgs(std::vector<std::string> args) {
   IMAGE_TYPE it = VIDEO_IMAGE;
   std::string flags;
   int ptsize;
   if (args.size() < 1) {
      EagleError() << "No args passed to LoadFromArgs." << std::endl;
      return false;
   }
   ptsize = STOI(args[0]);
   if (args.size() > 1) {
      flags = args[1];
      if (args.size() > 2) {
         if (args[2].compare("MEMORY")==0) {
            it = MEMORY_IMAGE;
         }
      }
   }
   return Load(filepath.Path() , ptsize , flags , it);
}



EagleFont::EagleFont(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      ResourceBase(RT_FONT),
      owner(0),
      fontman(0),
      height(0),
      srcfile(""),
      fontflags(FONT_NORMAL),
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


