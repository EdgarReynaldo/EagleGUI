
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
 * @file Scrollable.hpp
 * @brief This file contains an interface for making widgets scrollable
 */

#ifndef Scrollable_HPP
#define Scrollable_HPP



/**! @class IScrollable
 *   @brief A simple interface class to give widgets scrolling abilities
 */

class IScrollable {

protected :
   int scrollx;
   int scrolly;
   
   virtual void ScrollCallback()=0;
      
public :
   
   IScrollable() : scrollx(0) , scrolly(0) {}
   virtual ~IScrollable() {}
   
   virtual void SetScrollOffset(int xoffset , int yoffset) {
      scrollx = xoffset;
      scrolly = yoffset;
      ScrollCallback();
   }

   void ScrollViewBy(int dx , int dy) {
      SetScrollOffset(scrollx + dx , scrolly + dy);
   }

   int GetScrollX() {return scrollx;}
   int GetScrollY() {return scrolly;}


};






#endif // Scrollable_HPP



