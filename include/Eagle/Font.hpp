
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleFont_HPP
#define EagleFont_HPP


#include <string>


#include "Eagle/Image.hpp"
#include "Eagle/Object.hpp"

enum FONT_LOADING_FLAGS {
   LOAD_FONT_NORMAL = 0,
   LOAD_FONT_NO_KERNING = 1 << 0,
   LOAD_FONT_MONOCHROME = 1 << 1,
   LOAD_FONT_NOAUTOHINT = 1 << 2,
   LOAD_FONT_NOPREMULTALPHA = 1 << 3// applies to bitmap fonts
};



class EagleFont : public EagleObject {
   
protected :
   int height;
   std::string srcfile;

public :   
   EagleFont();
   EagleFont(std::string name);
   virtual ~EagleFont() {}

   virtual bool Load(std::string file , int size , int flags , IMAGE_TYPE type)=0;
   virtual void Free()=0;
   
   virtual bool Valid()=0;
   
   virtual int Width(const char* str)=0;
   virtual int Height()=0;

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

};


EagleFont* GetFont(std::string font_name);


void SetDefaultFontPath(std::string path);
void SetDefaultFontSize(int size);
void SetDefaultFontFlags(int flags);

std::string DefaultFontPath();
int DefaultFontSize();
int DefaultFontFlags();



#endif // EagleFont_HPP

