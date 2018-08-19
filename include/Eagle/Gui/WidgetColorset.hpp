
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



#ifndef EagleGuiWidgetColorset_HPP
#define EagleGuiWidgetColorset_HPP



#include "Eagle/Color.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/SharedMemory.hpp"

#include <map>


/*
/// From darkest (SDCOL shadow color) to lightest (HLCOL highlight color), with a few extras
enum WIDGETCOLOR {
   SDCOL   = 0,
   BGCOL   = 1,
   BMGCOL   = 2,
   MGCOL = 3,
   FMGCOL   = 4,
   FGCOL   = 5,
   HLCOL   = 6,
   TXTCOL  = 7
};
// 255/7 = 30 + 45/7 = 30 + 6 + 3/7

#define NUMCOLS 8
//*/

//*
/// From darkest (SDCOL shadow color) to lightest (HLCOL highlight color), with a few extras
enum WIDGETCOLOR {
   SDCOL   = 0,
   BGCOL   = 1,
   MGCOL   = 2,
   FGCOL   = 3,
   HLCOL   = 4,
   TXTCOL  = 5,
   HVRCOL  = 6,
   PADCOL  = 7,
   BORDCOL = 8,
   MARGCOL = 9
};
#define EAGLE_NUMCOLORS 10
//*/

std::string WidgetColorName(WIDGETCOLOR wc);
WIDGETCOLOR WidgetColorFromName(std::string name);



class WidgetColorset {

protected :
   EagleColor wcolorset[EAGLE_NUMCOLORS];

public :

   WidgetColorset();
   WidgetColorset(EagleColor colorset[EAGLE_NUMCOLORS]);

   WidgetColorset& operator=(const EagleColor colorset[EAGLE_NUMCOLORS]);
   

   EagleColor& operator[] (const WIDGETCOLOR& wc) {return wcolorset[wc];}
   const EagleColor& operator[] (const WIDGETCOLOR& wc) const {return wcolorset[wc];}

   EagleColor& operator[] (int wc) {return wcolorset[wc];}
   const EagleColor& operator[] (int wc) const {return wcolorset[wc];}
   
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   friend std::ostream& operator<<(std::ostream& os , const EagleColor& ecol);
};

extern EagleColor default_eagle_color_array[EAGLE_NUMCOLORS];



/// Defined colors are clear, white, black, light_gray, medium_gray, dark_gray,
/// red, orange, yellow, lime_green , green, neon_green, cyan, sky_blue, blue, purple, magenta, and fuchsia


class RegisteredColor {

public :
   RegisteredColor(std::string name , float red , float green , float blue, float alpha);
   RegisteredColor(std::string name , int red , int green , int blue, int alpha);

};



class ColorRegistry {
   
   std::map<std::string , EagleColor> named_colors;
   std::map<std::string , SHAREDOBJECT<WidgetColorset> > named_colorsets;
   
public :
   
   ColorRegistry() : named_colors() , named_colorsets() {}
   
   static ColorRegistry* GlobalColorRegistry();
   
   /// EagleColors
   bool HasColor(std::string name);

   EagleColor& GetColorByName(std::string name);

   void RegisterColor(std::string name , const EagleColor& color);
   
   /// WidgetColorsets
   bool HasColorset(std::string name);
   
   SHAREDOBJECT<WidgetColorset> GetColorsetByName(std::string name);
   SHAREDOBJECT<WidgetColorset> GetDefaultColorset() {return GetColorsetByName("Default");}
   
   void RegisterColorset(std::string name , const WidgetColorset& wc);
   
};

bool HasColor(std::string name);
EagleColor GetColorByName(std::string name);
void RegisterColor(std::string name , const EagleColor& color);

bool HasColorset(std::string name);
SHAREDOBJECT<WidgetColorset> GetColorsetByName(std::string name);
void RegisterColorset(std::string name , const WidgetColorset& wc);



#endif // EagleGuiWidgetColorset_HPP

