
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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#ifndef NinePatch_HPP
#define NinePatch_HPP

class EagleGraphicsContext;
class EagleImage;

#include "Eagle/Gui/WidgetArea.hpp"

class NinePatch {
public :
   EagleImage* imgs[3][3];
   NinePatch();
   NinePatch(const NinePatch& np);
   NinePatch& operator=(const NinePatch& np);
   
   EagleImage** operator[](int yindex);
   EagleImage* const* operator[](int index) const;
};

NinePatch MakeNinePatch(EagleGraphicsContext* win , EagleImage* src_img , WidgetArea nparea);



#endif // NinePatch_HPP





