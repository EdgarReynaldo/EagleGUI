
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


#include "Eagle/Gui/Layout/LayoutRectangle.hpp"


LayoutRectangle::LayoutRectangle(float x , float y , float w , float h) :
   fx(x),
   fy(y),
   fw(w),
   fh(h) 
{}



LayoutRectangle::LayoutRectangle(Rectangle outer_area , Rectangle layout_area) :
   fx(0.0),
   fy(0.0),
   fw(1.0),
   fh(1.0)
{
   EAGLE_ASSERT(outer_area.W());
   EAGLE_ASSERT(outer_area.H());
   
   float x = outer_area.X();
   float y = outer_area.Y();
   float w = outer_area.W();
   float h = outer_area.H();
   
   float lx = layout_area.X();
   float ly = layout_area.Y();
   float lw = layout_area.W();
   float lh = layout_area.H();
   
   fx = (lx - x)/w;
   fy = (ly - y)/h;
   fw = lw/w;
   fh = lh/h;
}



/// Global functions



Rectangle LayoutArea(Rectangle outer_area , LayoutRectangle layout_area) {
   float x = outer_area.X();
   float y = outer_area.Y();
   float w = outer_area.W();
   float h = outer_area.H();
   
   // apply percentages from layout_area
   float nx = x + layout_area.fx*w;
   float ny = y + layout_area.fy*h;
   float nw = w*layout_area.fw;
   float nh = h*layout_area.fh;
   
   // transform to integer boundaries, shrink to prevent overlap
   int nxpos = (int)(ceil(nx));
   int nypos = (int)(ceil(ny));
   int nwid = (int)(floor(nw));
   int nht = (int)(floor(nh));
   
   return Rectangle(nxpos , nypos , nwid , nht);
}



