
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 
#include "Eagle/Gui/Layout/StickyLayout.hpp"



const POSFUNC standard_pos_funcs[NUM_ANCHORS] = {
   GetWidgetPositionTopLeft,
   GetWidgetPositionTopCenter,
   GetWidgetPositionTopRight,
   GetWidgetPositionMiddleLeft,
   GetWidgetPositionMiddleCenter,
   GetWidgetPositionMiddleRight,
   GetWidgetPositionBottomLeft,
   GetWidgetPositionBottomCenter,
   GetWidgetPositionBottomRight
};



Pos2I GetWidgetPositionTopLeft(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().TopLeft();
}



Pos2I GetWidgetPositionTopCenter(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().TopCenter();
}



Pos2I GetWidgetPositionTopRight(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().TopRight();
}



Pos2I GetWidgetPositionMiddleLeft(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().MiddleLeft();
}



Pos2I GetWidgetPositionMiddleCenter(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().MiddleCenter();
}



Pos2I GetWidgetPositionMiddleRight(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().MiddleRight();
}



Pos2I GetWidgetPositionBottomLeft(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().BottomLeft();
}



Pos2I GetWidgetPositionBottomCenter(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().BottomCenter();
}



Pos2I GetWidgetPositionBottomRight(WidgetBase* w) {
   EAGLE_ASSERT(w);
   return w->OuterArea().BottomRight();
}



/// -------------------      StickyPositionBase    ------------------------------



StickyPositionBase::StickyPositionBase() :
      anchor_widget(0),
      poffset(),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      anchor_pos(ANCHOR_TOPLEFT)
{}



void StickyPositionBase::SetAnchor(WidgetBase* anchor , ANCHOR_POS apos , HALIGNMENT halignment , VALIGNMENT valignment , Pos2I offset) {
   anchor_widget = anchor;
   poffset = offset;
   halign = halignment;
   valign = valignment;
   anchor_pos = apos;
}



Pos2I StickyPositionBase::GetAnchorPoint() {
   if (!anchor_widget) {return GetOffset();}

   Pos2I anchor = standard_pos_funcs[anchor_pos](anchor_widget);
   anchor.MoveBy(GetOffset());
   return anchor;
}



WidgetBase* StickyPositionBase::AnchorWidget() {
   return anchor_widget;
}



HALIGNMENT StickyPositionBase::GetHorizontalAlignment() {
   return halign;
}



VALIGNMENT StickyPositionBase::GetVerticalAlignment() {
   return valign;
}



/// ----------------------     StickyLayout     ---------------------------------



int StickyLayout::PrivateHandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_WIDGET) {
      if (ee.widget.topic == TOPIC_DIALOG && ee.widget.msgs == DIALOG_I_MOVED) {
         WidgetBase* from = ee.widget.from;
         for (int i = 0 ; i < (int)anchors.size() ; ++i) {
            WidgetBase* awidget = anchors[i].AnchorWidget();
            if (awidget == from) {
               RepositionChild(i);
            }
         }
      }
   }
   return DIALOG_OKAY;
}




void StickyLayout::ReserveSlots(int nslots) {
   anchors.resize(nslots);
   RepositionAllChildren();
}



Rectangle StickyLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   
   const WidgetBase* widget = GetWidget(widget_slot);

   if (!widget) {
      return BADRECTANGLE;
   }

   Rectangle r = widget->OuterArea();
   (void)newx;
   (void)newy;
   
   /// newx and newy are ignored by this layout
   if (newwidth == INT_MAX) {newwidth = r.W();}
   if (newheight == INT_MAX) {newheight = r.H();}
   
   StickyPosition sp = anchors[widget_slot];
   Pos2I anchor = sp.GetAnchorPoint();
   HALIGNMENT halign2 = sp.GetHorizontalAlignment();
   VALIGNMENT valign2 = sp.GetVerticalAlignment();
   
   anchor.MoveBy((halign2 == HALIGN_RIGHT)?-r.W():(halign2 == HALIGN_CENTER)?-r.W()/2:0 ,
                 (valign2 == VALIGN_BOTTOM)?-r.H():(valign2 == VALIGN_CENTER)?-r.H()/2:0);
   
   return Rectangle(anchor.X() , anchor.Y() , newwidth , newheight);
}



void StickyLayout::PlaceWidget(WidgetBase* w , int slot) {
   StickyPosition spos;
   spos.SetAnchor(this , ANCHOR_TOPLEFT , HALIGN_LEFT , VALIGN_TOP);
   PlaceWidget(w , slot , spos);
}



int StickyLayout::AddWidget(WidgetBase* w) {
   StickyPosition spos;
   spos.SetAnchor(this , ANCHOR_TOPLEFT , HALIGN_LEFT , VALIGN_TOP);
   return AddWidget(w , spos);
}



void StickyLayout::PlaceWidget(WidgetBase* w , int slot , StickyPosition sp) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)anchors.size());
   anchors[slot] = sp;
   LayoutBase::PlaceWidget(w , slot);
}



int StickyLayout::AddWidget(WidgetBase* w , StickyPosition sp) {
   int newslot = LayoutBase::AddWidget(w);
   anchors[newslot] = sp;
   RepositionChild(newslot);
   return newslot;
}



void StickyLayout::StickyLayout::SetAnchor(int slot , StickyPosition sp) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)anchors.size());
   WidgetBase* a = sp.AnchorWidget();
   if (a) {ListenTo(a);}
   anchors[slot] = sp;
   RepositionChild(slot);
}






