
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



#include "Eagle/Gui/WidgetColorset.hpp"

#include "Eagle/StringWork.hpp"


EagleColor default_eagle_color_array[EAGLE_NUMCOLORS] = {
   EagleColor(0   , 0   , 0   , 255),
   EagleColor(64  , 64  , 64  , 255),
   EagleColor(128 , 128 , 128 , 255),
   EagleColor(192 , 192 , 192 , 255),
   EagleColor(255 , 255 , 255 , 255),
   EagleColor(255 , 255 , 255 , 255)
};
//




WidgetColorset::WidgetColorset() :
      EagleObject(StringPrintF("WidgetColorset object at %p" , this)),
      wcolorset()
{
   *this = default_eagle_color_array;
}



WidgetColorset::WidgetColorset(EagleColor colorset[EAGLE_NUMCOLORS]) :
   EagleObject(StringPrintF("WidgetColorset object at %p" , this)),
   wcolorset()
{
   *this = colorset;
}



WidgetColorset::WidgetColorset(const WidgetColorset& rhs) :
   EagleObject(StringPrintF("WidgetColorset object at %p" , this)),
   wcolorset()
{
   *this = rhs.wcolorset;
}



WidgetColorset& WidgetColorset::operator=(const EagleColor colorset[EAGLE_NUMCOLORS]) {
   for (int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {wcolorset[i] = colorset[i];}
   return *this;
}




std::ostream& WidgetColorset::DescribeTo(std::ostream& os , Indenter indent) const {
   const EagleColor* c = wcolorset;
   os << indent << StringPrintF("WidgetColorset object %s at %p : " , GetName().c_str() , this) << std::endl;
   ++indent;
   os << indent << "SDCOL = [" << c[SDCOL] << "] BGCOL = [" << c[BGCOL] << "] MGCOL = [" << c[MGCOL] << "]" << std::endl;
   os << indent << "FGCOL = [" << c[FGCOL] << "] HLCOL = [" << c[HLCOL] << "] TXTCOL = [" << c[TXTCOL] << "]" << std::endl;
   --indent;
   return os;
}



/// ------------------------   RegisteredColor    ----------------------------------



RegisteredColor::RegisteredColor(std::string name , float red , float green , float blue, float alpha) {
   color_registry.RegisterColor(name , EagleColor(red,green,blue,alpha));
}



RegisteredColor::RegisteredColor(std::string name , int red , int green , int blue, int alpha) {
   color_registry.RegisterColor(name , EagleColor(red,green,blue,alpha));
}
   

#define REGISTER_COLOR(name , red , green , blue , alpha) \
   RegisteredColor registered_color_##name(name , red , green , blue , alpha);
   
REGISTER_COLOR("clear"       , 1.0  , 1.0  , 1.0  , 0.0);
REGISTER_COLOR("white"       , 1.0  , 1.0  , 1.0  , 1.0);
REGISTER_COLOR("black"       , 0.0  , 0.0  , 0.0  , 1.0);
REGISTER_COLOR("light-gray"  , 0.75 , 0.75 , 0.75 , 1.0);
REGISTER_COLOR("medium-gray" , 0.5  , 0.5  , 0.5  , 1.0);
REGISTER_COLOR("dark-gray"   , 0.25 , 0.25 , 0.25 , 1.0);

REGISTER_COLOR("red"         , 1.0  , 0.0  , 0.0  , 1.0);
REGISTER_COLOR("orange"      , 1.0  , 0.5  , 0.0  , 1.0);
REGISTER_COLOR("yellow"      , 1.0  , 1.0  , 0.0  , 1.0);
REGISTER_COLOR("lime-green"  , 0.5  , 1.0  , 0.0  , 1.0);
REGISTER_COLOR("green"       , 0.0  , 1.0  , 0.0  , 1.0);
REGISTER_COLOR("neon-green"  , 0.0  , 1.0  , 0.5  , 1.0);
REGISTER_COLOR("cyan"        , 0.0  , 1.0  , 1.0  , 1.0);
REGISTER_COLOR("sky-blue"    , 0.0  , 0.5  , 1.0  , 1.0);
REGISTER_COLOR("blue"        , 0.0  , 0.0  , 1.0  , 1.0);
REGISTER_COLOR("purple"      , 0.5  , 0.0  , 1.0  , 1.0);
REGISTER_COLOR("magenta"     , 1.0  , 0.0  , 1.0  , 1.0);
REGISTER_COLOR("fuchsia"     , 1.0  , 0.0  , 0.5  , 1.0);


/// -------------------------     ColorRegistry     ---------------------------------



ColorRegistry color_registry;



bool ColorRegistry::HasColor(std::string name) {
   return named_colors.find(name) != named_colors.end();
}



EagleColor ColorRegistry::GetColorByName(std::string name) {
   std::map<std::string , EagleColor>::iterator it = named_colors.find(name);
   if (it != named_colors.end()) {
      return it->second;
   }
   return EagleColor();
}



void ColorRegistry::RegisterColor(std::string name , EagleColor color) {
   named_colors[name] = color;
}



/// --------------------------     Globals     ---------------------------------



bool HasColor(std::string name) {
   return color_registry.HasColor(name);
}



EagleColor GetColorByName(std::string name) {
   return color_registry.GetColorByName(name);
}



void RegisterColor(std::string name , EagleColor color) {
   color_registry.RegisterColor(name , color);
}







