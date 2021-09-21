
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
 * @file BoxLayout.cpp
 * @brief Implentations for the BoxLayout family of classes
 */

#include "Eagle/Gui/Layout/BoxLayout.hpp"



std::string PrintBoxAnchorPoint(BOX_ANCHOR_POINT p) {
   static const char* points[11] = {
      "INVALID",
      "HBOX_ANCHOR_W" ,//1
      "HBOX_ANCHOR_E" ,//2
      "INVALID",
      "VBOX_ANCHOR_N" ,//4
      "FBOX_ANCHOR_NW",//5
      "FBOX_ANCHOR_NE",//6
      "INVALID",
      "VBOX_ANCHOR_S" ,//8
      "FBOX_ANCHOR_SW",//9
      "FBOX_ANCHOR_SE" //10
   };
   return points[p];
}


std::string PrintBoxSpaceRule(BOX_SPACE_RULES b) {
   static const char* rules[4] = {
      "BOX_ALIGN_ONLY",
      "BOX_EXPAND",
      "BOX_SPACE_BETWEEN",
      "BOX_SPACE_EVEN"
   };
   return rules[b];
}



void BoxLayout::Resize(unsigned int nsize) {
   rcsizes.resize(nsize , BADRECTANGLE);
   LayoutBase::Resize(nsize);
}



BoxLayout::BoxLayout(std::string classname , std::string objname) :
   LayoutBase(classname , objname)
{}



Rectangle BoxLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   WidgetBase* w = LayoutBase::GetWidget(widget_slot);
   if (!w) {return BADRECTANGLE;}
   
   if (newwidth != w->PreferredWidth() || newheight != w->PreferredHeight()) {
      w->SetPreferredSize(newwidth , newheight);
      RecalcFlow();
   }
   
   Rectangle r = rcsizes[widget_slot];
   
   return r;
}



void BoxLayout::PlaceWidget(WidgetBase* w , int slot) {
   LayoutBase::PlaceWidget(w , slot);
   RecalcFlow();
   RepositionAllChildren();
}



int BoxLayout::AddWidget(WidgetBase* w) {
   int ret = LayoutBase::AddWidget(w);
   RecalcFlow();
   RepositionAllChildren();
   return ret;
}



void BoxLayout::InsertWidget(WidgetBase* w , int slot) {
   LayoutBase::InsertWidget(w , slot);
   RecalcFlow();
   RepositionAllChildren();
}



/// ----------------------     HBOX       ---------------------------



void HBoxLayout::RecalcFlow() {
   rcsizes.clear();
   rcsizes.resize(widget_children.size() , BADRECTANGLE);
   
   overflow = false;
   totalprefw = 0;
   maxprefh = 0;
   colsizes.clear();
   colsizes.resize(WChildren.size() , 0);
   colcount = 0;
   int x = 0;
   int y = 0;
   
   /// Rcsizes stores the relative position of all the widgets in the layout
   std::vector<WidgetBase*> wc = wchildren;
   for (unsigned int i = 0 ; i < wc.size() ; ++i) {
      WidgetBase* w = wc[i];
      if (!w) {continue;}
      int pw = w->PreferredWidth();
      int ph = w->PreferredHeight();
      if (pw < 1) {pw = defwidth;}
      if (ph < 1) {pw = defheight;}
      colsizes[colcount] = pw;
      colcount++;
      totalprefw += pw;
      if (ph > maxprefh) {maxprefh = ph;}
      rcsizes[i].SetArea(x , 0 , pw , ph);
      x += pw;
   }
   
   colwidthleft = InnerArea().W() - totalprefw;
   rowheightleft = InnerArea().H() - maxprefh;
   
   if ((colwidthleft < 0) || (rowheightleft < 0)) {
      overflow = true;
   }
   
   if (rules == BOX_EXPAND) {
      Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();
      t->Scale(InnerArea().W()/(float)totalprefw , InnerArea().H()/(float)maxprefh , 1.0);
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle r = rcsizes[i];
         double x = r.X();
         double y = r.Y();
         double w = r.W();
         double h = r.H();
         t.ApplyTransformation(&x , &y , 0);
         t.ApplyTransformation(&w , &h , 0);
         r->SetArea((int)x , (int)y , (int)w , (int)h);
      }
   }
   else if (rules == BOX_SPACE_BETWEEN) {
      int ncols = (int)colsizes.size();
      --ncols;
      if (ncols >= 1) {
         int percol = colwidthleft / ncols;
         int leftover = colwidthleft % ncols;
         int colcount = 0;
         int ox = 0;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            colcount++;
            ox += percol;
            if (leftover >= colcount) {
               ox++;
            }
            Rectangle* r = &rcsizes[i];
            r->MoveBy(ox , 0);
         }
      }
   }
   else if (rules == BOX_SPACE_EVEN) {
      int ncols = (int)colsizes.size();
      if (ncols > 0) {
         int percol = colwidthleft/2*ncols;
         int leftover = colwidthleft % (2*ncols);
         int leftoverleft = leftover/2;
         int leftoverright = leftover/2 + (leftover % 2 == 1)?1:0;
         int colcount = 0;
         int ox = percol + leftoverleft;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            colcount++;
            Rectangle* r = &rcsizes[i];
            r->MoveBy(ox , 0);
            ox += 2*percol + (leftoverleft >= colcount)?1:0 + (leftoverright >= colcount)?1:0;
         }
      }
   }
   else if (rules == BOX_ALIGN_ONLY) {
      HALIGNMENT hal = halign;
      if (anchor == BOX_ANCHOR_E) {
         if (hal == HALIGN_LEFT) {hal = HALIGN_RIGHT;}
         else if (hal == HALIGN_RIGHT) {hal = HALIGN_LEFT;}
      }
      int ox = 0;
      if (hal == HALIGN_CENTER) {
         ox = colwidthleft/2;
      }
      else if (hal == HALIGN_RIGHT) {
         ox = colwidthleft;
      }
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle* r = &rcsizes[i];
         r->MoveBy(ox , 0);
      }
   }
   
   /// Handle valign
   
   if (rules != BOX_EXPAND) {
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         int h = w->PreferredHeight();
         int oy = 0;
         if (valign == VALIGN_CENTER) {
            oy = (InnerArea().H() - h)/2;
         }
         else if (valign == VALIGN_BOTTOM) {
            oy = InnerArea().H() - h;
         }
         Rectangle* r = &rcsizes[i];
         r->MoveBy(0 , oy);
      }
      /// Handle flow direction
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle* r = &rcsizes[i];
         int x = r->X();
         int reflectx = InnerArea().W() - x;
         r->MoveBy(reflectx - x , 0);
      }
   }
   if (anchor == HBOX_ANCHOR_E) {
      // we need to reverse everything
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle* r = 
      }
   }
}



HBoxLayout::HBoxLayout(std::string classname , std::string objname) :
   BoxLayout(classname , objname),
   anchor(BOX_ANCHOR_W),
   rules(BOX_ALIGN_ONLY),
   totalprefw(0),
   maxprefh(0),
   colcount(0),
   colsizes(),
   colwidthleft(0),
   rowheightleft(0),
   defwidth(40),
   defheight(20),
   overflow(false)
{}



void HBoxLayout::SetAnchorPosition(BOX_ANCHOR_POINT p) {
   switch (p) {
   case HBOX_ANCHOR_W :
   case HBOX_ANCHOR_E :
      anchor = p;
      break;
   default :
      anchor = HBOX_ANCHOR_W;   
   }
   RecalcFlow();
   RepositionAllChildren();
}



int HBoxLayout::WidthLeft() {
   return colwidthleft;
}



int HBoxLayout::HeightLeft() {
   return rowheightleft;
}



bool HBoxLayout::OverflowWarning() {
   return overflow;
}



bool HBoxLayout::WidgetWouldOverflowLayout(WidgetBase* w) {
   if (!w) {return false;}
   int pw = w->PreferredWidth();
   int ph = w->PreferredHeight();
   return (pw > colwidthleft || ph > InnerArea().H());
}






