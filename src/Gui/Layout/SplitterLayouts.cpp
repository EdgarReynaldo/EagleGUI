
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



#include "Eagle/Gui/Layout/SplitterLayouts.hpp"
#include "Eagle/StringWork.hpp"



void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , SPLITTER_TYPE type , const WidgetColorset& colors) {
   float x = divider_rect.X();
   float y = divider_rect.Y();
   float w = divider_rect.W();
   float h = divider_rect.H();
   window->DrawFilledRectangle(x,y,w,h,colors[HLCOL]);
//   EagleLog() << StringPrintF("DefaultSplitterDrawFunction x,y,w,h = %d,%d,%d x %d\n",
//                              (int)x,(int)y,(int)w,(int)h);
   if (type == SPLITTER_VERTICAL) {
      // vertical bar
      window->DrawFilledRectangle(x,y,w/4.0,h,colors[SDCOL]);
      window->DrawFilledRectangle(x+3.0*w/4.0,y,w/4.0,h,colors[SDCOL]);
   }
   else if (type == SPLITTER_HORIZONTAL) {
      // horizontal bar
      window->DrawFilledRectangle(x,y,w,h/4.0,colors[SDCOL]);
      window->DrawFilledRectangle(x,y+3.0*h/4.0,w,h/4.0,colors[SDCOL]);
   }
}



Rectangle TwoWaySplitter::GetHandleArea() {
   Rectangle r = area.InnerArea();
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



void TwoWaySplitter::RepositionAllChildren() {
   RepositionChild(0);
   RepositionChild(1);
}



void TwoWaySplitter::RepositionChild(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < 2);
   WidgetBase* widget = wchildren[slot];
   if (!widget) {return;}
   
   widget->SetWidgetArea(RequestWidgetArea(widget , -1,-1,-1,-1) , false);
/**
   Rectangle r(area.InnerArea());
   int x = r.X();
   int y = r.Y();
   int w = r.W();
   int h = r.H();
   switch (splitter_type) {
   case SPLITTER_VERTICAL :
      if (slot == 0) {
         // left panel
         x += 0;
         w = divider_position;
      }
      else if (slot == 1) {
         // right panel
         x += divider_position + divider_size;
         w = (r.W() - (divider_position + divider_size));
      }
      break;
   case SPLITTER_HORIZONTAL :
      if (slot == 0) {
         // top panel
         y += 0;
         h = divider_position;
      }
      else if (slot == 1) {
         // bottom panel
         y += divider_position + divider_size;
         h = (r.H() - (divider_position + divider_size));
      }
      break;
   }
   
   widget->SetWidgetArea(x,y,w,h,false);
//*/   
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

   handle.MoveBy(Pos2d(xpos,ypos));

//   EagleLog() << StringPrintF("TwoWaySplitter::PrivateDisplay handle x,y,w,h = %d , %d , %d x %d , xpos,ypos = %d , %d\n" ,
//                              handle.X(),handle.Y(),handle.W(),handle.H(),xpos,ypos);
   (*draw_func)(win , handle , splitter_type , WCols());
   ClearRedrawFlag();
}




TwoWaySplitter::TwoWaySplitter(SPLITTER_TYPE stype , EagleGraphicsContext* window) : 
      Layout(StringPrintF("TwoWaySplitter object at %p" , this)),
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
   /// In case we go out of scope before our WidgetHandler
   DetachFromGui();
}


/**
void TwoWaySplitter::SetWidgetArea(const Rectangle& r , bool notify_layout) {
   WidgetBase::SetWidgetArea(r , notify_layout);
}
*/


void TwoWaySplitter::PlaceWidget(WidgetBase* widget , int slot) {
   EAGLE_ASSERT(slot == 0 || slot == 1);
   Layout::PlaceWidget(widget , slot);
}



void TwoWaySplitter::AddWidget(WidgetBase* widget) {
   EAGLE_ASSERT(NextFreeSlot() != -1);
   Layout::AddWidget(widget);
}



void TwoWaySplitter::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos , ypos , width , height,notify_layout);
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
   



void TwoWaySplitter::SetDividerPosActual(int divpos) {
   divider_position = divpos;
///      divider_percent = divider_position / (float)maxdiv;
   RepositionAllChildren();
   SetBgRedrawFlag();
}



void TwoWaySplitter::SetDividerSize(int divsize) {
   if (divsize < 2) {divsize = 2;}
   int maxdiv = area.InnerArea().W();
   if (splitter_type == SPLITTER_HORIZONTAL) {
      maxdiv = area.InnerArea().H();
   }
   if (divsize > maxdiv) {divsize = maxdiv;}
   divider_size = divsize;
   SetDividerPercent(divider_percent);
}



void TwoWaySplitter::SetDividerPos(int divpos) {
   if (divpos < 0) {divpos = 0;}
   int maxdiv = area.InnerArea().W() - divider_size;
   if (splitter_type == SPLITTER_HORIZONTAL) {
      maxdiv = area.InnerArea().H() - divider_size;
   }
   if (divpos > maxdiv) {divpos = maxdiv;}
   divider_percent = divider_position / (float)maxdiv;// |  x     |
   SetDividerPosActual(divpos);
}



Rectangle TwoWaySplitter::RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight) {
   
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   
   if (!widget) {
      return Rectangle();
   }

   int slot = WidgetIndex(widget);
   EAGLE_ASSERT(slot >= 0);

   Rectangle r(area.InnerArea());
   int x = r.X();
   int y = r.Y();
   int w = r.W();
   int h = r.H();
   switch (splitter_type) {
   case SPLITTER_VERTICAL :
      if (slot == 0) {
         // left panel
         x += 0;
         w = divider_position;
      }
      else if (slot == 1) {
         // right panel
         x += divider_position + divider_size;
         w = (r.W() - (divider_position + divider_size));
      }
      break;
   case SPLITTER_HORIZONTAL :
      if (slot == 0) {
         // top panel
         y += 0;
         h = divider_position;
      }
      else if (slot == 1) {
         // bottom panel
         y += divider_position + divider_size;
         h = (r.H() - (divider_position + divider_size));
      }
      break;
   }
   
   return Rectangle(x,y,w,h);
}



void TwoWaySplitter::SetDividerPercent(float percent) {
   if (percent < 0.0f) {percent = 0.0f;}
   if (percent > 1.0f) {percent = 1.0f;}
   
   divider_percent = percent;
   
   float maxdiv = area.InnerArea().W() - divider_size;
   if (splitter_type == SPLITTER_HORIZONTAL) {
      maxdiv = area.InnerArea().H() - divider_size;
   }
   
   int divpos = (int)(maxdiv*divider_percent);
   SetDividerPosActual(divpos);
}



void TwoWaySplitter::SetMouseWindow(EagleGraphicsContext* window) {
   /// TODO : Release from old window if necessary?
   mouse_window = window;
}




