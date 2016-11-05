
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
   TXTCOL  = 5
};
#define EAGLE_NUMCOLORS 6
//*/



class WidgetColorset : public EagleObject {

public :

   EagleColor wcolorset[EAGLE_NUMCOLORS];
   


   WidgetColorset();
   WidgetColorset(EagleColor colorset[EAGLE_NUMCOLORS]);
   WidgetColorset(const WidgetColorset& rhs);

   WidgetColorset& operator=(const EagleColor colorset[EAGLE_NUMCOLORS]);
   

   EagleColor& operator[] (const WIDGETCOLOR& wc) {return wcolorset[wc];}
   const EagleColor& operator[] (const WIDGETCOLOR& wc) const {return wcolorset[wc];}
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
};

extern EagleColor default_eagle_color_array[EAGLE_NUMCOLORS];



/// Defined colors are clear, white, black, light gray, medium gray, dark gray,
/// red, orange, yellow, lime-green , green, neon-green, cyan, sky-blue, blue, purple, magenta, and fuchsia


class RegisteredColor {

public :
   RegisteredColor(std::string name , float red , float green , float blue, float alpha);
   RegisteredColor(std::string name , int red , int green , int blue, int alpha);

}



REGISTER_COLOR(white , 1.0 , 1.0 , 1.0 , 1.0);
   
class ColorRegistry {
   
   std::map<std::string , EagleColor> named_colors;
   
public :
   
   EagleColor GetColorByName(std::string name);
   
   void RegisterColor(std::string name , EagleColor color);
   
}


extern ColorRegistry color_registry;


#endif // EagleGuiWidgetColorset_HPP

