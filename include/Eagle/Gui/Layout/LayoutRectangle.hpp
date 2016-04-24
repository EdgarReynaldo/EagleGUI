
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

#ifndef LayoutRectangle_HPP
#define LayoutRectangle_HPP



#include "Eagle/Area.hpp"



/*
   Think of this class as a percentage rectangle ie. it stores the relative position and scale
   so an area can shrink or expand dynamically by just changing the outer area
 */
class LayoutRectangle {

private :
   
public :
   
   LayoutRectangle(float x , float y , float w , float h);
   LayoutRectangle(Rectangle outer_area , Rectangle layout_area);
   
   float fx,fy,fw,fh;
   
   float LX() {return fx;}
   float TY() {return fy;}
   float RX() {return fx + fw;}
   float BY() {return fy + fh;}
   
};

Rectangle LayoutArea(Rectangle outer_area , LayoutRectangle layout_area);



#endif // LayoutRectangle_HPP


