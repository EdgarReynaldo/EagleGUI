
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
#include "Eagle/Lib.hpp"


std::string PrintBoxAnchorPoint(BOX_ANCHOR_POINT p) {
   static const char* points[8] = {
      "HBOX_ANCHOR_W" ,//0
      "HBOX_ANCHOR_E" ,//1
      "VBOX_ANCHOR_N" ,//2
      "VBOX_ANCHOR_S" ,//3
      "FBOX_ANCHOR_NW",//4
      "FBOX_ANCHOR_NE",//5
      "FBOX_ANCHOR_SE",//6
      "FBOX_ANCHOR_SW" //7
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
   LayoutBase(classname , objname),
   spacing(BOX_ALIGN_ONLY),
   anchor(HBOX_ANCHOR_W),
   rcsizes(),
   overflow(false)
{}



Rectangle BoxLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   WidgetBase* w = LayoutBase::GetWidget(widget_slot);
   if (!w) {return BADRECTANGLE;}
   
   if ((newwidth != INT_MAX && newwidth != w->PreferredWidth()) || (newheight != INT_MAX && newheight != w->PreferredHeight())) {
      w->SetPreferredSize(newwidth , newheight);
      RecalcFlow();
   }
   
   Rectangle r = rcsizes[widget_slot];
   
   r.MoveBy(InnerArea().X() , InnerArea().Y());
   
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



void BoxLayout::SetBoxSpacing(BOX_SPACE_RULES r) {
   spacing = r;
   RecalcFlow();
   RepositionAllChildren();
}



void BoxLayout::SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align) {
   halign = h_align;
   valign = v_align;
   RecalcFlow();
   RepositionAllChildren();
}



bool BoxLayout::OverflowWarning() {
   return overflow;
}



/// ----------------------     HBOX       ---------------------------



void HBoxLayout::RecalcFlow() {

   /// Rcsizes stores the relative position of all the widgets in the layout
   rcsizes.clear();
   rcsizes.resize(wchildren.size() , BADRECTANGLE);
   
   overflow = false;
   totalprefw = 0;
   maxprefh = 0;
   colsizes.clear();
   colsizes.resize(WChildren().size() , 0);
   colcount = 0;
   int x = 0;
   int y = 0;
   
   /// Stack the widgets horizontally at y = 0, starting from x = 0
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
   
   
   if (spacing == BOX_EXPAND) {
      Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();
      t.Scale(InnerArea().W()/(float)totalprefw , InnerArea().H()/(float)maxprefh , 1.0);
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle r2 = rcsizes[i];
         Rectangle* r = &rcsizes[i];
         double x2 = r2.X();
         double y2 = r2.Y();
         double w2 = r2.W();
         double h2 = r2.H();
         t.ApplyTransformation(&x2 , &y2 , 0);
         t.ApplyTransformation(&w2 , &h2 , 0);
         r->SetArea((int)x2 , (int)y2 , (int)w2 , (int)h2);
      }
   }
   else if (spacing == BOX_ALIGN_ONLY) {
      HALIGNMENT hal = halign;
      if (anchor == HBOX_ANCHOR_E) {
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
   else if (spacing == BOX_SPACE_BETWEEN) {
      int ncols = (int)colsizes.size();
      --ncols;
      if (ncols >= 1) {
         int percol = colwidthleft / ncols;
         int leftover = colwidthleft % ncols;
         int count = 0;
         int ox = 0;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            Rectangle* r = &rcsizes[i];
            r->MoveBy(ox , 0);

            count++;
            ox += percol;
            if (leftover >= count) {
               ox++;
            }
         }
      }
   }
   else if (spacing == BOX_SPACE_EVEN) {
      int ncols = (int)colsizes.size();
      if (ncols > 0) {
         int percol = colwidthleft/ncols;
         int leftover = colwidthleft%ncols;
         int leftoverleft = leftover/2;
         int leftoverright = leftover - leftoverleft;
         int count = 0;
         int ox = percol/2;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            Rectangle* r = &rcsizes[i];
            r->MoveBy(ox , 0);
            count++;
            ox += percol + ((leftoverleft > count)?1:0) + ((leftoverright > count)?1:0);
         }
      }
   }
   
   /// Handle valign
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (!w) {continue;}
      int h = rcsizes[i].H();
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
   if (anchor == HBOX_ANCHOR_E) {
      // we need to reverse everything
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle r2 = rcsizes[i];
         Rectangle* r = &rcsizes[i];
         r->SetArea(InnerArea().W() - r2.X() - r2.W() , r2.Y() , r2.W() , r2.H());
      }
   }
}



HBoxLayout::HBoxLayout(std::string classname , std::string objname) :
   BoxLayout(classname , objname),
   totalprefw(0),
   maxprefh(0),
   colcount(0),
   colsizes(),
   colwidthleft(0),
   rowheightleft(0),
   defwidth(40),
   defheight(20)
{
   anchor = HBOX_ANCHOR_W;
}



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



bool HBoxLayout::WidgetWouldOverflowLayout(WidgetBase* w) {
   if (!w) {return false;}
   int pw = w->PreferredWidth();
   int ph = w->PreferredHeight();
   return (pw > colwidthleft || ph > InnerArea().H());
}




void VBoxLayout::RecalcFlow() {
   rcsizes.clear();
   rcsizes.resize(wchildren.size() , BADRECTANGLE);
   
   maxprefw = 0;
   totalprefh = 0;
   rowcount = 0;
   rowsizes.clear();
   rowsizes.resize(WChildren().size() , 0);
   colwidthleft = 0;
   rowheightleft = 0;
   overflow = false;
   
   int x = 0;
   int y = 0;
   
   /// rcsizes holds the relative position of all the sub widgets
   std::vector<WidgetBase*> wc = wchildren;
   for (unsigned int i = 0 ; i < wc.size() ; ++i) {
      WidgetBase* w = wc[i];
      if (!w) {continue;}
      int pw = w->PreferredWidth();
      int ph = w->PreferredHeight();
      if (pw < 1) {pw = defwidth;}
      if (ph < 1) {ph = defheight;}
      if (pw > maxprefw) {
         maxprefw = pw;
      }
      rcsizes[i].SetArea(x,y,pw,ph);
      totalprefh += ph;
      y += ph;
      rowsizes[rowcount] = ph;
      rowcount++;
   }
   
   colwidthleft = InnerArea().W() - maxprefw;
   rowheightleft = InnerArea().H() - totalprefh;
   
   if (colwidthleft < 0 || rowheightleft < 0) {
      overflow = true;
   }
   
   ///
   
   if (spacing == BOX_EXPAND) {
      Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();
      t.Scale(InnerArea().W()/(float)maxprefw , InnerArea().H()/(float)totalprefh , 1.0);
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle r2 = rcsizes[i];
         Rectangle* r = &rcsizes[i];
         double x2 = r2.X();
         double y2 = r2.Y();
         double w2 = r2.W();
         double h2 = r2.H();
         t.ApplyTransformation(&x2 , &y2 , 0);
         t.ApplyTransformation(&w2 , &h2 , 0);
         r->SetArea((int)x2 , (int)y2 , (int)w2 , (int)h2);
      }
   }
   else if (spacing == BOX_ALIGN_ONLY) {
      VALIGNMENT val = valign;
      if (anchor == VBOX_ANCHOR_S) {
         if (val == VALIGN_TOP) {val = VALIGN_BOTTOM;}
         else if (val == VALIGN_BOTTOM) {val = VALIGN_TOP;}
      }
      int oy = 0;
      if (val == VALIGN_CENTER) {
         oy = rowheightleft/2;
      }
      else if (val == VALIGN_BOTTOM) {
         oy = rowheightleft;
      }
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w  = wchildren[i];
         if (!w) {continue;}
         Rectangle* r = &rcsizes[i];
         r->MoveBy(0 , oy);
      }
   }
   else if (spacing == BOX_SPACE_BETWEEN) {
      int nrows = (int)rowsizes.size();
      --nrows;
      if (nrows >= 1) {
         int perrow = rowheightleft / nrows;
         int leftover = rowheightleft % nrows;
         rowcount = 0;
         int oy = 0;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            Rectangle* r = &rcsizes[i];
            r->MoveBy(0 , oy);

            rowcount++;
            oy += perrow;
            if (leftover >= rowcount) {
               oy++;
            }

         }
      }
   }
   else if (spacing == BOX_SPACE_EVEN) {
      int nrows = (int)rowsizes.size();
      if (nrows > 0) {
         int perrow = rowheightleft/nrows;
         int leftover = rowheightleft%nrows;
         int leftovertop = leftover/2;
         int leftoverbottom = leftover - leftovertop;
         rowcount = 0;
         int oy = perrow/2;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            rowcount++;
            Rectangle* r = &rcsizes[i];
            r->MoveBy(0 , oy);
            oy += perrow + ((leftovertop > rowcount)?1:0) + ((leftoverbottom > rowcount)?1:0);
         }
      }
   }
   
   /// Handle horizontal alignment
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (!w) {continue;}
      Rectangle r2 = rcsizes[i];
      Rectangle* r = &rcsizes[i];
      int ox = 0;
      if (halign == HALIGN_CENTER) {
         ox = (InnerArea().W() - r2.W())/2;
      }
      else if (halign == HALIGN_RIGHT) {
         ox = (InnerArea().W() - r2.W());
      }
      r->MoveBy(ox , 0);
   }
   
   if (anchor == VBOX_ANCHOR_S) {
      /// Mirror the flow from the other side in the same order
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle r2 = rcsizes[i];
         Rectangle* r = &rcsizes[i];
         r->SetArea(r2.X() , InnerArea().H() - r2.Y() - r2.H() , r2.W() , r2.H());
      }
   }
   
}



VBoxLayout::VBoxLayout(std::string classname , std::string objname) :
      BoxLayout(classname , objname),
      maxprefw(0),
      totalprefh(0),
      rowcount(0),
      rowsizes(),
      colwidthleft(0),
      rowheightleft(0),
      defwidth(20),
      defheight(40)
{
   anchor = VBOX_ANCHOR_N;
}



void VBoxLayout::SetAnchorPosition(BOX_ANCHOR_POINT p) {
   switch (p) {
   case VBOX_ANCHOR_N :
   case VBOX_ANCHOR_S :
      anchor = p;
      break;
   default :
      anchor = VBOX_ANCHOR_N;
   }
   RecalcFlow();
   RepositionAllChildren();
}



int VBoxLayout::WidthLeft() {
   return colwidthleft;
}



int VBoxLayout::HeightLeft() {
   return rowheightleft;
}



bool VBoxLayout::WidgetWouldOverflowLayout(WidgetBase* w) {
   if (!w) {return false;}
   return (w->PreferredWidth() > InnerArea().W() || w->PreferredHeight() > rowheightleft);
}



std::ostream& BoxLayout::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "BOX_SPACE_RULES = " << PrintBoxSpaceRule(spacing) << std::endl;
   os << indent << "BOX_ANCHOR_POINT = " << PrintBoxAnchorPoint(anchor) << std::endl;
   os << indent << "This layout has " << (overflow?"overflowed":"not overflowed") << " RCSIZES :" << std::endl;
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (!w) {continue;}
      os << indent << "rcsizes[" << i << "]=" << rcsizes[i] << std::endl;
   }
   LayoutBase::DescribeTo(os , indent);
   return os;
}









