
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
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Gui/Layout/SplitterLayouts.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/StringWork.hpp"






void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , const WidgetColorset& colors) {
   DefaultSplitterDrawFunction(window , divider_rect , horizontal , colors[HLCOL] , colors[SDCOL]);
}



void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , EagleColor hlcol , EagleColor sdcol) {
   float x = divider_rect.X();
   float y = divider_rect.Y();
   float w = divider_rect.W();
   float h = divider_rect.H();
   window->DrawFilledRectangle(x,y,w,h,hlcol);
//   EagleInfo() << StringPrintF("DefaultSplitterDrawFunction x,y,w,h = %d,%d,%d x %d\n",
//                              (int)x,(int)y,(int)w,(int)h);
   if (horizontal) {
      // horizontal bar
      window->DrawFilledRectangle(x,y,w,h/4.0,sdcol);
      window->DrawFilledRectangle(x,y+3.0*h/4.0,w,h/4.0,sdcol);
   }
   else {
      // vertical bar
      window->DrawFilledRectangle(x,y,w/4.0,h,sdcol);
      window->DrawFilledRectangle(x+3.0*w/4.0,y,w/4.0,h,sdcol);
   }
}



void TwoWaySplitter::ReserveSlots(int nslots) {
   (void)nslots;
   LayoutBase::ReserveSlots(2);
}



Rectangle TwoWaySplitter::GetHandleArea() {
   Rectangle r = InnerArea();
   int x = r.X();
   int y = r.Y();
   int w = r.W();
   int h = r.H();
   switch (splitter_type) {
   case SPLITTER_VERTICAL :
      x += divider_position;
      w = divider_size;
      break;
   case SPLITTER_HORIZONTAL :
      y += divider_position;
      h = divider_size;
      break;
   }
   return Rectangle(x,y,w,h);
}



TwoWaySplitter::TwoWaySplitter(SPLITTER_TYPE stype , EagleGraphicsContext* window , std::string objname) : 
      LayoutBase("TwoWaySplitter" , objname),
      splitter_type(stype),
      draw_func(DefaultSplitterDrawFunction),
      divider_size(4),
      divider_position(0),
      divider_percent(0.5f),
      drag(false),
      olddrag(false),
      dragxstart(0),
      dragystart(0),
      divposstart(0),
      oldhover(false),
      mouse_window(window),
      owned_pointer_count(0)
{
   ReserveSlots(2);
}



TwoWaySplitter::~TwoWaySplitter() {
   (void)0;
}



int TwoWaySplitter::PrivateHandleEvent(EagleEvent e) {
   int ret = DIALOG_OKAY;
   

   if (IsMouseEvent(e)) {
      int mx = e.mouse.x;
      int my = e.mouse.y;
      olddrag = drag;
      bool hover = false;
      if ( (hover = GetHandleArea().Contains(mx,my)) ) {
         // hover over handle
         /// TODO : ShowMouseIcon
         if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1) {
            /// click
            drag = true;
            dragxstart = mx;
            dragystart = my;
            divposstart = divider_position;
            ret |= DIALOG_INPUT_USED;
         }
      }
      
      if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP && e.mouse.button == 1) {
         drag = false;
      }
      
      if (mouse_window) {// only if we have a mouse window

         MOUSE_POINTER_TYPE POINTER_IN_USE = POINTER_NS;// up-down arrow
         if (splitter_type == SPLITTER_VERTICAL) {
            POINTER_IN_USE = POINTER_EW;// left-right arrow
         }
         
         if (hover && !oldhover) {
            // just got mouse back over us
            mouse_window->AcquireMousePointer(this , POINTER_IN_USE , true);
            if (!drag) {
               // we aren't dragging, so this is the first time we have gotten hover, so take the mouse
            //   mouse_window->AcquireMousePointer(this , POINTER_IN_USE , true);
            }
            else {
               // drag is in effect, so just leave the mouse pointer alone
            }
         }
         
         if (drag && !olddrag) {
            mouse_window->AcquireMousePointer(this , POINTER_IN_USE , true);
         }
         
         if (!hover && oldhover) {
            // just lost hover
            mouse_window->ReleaseMousePointer(this);
         }
         if (!drag && olddrag) {
            mouse_window->ReleaseMousePointer(this);
         }
      }
      
      if (drag && e.type == EAGLE_EVENT_MOUSE_AXES) {
         int dx = mx - dragxstart;
         int dy = my - dragystart;
         int newdivpos = divposstart;
         switch (splitter_type) {
         case SPLITTER_VERTICAL:
            newdivpos += dx;
            break;
         case SPLITTER_HORIZONTAL:
            newdivpos += dy;
            break;
         }
         SetDividerPos(newdivpos);
      }
      
      oldhover = hover;
      olddrag = drag;
      
   }
   return ret;
}



int TwoWaySplitter::PrivateCheckInputs() {
   return DIALOG_OKAY;
}


//typedef void (*SPLITTER_DRAW_FUNC) (EagleGraphicsContext* window , Rectangle divider_rect , SPLITTER_TYPE type , const WidgetColorset& colors);

void TwoWaySplitter::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle handle = GetHandleArea();

   handle.MoveBy(Pos2I(xpos,ypos));

//   EagleInfo() << StringPrintF("TwoWaySplitter::PrivateDisplay handle x,y,w,h = %d , %d , %d x %d , xpos,ypos = %d , %d\n" ,
//                              handle.X(),handle.Y(),handle.W(),handle.H(),xpos,ypos);
   (*draw_func)(win , handle , splitter_type , WidgetColors());
}



void TwoWaySplitter::OnAreaChanged() {
   SetDividerPercent(divider_percent);
}



int TwoWaySplitter::AbsMinWidth() {
   if (splitter_type == SPLITTER_VERTICAL) {
      return divider_size;
   }
   return WidgetBase::AbsMinHeight();
}



int TwoWaySplitter::AbsMinHeight() {
   if (splitter_type == SPLITTER_HORIZONTAL) {
      return divider_size;
   }
   return WidgetBase::AbsMinHeight();
}
   


void TwoWaySplitter::PlaceWidget(SHAREDWIDGET widget , int slot) {
   if ((slot != 0) && (slot != 1)) {
      
      throw EagleException(StringPrintF("ERROR : TwoWaySplitter::PlaceWidget - Slot %d is invalid." , slot));
   }
   LayoutBase::PlaceWidget(widget , slot);
}



int TwoWaySplitter::AddWidget(SHAREDWIDGET widget) {
   if (NextFreeSlot() == -1) {
      throw EagleException("ERROR : TwoWaySplitter::AddWidget - No free slots available.");
   }
   return LayoutBase::AddWidget(widget);
}


Rectangle TwoWaySplitter::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const {
   
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   
   if ((widget_slot != 0) && (widget_slot != 1)) {
      throw EagleException(StringPrintF("ERROR : TwoWaySplitter::PlaceWidget - Slot %d is invalid." , widget_slot));
   }

   const WidgetBase* widget = GetWidget(widget_slot);
   
   if (!widget) {
      return Rectangle(-1,-1,-1,-1);
   }

   Rectangle r = InnerArea();
   int x = r.X();
   int y = r.Y();
   int w = r.W();
   int h = r.H();
   switch (splitter_type) {
   case SPLITTER_VERTICAL :
      if (widget_slot == 0) {
         // left panel
         x += 0;
         w = divider_position;
      }
      else if (widget_slot == 1) {
         // right panel
         x += divider_position + divider_size;
         w = (r.W() - (divider_position + divider_size));
      }
      break;
   case SPLITTER_HORIZONTAL :
      if (widget_slot == 0) {
         // top panel
         y += 0;
         h = divider_position;
      }
      else if (widget_slot == 1) {
         // bottom panel
         y += divider_position + divider_size;
         h = (r.H() - (divider_position + divider_size));
      }
      break;
   }
   
   return Rectangle(x,y,w,h);
}





void TwoWaySplitter::SetDividerPosActual(int divpos) {
   divider_position = divpos;
///      divider_percent = divider_position / (float)maxdiv;
   RepositionAllChildren();
   SetBgRedrawFlag();
}



void TwoWaySplitter::SetDividerSize(int divsize) {
   if (divsize < 2) {divsize = 2;}
   int maxdiv = InnerArea().W();
   if (splitter_type == SPLITTER_HORIZONTAL) {
      maxdiv = InnerArea().H();
   }
   if (divsize > maxdiv) {divsize = maxdiv;}
   divider_size = divsize;
   SetDividerPercent(divider_percent);
}



void TwoWaySplitter::SetDividerPos(int divpos) {
   if (divpos < 0) {divpos = 0;}
   int maxdiv = InnerArea().W() - divider_size;
   if (splitter_type == SPLITTER_HORIZONTAL) {
      maxdiv = InnerArea().H() - divider_size;
   }
   if (divpos > maxdiv) {divpos = maxdiv;}
   divider_percent = divider_position / (float)maxdiv;// |  x     |
   SetDividerPosActual(divpos);
}



void TwoWaySplitter::SetDividerPercent(float percent) {
   if (percent < 0.0f) {percent = 0.0f;}
   if (percent > 1.0f) {percent = 1.0f;}
   
   divider_percent = percent;
   
   float maxdiv = InnerArea().W() - divider_size;
   if (splitter_type == SPLITTER_HORIZONTAL) {
      maxdiv = InnerArea().H() - divider_size;
   }
   
   int divpos = (int)(maxdiv*divider_percent);
   SetDividerPosActual(divpos);
}



void TwoWaySplitter::SetMouseWindow(EagleGraphicsContext* window) {
   /// TODO : Release from old window if necessary?
   mouse_window = window;
}




