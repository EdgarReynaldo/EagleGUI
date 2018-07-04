
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
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui/WidgetColorset.hpp"

#include "Eagle/StringWork.hpp"





std::string WidgetColorName(WIDGETCOLOR wc) {
   static const char* names[EAGLE_NUMCOLORS] = {
      "SDCOL",
      "BGCOL",
      "MGCOL",
      "FGCOL",
      "HLCOL",
      "TXTCOL",
      "HVRCOL"
   };
   return names[wc];
}



WIDGETCOLOR WidgetColorFromName(std::string name) {
   static const std::string names[EAGLE_NUMCOLORS] = {
      "SDCOL",
      "BGCOL",
      "MGCOL",
      "FGCOL",
      "HLCOL",
      "TXTCOL",
      "HVRCOL"
   };
   for (int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {
      if (name.compare(names[i]) == 0) {
         return (WIDGETCOLOR)i;
      }
   }
   throw EagleException(StringPrintF("Color '%s' is not a recognized WIDGETCOLOR.\n" , name));
   return -1;
}



EagleColor default_eagle_color_array[EAGLE_NUMCOLORS] = {
   EagleColor(0   , 0   , 0   , 255),
   EagleColor(64  , 64  , 64  , 255),
   EagleColor(128 , 128 , 128 , 255),
   EagleColor(192 , 192 , 192 , 255),
   EagleColor(255 , 255 , 255 , 255),
   EagleColor(255 , 255 , 255 , 255),
   EagleColor(255 , 255 , 255 , 255)
};



WidgetColorset::WidgetColorset() :
      wcolorset()
{
   *this = default_eagle_color_array;
}



WidgetColorset::WidgetColorset(EagleColor colorset[EAGLE_NUMCOLORS]) :
   wcolorset()
{
   *this = colorset;
}



WidgetColorset& WidgetColorset::operator=(const EagleColor colorset[EAGLE_NUMCOLORS]) {
   for (int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {wcolorset[i] = colorset[i];}
   return *this;
}




std::ostream& WidgetColorset::DescribeTo(std::ostream& os , Indenter indent) const {
   const EagleColor* c = wcolorset;
   os << indent << "WidgetColorset : " << std::endl;
   ++indent;
   os << indent << "SDCOL = [" << c[SDCOL] << "] BGCOL = [" << c[BGCOL] << "] MGCOL = [" << c[MGCOL] << "]" << std::endl;
   os << indent << "FGCOL = [" << c[FGCOL] << "] HLCOL = [" << c[HLCOL] << "] TXTCOL = [" << c[TXTCOL] << "] HVRCOL = [" << c[HVRCOL] << "]" << std::endl;
   --indent;
   return os;
}



std::ostream& operator<<(std::ostream& os , const WidgetColorset& wc) {
    wc.DescribeTo(os);
    return os;
}



/// ------------------------   RegisteredColor    ----------------------------------



RegisteredColor::RegisteredColor(std::string name , float red , float green , float blue, float alpha) {
   ColorRegistry::GlobalColorRegistry()->RegisterColor(name , EagleColor(red,green,blue,alpha));
}



RegisteredColor::RegisteredColor(std::string name , int red , int green , int blue, int alpha) {
   ColorRegistry::GlobalColorRegistry()->RegisterColor(name , EagleColor(red,green,blue,alpha));
}
   


/// -------------------------     ColorRegistry     ---------------------------------



ColorRegistry* ColorRegistry::GlobalColorRegistry() {
   static ColorRegistry reg;
   static int init = 1;
   if (init) {
      reg.RegisterColorset("Default" , WidgetColorset());
      init = 0;
   }
   return &reg;
}



#define REGISTER_COLOR(name , red , green , blue , alpha) \
   RegisteredColor registered_color_##name(#name , red , green , blue , alpha);
   
REGISTER_COLOR(clear       , 1.0f  , 1.0f  , 1.0f  , 0.0f);
REGISTER_COLOR(white       , 1.0f  , 1.0f  , 1.0f  , 1.0f);
REGISTER_COLOR(black       , 0.0f  , 0.0f  , 0.0f  , 1.0f);
REGISTER_COLOR(light_gray  , 0.75f , 0.75f , 0.75f , 1.0f);
REGISTER_COLOR(medium_gray , 0.5f  , 0.5f  , 0.5f  , 1.0f);
REGISTER_COLOR(dark_gray   , 0.25f , 0.25f , 0.25f , 1.0f);

REGISTER_COLOR(red         , 1.0f  , 0.0f  , 0.0f  , 1.0f);
REGISTER_COLOR(orange      , 1.0f  , 0.5f  , 0.0f  , 1.0f);
REGISTER_COLOR(yellow      , 1.0f  , 1.0f  , 0.0f  , 1.0f);
REGISTER_COLOR(lime_green  , 0.5f  , 1.0f  , 0.0f  , 1.0f);
REGISTER_COLOR(green       , 0.0f  , 1.0f  , 0.0f  , 1.0f);
REGISTER_COLOR(neon_green  , 0.0f  , 1.0f  , 0.5f  , 1.0f);
REGISTER_COLOR(cyan        , 0.0f  , 1.0f  , 1.0f  , 1.0f);
REGISTER_COLOR(sky_blue    , 0.0f  , 0.5f  , 1.0f  , 1.0f);
REGISTER_COLOR(blue        , 0.0f  , 0.0f  , 1.0f  , 1.0f);
REGISTER_COLOR(purple      , 0.5f  , 0.0f  , 1.0f  , 1.0f);
REGISTER_COLOR(magenta     , 1.0f  , 0.0f  , 1.0f  , 1.0f);
REGISTER_COLOR(fuchsia     , 1.0f  , 0.0f  , 0.5f  , 1.0f);


bool ColorRegistry::HasColor(std::string name) {
   return named_colors.find(name) != named_colors.end();
}



EagleColor& ColorRegistry::GetColorByName(std::string name) {
   std::map<std::string , EagleColor>::iterator it = named_colors.find(name);
   if(it == named_colors.end()) {
      throw EagleException(StringPrintF("Color '%s' not found in color registry.\n" , name.c_str()));
   }
   return it->second;
}



void ColorRegistry::RegisterColor(std::string name , const EagleColor& color) {
   named_colors[name] = color;
}



bool ColorRegistry::HasColorset(std::string name) {
   return named_colorsets.find(name) != named_colorsets.end();
}



std::shared_ptr<WidgetColorset> ColorRegistry::GetColorsetByName(std::string name) {
   std::map<std::string , WidgetColorset>::iterator it = named_colorsets.find(name);
   if (it == named_colorsets.end()) {
      throw EagleException(StringPrintF("WidgetColorset '%s' not found in color registry.\n" , name.c_str()));
   }
   return it->second;
}



void ColorRegistry::RegisterColorset(std::string name , const WidgetColorset& wc) {
   named_colorsets[name] = std::shared_ptr<WidgetColorset>(new WidgetColorset(wc));
}



/// --------------------------     Globals     ---------------------------------



bool HasColor(std::string name) {
   return ColorRegistry::GlobalColorRegistry()->HasColor(name);
}



EagleColor GetColorByName(std::string name) {
   return ColorRegistry::GlobalColorRegistry()->GetColorByName(name);
}



void RegisterColor(std::string name , const EagleColor& color) {
   ColorRegistry::GlobalColorRegistry()->RegisterColor(name , color);
}



bool HasColorset(std::string name) {
   return ColorRegistry::GlobalColorRegistry()->HasColorset(name);
}



std::shared_ptr<WidgetColorset> GetColorsetByName(std::string name) {
   return ColorRegistry::GlobalColorRegistry()->GetColorsetByName(name);
}



void RegisterColorset(std::string name , const WidgetColorset& wc) {
   return ColorRegistry::GlobalColorRegistry()->RegisterColorset(name , wc);
}






