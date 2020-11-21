
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WidgetMover.hpp
 * @brief This file holds the interface for the @ref WidgetMover class
 * 
 */

#ifndef WidgetMover_HPP
#define WidgetMover_HPP



#include "Eagle/InputHandler.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/MousePointer.hpp"



#include <set>


/**! @class WidgetMover
 *   @brief A simple widget to control other widget's positions and areas graphically using the mouse pointer
 */

class WidgetMover : public WidgetBase {
   
protected :
   
   std::unordered_set<WidgetBase*> wlist;
   std::unordered_set<WidgetBase*> blist;
   
   InputGroup hotkey;
   bool active;

   WIDGETAREA original_area;
   WIDGETAREA abs_area;
   CELL_AREA size_corner;
   bool sizing_enabled;
   bool moving_enabled;
   bool sizing;
   bool moving;
   int mxstart;
   int mystart;
   int mdxtotal;
   int mdytotal;
   Pos2I anchorpt;
   Pos2I movept;
   int anchorw;
   int anchorh;
   WidgetBase* mwidget;
   bool macquired;
   MOUSE_POINTER_TYPE pointer_type;

   
   int PrivateHandleEvent(EagleEvent e);
   
public :
   
///   WidgetMover(std::string objname);
WidgetMover(std::string objname) :
      WidgetBase("WidgetMover" , objname),
      wlist(),
      blist(),
      hotkey(input_key_held(EAGLE_KEY_LSHIFT) && input_key_press(EAGLE_KEY_ENTER)),
      active(false),
      original_area(),
      abs_area(),
      size_corner(CELL_AREA_OUTSIDE),
      sizing_enabled(false),
      moving_enabled(false),
      sizing(false),
      moving(false),
//      mxstart(-1),
//      mystart(-1),
      mdxtotal(0),
      mdytotal(0),
      anchorpt(),
      movept(),
      anchorw(0),
      anchorh(0),
      mwidget(0),
      macquired(false),
      pointer_type(POINTER_NORMAL)
{
   
}
         
   void SetAbilities(bool can_move , bool can_size);///< Whether to allow moving or sizing
   void SetHotKey(InputGroup ig);///< Set the hot key for activation of the widget mover
   void WhiteList(WidgetBase* widget);///< Add a widget to the whitelist
   void BlackList(WidgetBase* widget);///< Add a widget to the blacklist
   
   WidgetBase* GetMoveWidget() {return mwidget;}///< The widget currently being moved
};



#endif // WidgetMover_HPP



