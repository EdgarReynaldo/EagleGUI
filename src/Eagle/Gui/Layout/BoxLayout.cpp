
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
 * @file 
 * @brief
 * 
 * 
 * 
 */

#include "Eagle/Gui/Layout/BoxLayout.hpp"



void BoxLayout::Resize(unsigned int nsize) {
   areas.resize(nsize , BADRECTANGLE);
   LayoutBase::Resize(nsize);
}



BoxLayout::BoxLayout(std::string classname , std::string objname) :
      LayoutBase(classname , objname),
      overflow(false),
      box_rules(BOX_ALIGN),
      areas()
{
   LayoutBase::SetAlignment(HALIGN_CENTER , VALIGN_CENTER);
}



Rectangle BoxLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   
   Rectangle r = areas[widget_slot];
   
   return r;
}



void BoxLayout::PlaceWidget(WidgetBase* w , int slot) {
   LayoutBase::PlaceWidget(w , slot);
   RecalcFlow();
   RepositionAllChildren();
}



int BoxLayout::AddWidget(WidgetBase* w) {
   LayoutBase::AddWidget(w);
   RecalcFlow();
   RepositionAllChildren();
}



void HBoxLayout::RecalcFlow() {
   areas.clear();
   areas.resize(wchildren.size() , BADRECTANGLE);
   
   Rectangle& in = InnerArea();
   int ix = in.X();
   int iy = in.Y();
   int iw = in.W();
   int ih = in.H();
   

   /// calculate space needed for preferred size and minimum size of widgets
   int nchildren = 0;
   int maxh = 0;
   int totalprefw = 0;
   int totalminw = 0;
   int noprefcount = 0;
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (!w) {
         continue;
      }
      ++nchildren;
      if (w->PreferredWidth() == 0) {
         noprefcount++;
         continue;
      }

      EAGLE_ASSERT(w->PreferredWidth() >= w->AbsMinWidth());
      
      totalprefw += w->PreferredWidth();
      totalminw += w->AbsMinWidth();
      if (w->PreferredHeight() > maxh) {
         maxh = w->PreferredHeight();
      }
   }
   
   if (totalminw > InnerArea().W()) {
      overflow = true;
      return;
   }
   
   if (maxh > InnerArea.H()) {
      overflow = true;
   }
   
   
///   BOX_ALIGN_ONLY    = 0,///< Left over space is unused
///   BOX_EXPAND        = 1,///< Left over space is given completely to widgets
///   BOX_SPACE_BETWEEN = 2,///< Left over space is split up between each widget, pushing them out from the middle
///   BOX_SPACE_EVEN    = 3 ///< Left over space is split up evenly between each widget on its outer edges
   /// Now we have the total preferred width we can allocate space according to our sizing rules
   int leftover = InnerArea().W() - totalprefw;
   if (box_rules != BOX_ALIGN_ONLY) {
      if (leftover < 0) {
         /// Widgets take up too much room at their preferred size, so we must shrink them to fit
         
      }
      else if (leftover > 0) {
         int left = (noprefcount?(leftover / noprefcount):0);/// Give the remaining space to the unspecified widgets
         if (left) {
            for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
               WidgetBase* w = wchildren[i];
               if (!w) {continue;}
               int pw = w->PreferredWidth();
               if (!pw) {
                  pw = left;
               }
               
               ix += pw;
            }
         }
         else {
            /// Extra space leftover, follow box size rules to expand or pad widgets
            if (box_rules == BOX_EXPAND) {
               int grow = leftover / nchildren;
               int extra = leftover % nchildren;
               for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
                  WidgetBase* w = wchildren[i];
                  if (!w) {continue;}
                  int ww = w->PreferredWidth() + grow + (i%nchildren)?1:0; 
                  areas[i].SetArea(ix , iy , ww , maxh);
                  ix += ww;
               }
            }
            else if (box_rules == BOX_SPACE_BETWEEN) {
               /// Push widgets outwards from centered middle widget, creating N-1 horizontal spaces
               int ncols = nchildren - 1;
               if (ncols >= 1) {
                  int colsize = leftover / ncols;
                  int extra = leftover % colsize;
                  for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
                     WidgetBase* w = wchildren[i];
                     if (!w) {continue;}
                     areas[i].SetArea(ix , iy , w->PreferredWidth() , maxh);
                     ix += w->PreferredWidth() + colsize;
                  }
               }
               else if (nchildren == 1) {
                  /// This is a conundrum. Only one widget, with sizing option space between
                  /// For now just give the single widget the entire space as if it was set to expand
                  areas[0].SetArea(ix , iy , iw , ih);
               }
            }
            else if (box_rules == BOX_SPACE_EVEN) {
               /// Create N + 1 columnds of padding surrounding the widgets and between them
               int ncols = nchildren + 1;
               int colsize = leftover / ncols;
               int extra = leftover % ncols
               for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
                  WidgetBase* w = wchildren[i];
                  if (!w) {continue;}
                  ix += colsize;
                  if (i % extra) {
                     ix += 1;
                  }
                  areas[i].SetArea(ix , iy , w->PreferredWidth() , maxh);
                  ix += w->PreferredWidth();
               }
            }
         }
      }
      else {
         /// Just enough room for each widget to have their preferred size
         for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            areas[i].SetArea(ix , iy , w->PreferredWidth() , maxh);
            ix += w->PreferredWidth();
         }
      }
   }
   else {/// BOX_ALIGN_ONLY
      /// Our horizontal alignment tells us whether to place widgets near the beginning, center, or end of the box
      /// These act like flex-start , flex-end, and flex-center in a CSS Flexbox for the hbox and vbox layouts
      switch (halign) {
      case HALIGN_LEFT :
         ix = ix;
         break;
      case HALIGN_CENTER :
         ix += leftover/2;
         break;
      case HALIGN_RIGHT :
         ix += leftover;
         break;
      default :
         break;
      }
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         areas[i].SetArea(ix , iy , w->PreferredWidth() , maxh);
         ix += w->PreferredWidth();
      }
   }
}







