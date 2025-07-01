
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
 *    Copyright 2009-2025+ by Edgar Reynaldo
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
      "FCSCOL",  /// EXTENDED
      "HVRCOL",  /// EXTENDED
      "PADCOL",  /// EXTENDED
      "BORDCOL", /// EXTENDED
      "MARGCOL"  /// EXTENDED
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
      "FCSCOL",
      "HVRCOL",
      "PADCOL",
      "BORDCOL",
      "MARGCOL"
   };
   for (int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {
      if (name.compare(names[i]) == 0) {
         return (WIDGETCOLOR)i;
      }
   }
   throw EagleException(StringPrintF("Color '%s' is not a recognized WIDGETCOLOR.\n" , name.c_str()));
   return SDCOL;
}



EagleColor default_eagle_color_array[EAGLE_NUMCOLORS] = {
   EagleColor(0   , 0   , 0   , 255),///< SDCOL
   EagleColor(64  , 64  , 64  , 255),///< BGCOL
   EagleColor(128 , 128 , 128 , 255),///< MGCOL
   EagleColor(192 , 192 , 192 , 255),///< FGCOL
   EagleColor(255 , 255 , 255 , 255),///< HLCOL
   EagleColor(255 , 255 , 255 , 255),///< TXTCOL
   EagleColor(255 , 255 , 255 , 255),///< FCSCOL
   EagleColor(255 , 255 , 255 , 255),///< HVRCOL
   EagleColor(255 , 255 , 255 , 255),///< PADCOL
   EagleColor(0   , 255 , 0   , 255),///< BORDCOL
   EagleColor(0   , 0   , 0   , 0  ) ///< MARGCOL
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



WidgetColorset::WidgetColorset(const WidgetColorset& wcols) :
   wcolorset()
{
   for (unsigned int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {
      wcolorset[i] = wcols[i];
   }
}



WidgetColorset& WidgetColorset::operator=(const WidgetColorset& wcols) {
   for (unsigned int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {
      wcolorset[i] = wcols.wcolorset[i];
   }
   return *this;
}



WidgetColorset& WidgetColorset::operator=(const EagleColor colorset[EAGLE_NUMCOLORS]) {
   for (int i = 0 ; i < EAGLE_NUMCOLORS ; ++i) {wcolorset[i] = colorset[i];}
   return *this;
}




std::ostream& WidgetColorset::DescribeTo(std::ostream& os , Indenter indent) const {
   const EagleColor* c = wcolorset;
   os << indent << "WidgetColorset : " << std::endl;
   ++indent;
   os << indent << "SD[" << c[SDCOL] << "] BG[" << c[BGCOL] << "] MG[" << c[MGCOL] << "] FG[" << c[FGCOL] << "] HL[" << c[HLCOL] << "]" << std::endl;
   os << indent << "FCS[" << c[FCSCOL] << "] TXT[" << c[TXTCOL] << "] HVR[" << c[HVRCOL] << "] PAD[" << c[PADCOL] << "] BORD[" << c[BORDCOL] << "] MARG[" << c[MARGCOL] << "]" << std::endl;
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
   RegisteredColor RC_##name(#name , red , green , blue , alpha);
   
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



SHAREDOBJECT<WidgetColorset> ColorRegistry::GetColorsetByName(std::string name) {
   if (!name.size()) {
      name = "Default";
   }
   std::map<std::string , SHAREDOBJECT<WidgetColorset> >::iterator it = named_colorsets.find(name);
   if (it == named_colorsets.end()) {
      throw EagleException(StringPrintF("WidgetColorset '%s' not found in color registry.\n" , name.c_str()));
   }
   return it->second;
}



void ColorRegistry::RegisterColorset(std::string name , const WidgetColorset& wc) {
   named_colorsets[name] = HeapObject(new WidgetColorset(wc));
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



SHAREDOBJECT<WidgetColorset> GetColorsetByName(std::string name) {
   return ColorRegistry::GlobalColorRegistry()->GetColorsetByName(name);
}



void RegisterColorset(std::string name , const WidgetColorset& wc) {
   return ColorRegistry::GlobalColorRegistry()->RegisterColorset(name , wc);
}






