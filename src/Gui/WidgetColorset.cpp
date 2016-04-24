
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




EagleColor default_eagle_color_array[EAGLE_NUMCOLORS] = { EagleColor(0 , 0 , 0 , 255),
                                                  EagleColor(64 , 64 , 64 , 255),
                                                  EagleColor(128 , 128 , 128 , 255),
                                                  EagleColor(192 , 192 , 192 , 255),
                                                  EagleColor(255 , 255 , 255 , 255),
                                                  EagleColor(255 , 255 , 255 , 255)   };
//




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



WidgetColorset::WidgetColorset(const WidgetColorset& rhs) {
   *this = rhs.wcolorset;
}



