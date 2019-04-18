
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
 */




#include "Eagle/Gui/WidgetMover.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"




int WidgetMover::PrivateHandleEvent(EagleEvent e) {
   if (false) {///!active) {
      if (hotkey) {
         active = true;
         return DIALOG_INPUT_USED;
      }
      return DIALOG_OKAY;
   }
   if (!IsMouseEvent(e)) {return DIALOG_OKAY;}
   
   const Pos2I mabs = AbsParentPos().MovedBy(e.mouse.x , e.mouse.y);
   
   /// Only use dx and dy from the mouse event, because the relative mouse xy may change if our root moves b/c that moves us
   /// and the relative mouse position given to us in mouse.x and mouse.y
   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      mdxtotal += e.mouse.dx;
      mdytotal += e.mouse.dy;
   }
   
   const int mdx = mdxtotal;
   const int mdy = mdytotal;
   
   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      /// TODO : FIXME : This doesn't work for the root gui
      if (moving) {
         Rectangle newarea = original_area.OuterArea();
         newarea.MoveBy(mdx , mdy);
         mwidget->SetWidgetArea(newarea , true);
      }
      else if (sizing) {
         Rectangle oldarea = original_area.OuterArea();
         Rectangle newarea = oldarea;
         if (anchorw || anchorh) {
            /// horizontal or vertical sizing
            if (anchorw) {
               /// horizontal sizing
               if (size_corner == CELL_AREA_MIDDLELEFT) {
                  /// moving left edge
                  newarea.SetArea(oldarea.X() + mdx , oldarea.Y() , anchorw - mdx , oldarea.H());
               }
               if (size_corner == CELL_AREA_MIDDLERIGHT) {
                  /// moving right edge
                  newarea.SetArea(oldarea.X() , oldarea.Y() , anchorw + mdx , oldarea.H());
               }
            }
            if (anchorh) {
               /// vertical sizing
               if (size_corner == CELL_AREA_TOPMIDDLE) {
                  /// moving top edge
                  newarea.SetArea(oldarea.X() , oldarea.Y() + mdy , oldarea.W() , anchorh - mdy);
               }
               else if (size_corner == CELL_AREA_BOTTOMMIDDLE) {
                  /// moving bottom edge
                  newarea.SetArea(oldarea.X() , oldarea.Y() , oldarea.W() , anchorh + mdy);
               }
            }
            mwidget->SetWidgetArea(newarea , true);
         }
         else {
            /// diagonal sizing
            Pos2I newcorner = movept.MovedBy(mdx , mdy);
            newarea.SetCorners(anchorpt.X() , anchorpt.Y() , newcorner.X() , newcorner.Y());
            mwidget->SetWidgetArea(newarea , true);
         }
      }
      else {
         mwidget = 0;
         
         EAGLE_ASSERT(RootHandler());/// We need the root gui to tell us which widget is under the mouse
         
         WidgetBase* hwidget = RootHandler()?RootHandler()->GetWidgetAt(mabs.X() , mabs.Y()):(WidgetBase*)0;
         if (blist.find(hwidget) == blist.end()) {
            /// Not on blacklist
            if (wlist.empty() || (wlist.find(hwidget) != wlist.end())) {
               /// On whitelist, or no whitelist
               if (hwidget &&
                   (hwidget->Flags().FlagOn(MOVEABLE) || hwidget->Flags().FlagOn(RESIZEABLE))) {
                  mwidget = hwidget;
                  original_area = mwidget->GetWidgetArea();
                  /// We need the absolute position here
                  abs_area = mwidget->AbsoluteArea();
               }
            }
         }
         
         /// Set mouse icon here

         /// Icon depends on whether or not we are hovering over a margin cell
         /// or border cell
         MOUSE_POINTER_TYPE old_pointer_type = pointer_type;
         pointer_type = POINTER_NORMAL;
         
         if (mwidget) {
            CELL_AREA mcell = abs_area.OuterNP().GetCellArea(mabs.X() , mabs.Y());
            CELL_AREA bcell = abs_area.BorderNP().GetCellArea(mabs.X() , mabs.Y());
            
            EAGLE_DEBUG(
///               EagleInfo() << "mcell = " << mcell << " , bcell = " << bcell << " , mabsxy = " << mabs.X() << "," << mabs.Y() << std::endl;
///               EagleInfo() << "area = " << abs_area << std::endl;
            );
            
            if (mcell != CELL_AREA_OUTSIDE) {
               if (mcell != CELL_AREA_MIDDLEMIDDLE) {
                  /// Hovering over a margin cell, show sizing mouse icon
                  const static MOUSE_POINTER_TYPE mptrs[9] = {
                     POINTER_NW,
                     POINTER_N,
                     POINTER_NE,
                     POINTER_W,
                     POINTER_NORMAL,
                     POINTER_E,
                     POINTER_SW,
                     POINTER_S,
                     POINTER_SE
                  };
                  if (sizing_enabled && mwidget->Flags().FlagOn(RESIZEABLE)) {
                     pointer_type = mptrs[mcell];
                  }
               }
               else {
                  /// We're inside the border area
                  if (bcell != CELL_AREA_MIDDLEMIDDLE) {
                     /// Hovering over a border cell, show grab mouse icon
                     if (moving_enabled && mwidget->Flags().FlagOn(MOVEABLE)) {
                        pointer_type = POINTER_GRAB;
                     }
                  }
               }
            }
         }
         
         EagleGraphicsContext* win = RootHandler()?RootHandler()->GetDrawWindow():(EagleGraphicsContext*)0;
         
         EAGLE_ASSERT(win);/// We need the drawing window to set its mouse icon
         
         if (win && (pointer_type != old_pointer_type)) {
            /// Pointer changed
            if (pointer_type != POINTER_NORMAL) {
               macquired = win->AcquireMousePointer(this , pointer_type , true);
               EAGLE_ASSERT(macquired);
            }
            else {/// pointer_type == POINTER_NORMAL
               if (macquired) {
                  win->ReleaseMousePointer(this);
                  macquired = false;
               }
            }
         }
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (moving || sizing) {
         if (e.mouse.button == 1) {
            moving = false;
            sizing = false;
            if (macquired) {
               EagleGraphicsContext* win = RootHandler()?RootHandler()->GetDrawWindow():(EagleGraphicsContext*)0;
               if (win) {
                  win->ReleaseMousePointer(this);
               }
               macquired = false;
            }
            return DIALOG_INPUT_USED;
         }
      }
   }
   if ((e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) && /// on mouse button
       (mwidget) && /// don't try to move a null widget
       (e.mouse.button == 1)) /// only if LMB is pressed
   {
      if (!moving && !sizing) {/// so we don't interrupt ourselves
//         mxstart = e.mouse.x;
//         mystart = e.mouse.y;
         mdxtotal = 0;
         mdytotal = 0;
         original_area = mwidget->GetWidgetArea();
         abs_area = mwidget->AbsoluteArea();
         if (e.mouse.button == 1) {
            if (mwidget) {/// If we are over a widget
               if (moving_enabled && mwidget->Flags().FlagOn(MOVEABLE)) {
                  CELL_AREA bcell = abs_area.BorderNP().GetCellArea(mabs.X() , mabs.Y());
                  if ((bcell != CELL_AREA_OUTSIDE) && (bcell != CELL_AREA_MIDDLEMIDDLE)) {
                     /// User clicked on our border
                     moving = true;
                     /// If we have a drawing context, we can set the mouse pointer icon to the grabby hand to
                     /// indicate we're holding on to a widget
                     EagleGraphicsContext* win = RootHandler()?RootHandler()->GetDrawWindow():(EagleGraphicsContext*)0;
                     win->AcquireMousePointer(this , POINTER_GRABBED , true);
                     return DIALOG_INPUT_USED;
                  }
               }
               if (sizing_enabled && mwidget->Flags().FlagOn(RESIZEABLE)) {
                  CELL_AREA mcell = abs_area.OuterNP().GetCellArea(mabs.X() , mabs.Y());
                  if ((mcell != CELL_AREA_OUTSIDE) && (mcell != CELL_AREA_MIDDLEMIDDLE)) {
                     /// User clicked on our margin
                     size_corner = mcell;
                     sizing = true;
                     anchorw = 0;
                     anchorh = 0;
                     switch (mcell) {
                     case CELL_AREA_TOPLEFT :
                        movept.SetPos(original_area.OuterArea().X() , original_area.OuterArea().Y());
                        anchorpt.SetPos(original_area.OuterArea().BRX() , original_area.OuterArea().BRY());
                        break;
                     case CELL_AREA_TOPRIGHT :
                        movept.SetPos(original_area.OuterArea().BRX() , original_area.OuterArea().Y());
                        anchorpt.SetPos(original_area.OuterArea().X() , original_area.OuterArea().BRY());
                        break;
                     case CELL_AREA_BOTTOMLEFT :
                        movept.SetPos(original_area.OuterArea().X() , original_area.OuterArea().BRY());
                        anchorpt.SetPos(original_area.OuterArea().BRX() , original_area.OuterArea().Y());
                        break;
                     case CELL_AREA_BOTTOMRIGHT :
                        movept.SetPos(original_area.OuterArea().BRX() , original_area.OuterArea().BRY());
                        anchorpt.SetPos(original_area.OuterArea().X() , original_area.OuterArea().Y());
                        break;
                     case CELL_AREA_TOPMIDDLE :
                        anchorpt.SetPos(0 , original_area.OuterArea().BRY());
                        anchorh = original_area.OuterArea().H();
                        break;
                     case CELL_AREA_BOTTOMMIDDLE :
                        anchorpt.SetPos(0 , original_area.OuterArea().Y());
                        anchorh = original_area.OuterArea().H();
                        break;
                     case CELL_AREA_MIDDLELEFT :
                        anchorpt.SetPos(original_area.OuterArea().BRX() , 0);
                        anchorw = original_area.OuterArea().W();
                        break;
                     case CELL_AREA_MIDDLERIGHT :
                        anchorpt.SetPos(original_area.OuterArea().X() , 0);
                        anchorw = original_area.OuterArea().W();
                        break;
                     default:
                        break;
                     }
                     return DIALOG_INPUT_USED;
                  }
               }
            }
         }
      }
   }
   return DIALOG_OKAY;
}




void WidgetMover::SetAbilities(bool can_move , bool can_size) {
   moving_enabled = can_move;
   sizing_enabled = can_size;
}



void WidgetMover::SetHotKey(InputGroup ig) {
   hotkey = ig;
}



void WidgetMover::WhiteList(WidgetBase* widget) {
   /// Add to whitelist
   wlist.insert(widget);
   /// Remove from blacklist
   if (blist.find(widget) != blist.end()) {blist.erase(blist.find(widget));}
}



void WidgetMover::BlackList(WidgetBase* widget) {
   /// Add to blacklist
   blist.insert(widget);
   /// Remove from whitelist
   if (wlist.find(widget) != wlist.end()) {wlist.erase(wlist.find(widget));}
}



