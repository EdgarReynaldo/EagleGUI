
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file LayoutRectangle.hpp
 * @brief A simple interface to define areas relative to a parent rectangle
 */

#ifndef LayoutRectangle_HPP
#define LayoutRectangle_HPP



#include "Eagle/Area.hpp"



#include <iostream>




/**! @class LayoutRectangle
 *   @brief A class to help make relative rectangular areas
 *
 *   Think of this class as a percentage rectangle ie. it stores the relative position and scale
 *   so an area can shrink or expand dynamically by just changing the outer area
 */

class LayoutRectangle {

protected :
   
   friend Rectangle LayoutArea(Rectangle outer_area , LayoutRectangle layout_area);///< Helper function
   
   float fx,fy,fw,fh;///< Coordinates stored as relative floating point percentages [0.0,1.0]
   
public :

   LayoutRectangle();///< Default empty constructor

   ///< Construct a new layout rectangle with the specified relative attributes
   ///< @sa SetLayoutRectangle(float,float,float,float)
   LayoutRectangle(float x , float y , float w , float h);

   ///< Define a new layout rectangle from an outer and inner rectangle
   ///< @sa SetLayoutRectangle(Rectangle,Rectangle)
   LayoutRectangle(Rectangle outer_area , Rectangle layout_area);

   ///< Construct a new layout rectangle with the specified relative attributes
   void SetLayoutRectangle(float x , float y , float w , float h);

   ///< Define a new layout rectangle from an outer and inner rectangle
   void SetLayoutRectangle(Rectangle outer_area , Rectangle layout_area);

   float LX() const {return fx;}///< Get the left x position in percent
   float TY() const {return fy;}///< Get the top y position in percent
   float RX() const {return fx + fw;}///< Get the right x position in percent
   float BY() const {return fy + fh;}///< Get the bottom y position in percent
   float CX() const {return fx + fw/2.0f;}///< Get the center x position in percent
   float CY() const {return fy + fh/2.0f;}///< Get the center y position in percent
   float W()  const {return fw;}///< Get the width in percent
   float H()  const {return fh;}///< Get the height in percent
};

Rectangle LayoutArea(Rectangle outer_area , LayoutRectangle layout_area);///< Helper function to get the actual area of a layout rectangle

std::ostream& operator<<(std::ostream& os , const LayoutRectangle& lr);///< Describe the layout rectangle to a stream



#endif // LayoutRectangle_HPP



