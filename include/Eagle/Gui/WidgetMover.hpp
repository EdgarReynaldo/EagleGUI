
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
#include "Eagle/Events.hpp"


#include <set>


extern const unsigned int TOPIC_WIDGET_MOVER;///< The topic for all widget mover messages



/**! @enum WIDGET_MOVER_MESSAGES
 *   @brief The messages passed by a @ref WidgetMover.
 *   When a widget starts moving a widget, it will emit a pickup message, followed by moving messages, completed by a drop message.
 *   When a widget sizes a widget, it will begin by emitting a sizing message, completed by a size finish message.
 */
enum WIDGET_MOVER_MESSAGES {
   WIDGET_MOVER_PICKUP      = 1,///< The mover has picked up a widget
   WIDGET_MOVER_MOVING      = 2,///< The mover is moving a widget
   WIDGET_MOVER_DROP        = 3,///< The mover has dropped a widget
   WIDGET_MOVER_SIZING      = 4,///< The mover has started or is continuing to size a widget
   WIDGET_MOVER_SIZE_FINISH = 5 ///< The mover has finished sizing a widget
};



/**! @class WidgetMover
 *   @brief A simple widget to control other widget's positions and areas graphically using the mouse pointer
 *   A widget needs a border to be properly sized. The mover will pickup widgets and move them until they are dropped,
 *   at which point a WIDGET_MOVER_DROP event is sent to its listeners.
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

   DRAG_AND_DROP_DATA dnd;
   
   int PrivateHandleEvent(EagleEvent e);
   
public :
   
   WidgetMover(std::string objname = "Mover");
         
   void SetAbilities(bool can_move , bool can_size);///< Whether to allow moving or sizing. Both are true to begin with.
   void SetHotKey(InputGroup ig);///< Set the hot key for activation of the widget mover
   void WhiteList(WidgetBase* widget);///< Add a widget to the whitelist
   void BlackList(WidgetBase* widget);///< Add a widget to the blacklist
   
   WidgetBase* GetMoveWidget() {return mwidget;}///< The widget currently being moved
};



#endif // WidgetMover_HPP



