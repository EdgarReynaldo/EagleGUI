
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
 * @file Allegro5MousePointer.hpp
 * @brief The interface for Allegro 5 mouse pointers
 */

#ifndef Allegro5MousePointer_HPP
#define Allegro5MousePointer_HPP



#include "Eagle/MousePointer.hpp"



struct ALLEGRO_MOUSE_CURSOR;


/**! @class Allegro5MousePointer
 *   @brief An implementation of @ref MousePointer for Allegro 5
 */

class Allegro5MousePointer : public MousePointer {
private :
   
   ALLEGRO_MOUSE_CURSOR* a5mpointer;
   
public :
   Allegro5MousePointer();///< Empty constructor
   Allegro5MousePointer(MousePointerInfo info);///< Create a pointer from MousePointerInfo

   virtual ~Allegro5MousePointer();///< Virtual destructor

   virtual bool Create(MousePointerInfo info);///< Create a mouse pointer from MousePointerInfo

   virtual void Free();///< Free this pointer

   virtual bool Valid();///< True if valid and ready
   
   ALLEGRO_MOUSE_CURSOR* GetAllegroMousePointer();///< Get the underlying ALLEGRO_MOUSE_CURSOR*
};



class Allegro5GraphicsContext;

struct ALLEGRO_DISPLAY;



/**! @class Allegro5MousePointerManager
 *   @brief A manager class for Allegro 5 mouse pointers
 */

class Allegro5MousePointerManager : public MousePointerManager {

protected :
   Allegro5GraphicsContext* a5win;///< Our graphics window
   ALLEGRO_DISPLAY* a5display;///< Our display
   

   bool SelectSystemMousePointer(MOUSE_POINTER_TYPE ptype);
   bool SelectCustomMousePointer(Allegro5MousePointer* a5mp);

   virtual bool SelectMousePointerDirect(MousePointer** mp);

   virtual MousePointer* CreateMousePointer(MousePointerInfo info);
   
public :

   Allegro5MousePointerManager(EagleGraphicsContext* window);

   virtual void ShowMouse();///< Show the mouse
   virtual void HideMouse();///< Hide the mouse
   
   virtual void SetMousePosition(int mousex , int mousey);///< Set the display relative mouse position
};



#endif // Allegro5MousePointer_HPP



