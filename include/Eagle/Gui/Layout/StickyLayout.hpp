
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
 * @file StickyLayout.hpp
 * @brief A layout that makes widgets stick together, like a chain of widgets glued together
 * 
 * This class is good for anchoring widgets to a particular side or corner of another widget
 */

#ifndef StickyLayout_HPP
#define StickyLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"


/**! @typedef POSFUNC
 *   @brief A typedef for a function that returns an anchor position for a widget
 */

typedef Pos2I(*POSFUNC)(WidgetBase*);


/**! @enum ANCHOR_POS
 *   @brief Where to anchor the widget from
 */

enum ANCHOR_POS {
   ANCHOR_TOPLEFT      = 0,
   ANCHOR_TOPCENTER    = 1,
   ANCHOR_TOPRIGHT     = 2,
   ANCHOR_MIDDLELEFT   = 3,
   ANCHOR_MIDDLECENTER = 4,
   ANCHOR_MIDDLERIGHT  = 5,
   ANCHOR_BOTTOMLEFT   = 6,
   ANCHOR_BOTTOMCENTER = 7,
   ANCHOR_BOTTOMRIGHT  = 8,
   NUM_ANCHORS         = 9
};

///< Functions to get the specified position on a specified widget
Pos2I GetWidgetPositionTopLeft     (WidgetBase* w);
Pos2I GetWidgetPositionTopCenter   (WidgetBase* w);
Pos2I GetWidgetPositionTopRight    (WidgetBase* w);
Pos2I GetWidgetPositionMiddleLeft  (WidgetBase* w);
Pos2I GetWidgetPositionMiddleCenter(WidgetBase* w);
Pos2I GetWidgetPositionMiddleRight (WidgetBase* w);
Pos2I GetWidgetPositionBottomLeft  (WidgetBase* w);
Pos2I GetWidgetPositionBottomCenter(WidgetBase* w);
Pos2I GetWidgetPositionBottomRight (WidgetBase* w);

///extern const POSFUNC standard_pos_funcs[NUM_ANCHORS];



/**! @class StickyPosition
 *   @brief A class to make glue widget objects together for the StickyLayout class to use
 */

class StickyPosition {
protected :
   
   WidgetBase* anchor_widget;///< The widget to anchor to
   WidgetBase* widget_to_move;///< The widget that is anchored, and moves with the anchor widget
   
   Pos2I poffset;///< The offset for the anchor position, specifies the relative position to the anchor widget

   HALIGNMENT halign;///< The specified horizontal alignment of the widget being moved relative to the anchor widget
   VALIGNMENT valign;///< The specified vertical alignment of the widget being moved relative to the anchor widget

   ANCHOR_POS anchor_pos;///< Where to anchor the widget to relative to the anchor widget
   
   
   
   virtual Pos2I GetOffset() {return poffset;}

public :

   StickyPosition();
   virtual ~StickyPosition() {}
   
   ///< Set the anchor widget using the specified alignment and offset from the widget
   void SetAnchor(WidgetBase* anchor , ANCHOR_POS apos , HALIGNMENT halignment , VALIGNMENT valignment , Pos2I offset = Pos2I());

   ///< Get the anchor point stored in this StickyPosition object
   Pos2I GetAnchorPoint();
   
   ///< Get a pointer to the anchor widget stored in this StickyPosition
   WidgetBase* AnchorWidget();
   
   ///< Get the horizontal alignment to the anchor widget stored in this StickyPosition
   HALIGNMENT GetHorizontalAlignment();

   ///< Get the vertical alignment to the anchor widget stored in this StickyPosition
   VALIGNMENT GetVerticalAlignment();

};



/**! @class StickyLayout
 *   @brief A layout to make widgets stick together
 */

class StickyLayout : public LayoutBase , public EagleEventListener {
   
   std::vector<StickyPosition> anchors;
   
   virtual void ReserveSlots(int nslots);

   virtual int PrivateHandleEvent(EagleEvent ee);
   
public :
   
   /// @sa LayoutBase::RequestWidgetArea
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   /// @sa LayoutBase::PlaceWidget
   virtual void PlaceWidget(WidgetBase* w , int slot);

   /// @sa LayoutBase::AddWidget
   virtual int AddWidget(WidgetBase* w);

   ///< Places a widget in the specified slot with the specified StickyPosition
   void PlaceWidget(WidgetBase* w , int slot , StickyPosition sp);
   
   ///< Adds a widget to the next empty slot using the specified StickyPosition
   int AddWidget(WidgetBase* w , StickyPosition sp);

   ///< Alter the anchor properties using a StickyPosition
   void SetAnchor(int slot , StickyPosition sp);
};



#endif // StickyLayout_HPP



