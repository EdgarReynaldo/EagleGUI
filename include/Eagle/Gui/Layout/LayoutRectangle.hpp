
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef LayoutRectangle_HPP
#define LayoutRectangle_HPP



#include "Eagle/Area.hpp"

#include <iostream>

/*
   Think of this class as a percentage rectangle ie. it stores the relative position and scale
   so an area can shrink or expand dynamically by just changing the outer area
 */
class LayoutRectangle {

protected :
   
   friend Rectangle LayoutArea(Rectangle outer_area , LayoutRectangle layout_area);
   
   float fx,fy,fw,fh;
   
public :

   LayoutRectangle();

   LayoutRectangle(float x , float y , float w , float h);
   LayoutRectangle(Rectangle outer_area , Rectangle layout_area);

   void SetLayoutRectangle(float x , float y , float w , float h);
   void SetLayoutRectangle(Rectangle outer_area , Rectangle layout_area);

   float LX() const {return fx;}
   float TY() const {return fy;}
   float RX() const {return fx + fw;}
   float BY() const {return fy + fh;}
   float CX() const {return fx + fw/2.0f;}
   float CY() const {return fy + fh/2.0f;}
   float W()  const {return fw;}
   float H()  const {return fh;}
};

Rectangle LayoutArea(Rectangle outer_area , LayoutRectangle layout_area);

std::ostream& operator<<(std::ostream& os , const LayoutRectangle& lr);

#endif // LayoutRectangle_HPP


