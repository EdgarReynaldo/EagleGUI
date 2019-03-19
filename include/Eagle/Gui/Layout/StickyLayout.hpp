
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



#ifndef StickyLayout_HPP
#define StickyLayout_HPP

#include "Eagle/Gui/Layout/Layout.hpp"


typedef Pos2I(*POSFUNC)(WidgetBase*);

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

Pos2I GetWidgetPositionTopLeft     (WidgetBase* w);
Pos2I GetWidgetPositionTopCenter   (WidgetBase* w);
Pos2I GetWidgetPositionTopRight    (WidgetBase* w);
Pos2I GetWidgetPositionMiddleLeft  (WidgetBase* w);
Pos2I GetWidgetPositionMiddleCenter(WidgetBase* w);
Pos2I GetWidgetPositionMiddleRight (WidgetBase* w);
Pos2I GetWidgetPositionBottomLeft  (WidgetBase* w);
Pos2I GetWidgetPositionBottomCenter(WidgetBase* w);
Pos2I GetWidgetPositionBottomRight (WidgetBase* w);

extern const POSFUNC standard_pos_funcs[NUM_ANCHORS];

class StickyPositionBase {
protected :
   
   WidgetBase* anchor_widget;
   WidgetBase* widget_to_move;
   
   Pos2I poffset;/// Offset the anchor position by this much

   HALIGNMENT halign;
   VALIGNMENT valign;

   ANCHOR_POS anchor_pos;
   


   virtual Pos2I GetOffset() {return poffset;}
   

public :

   StickyPositionBase();
   virtual ~StickyPositionBase() {}
   
   void SetAnchor(WidgetBase* anchor , ANCHOR_POS apos , HALIGNMENT halignment , VALIGNMENT valignment , Pos2I offset = Pos2I());

   Pos2I GetAnchorPoint();
   WidgetBase* AnchorWidget();
   HALIGNMENT GetHorizontalAlignment();
   VALIGNMENT GetVerticalAlignment();

};






#include "Eagle/SharedMemory.hpp"



///typedef SHAREDOBJECT<StickyPositionBase> StickyPosition;
typedef StickyPositionBase StickyPosition;



class StickyLayout : public LayoutBase , public EagleEventListener {
   
   std::vector<StickyPosition> anchors;
   
   virtual void ReserveSlots(int nslots);

   virtual int PrivateHandleEvent(EagleEvent ee);
   
public :
   
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

   void PlaceWidget(WidgetBase* w , int slot , StickyPosition sp);
   int AddWidget(WidgetBase* w , StickyPosition sp);

   void SetAnchor(int slot , StickyPosition sp);


};



#endif // StickyLayout_HPP
