
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
 * @file BoxLayout.hpp
 * @brief Simple box layouts for Eagle
 */

 
 
#ifndef BoxLayout_HPP
#define BoxLayout_HPP


#include "Eagle/Gui/Layout/Layout.hpp"


/**! @enum BOX_ANCHOR_POINT
 *   @brief Where to anchor the flow from. The opposite corner is where it flows to
 */
enum BOX_ANCHOR_POINT {
   HBOX_ANCHOR_W = 1 << 1,/// Anchor the flow on the west side, for hbox 1
   HBOX_ANCHOR_E = 1 << 2,/// Anchor the flow on the east side, for hbox 2
   VBOX_ANCHOR_N = 1 << 3,/// Anchor the flow on the north side, for vbox 4
   VBOX_ANCHOR_S = 1 << 4,/// Anchor the flow on the south side, for vbox 8
	FBOX_ANCHOR_NW = HBOX_ANCHOR_W | VBOX_ANCHOR_N,///< Anchor the flow in the NW corner, for flow 5
	FBOX_ANCHOR_NE = HBOX_ANCHOR_E | VBOX_ANCHOR_N,///< Anchor the flow in the NE corner, for flow 6
	FBOX_ANCHOR_SE = HBOX_ANCHOR_E | VBOX_ANCHOR_S,///< Anchor the flow in the SE corner, for flow 10
	FBOX_ANCHOR_SW = HBOX_ANCHOR_W | VBOX_ANCHOR_S ///< Anchor the flow in the SW corner, for flow 9
};

std::string PrintBoxAnchorPoint(BOX_ANCHOR_POINT p);


/**! @enum BOX_SPACE_RULES
 *   @brief How empty space is used in a box layout
 */

enum BOX_SPACE_RULES {
   BOX_ALIGN_ONLY    = 0,///< Left over space is unused
   BOX_EXPAND        = 1,///< Left over space is given completely to widgets
   BOX_SPACE_BETWEEN = 2,///< Left over space is split up between each widget, pushing them out from the middle
   BOX_SPACE_EVEN    = 3 ///< Left over space is split up evenly between each widget on its outer edges
};


std::string PrintBoxSpaceRule(BOX_SPACE_RULES b);



class BoxLayout : public LayoutBase {
   
protected :
   
   std::vector<Rectangle> rcsizes;
   
   virtual void RecalcFlow()=0;
   
   
public :
   BoxLayout(std::string classname = "BoxLayout" , std::string objname = "Nemo");
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) override;

   virtual void Resize(unsigned int nsize) override;

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);
   virtual void InsertWidget(WidgetBase* w , int slot);
   
   
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p)=0;
   
   virtual int WidthLeft()=0;
   virtual int HeightLeft()=0;
   virtual bool OverflowWarning()=0;
   bool WidgetWouldOverflowLayout(WidgetBase* w)=0;
};



class HBoxLayout : public BoxLayout {
   
protected :
   BOX_ANCHOR_POINT anchor;
   BOX_SPACE_RULES rules;
   int totalprefw;
   int maxprefh;
   int colcount;
   std::vector<int> colsizes;
   int colwidthleft;
   int rowheightleft;
   int defwidth;
   int defheight;
   bool overflow;
   
   
   virtual void RecalcFlow();
   
public :
   HBoxLayout(std::string classname = "HBoxLayout" , std::string objname = "Nemo");
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p);
   
   virtual int WidthLeft();
   virtual int HeightLeft();

   virtual bool OverflowWarning();
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w);
};



class VBoxLayout : public BoxLayout {
protected :
   BOX_ANCHOR_POINT anchor;
   BOX_SPACE_RULES rules;
   int maxprefw;
   int totalprefh;
   int rowcount;
   std::vector<int> rowsizes;
   int colwidthleft;
   int rowheightleft;
   int defwidth;
   int defheight;
   bool overflow;
   
   virtual void RecalcFlow();
   
public :
   VBoxLayout(std::string classname = "VBoxLayout" , std::string objname = "Nemo");
   
   virtual void SetAnchorPosition(BOX_ANCHOR_POINT p);
   
   virtual int WidthLeft();
   virtual int HeightLeft();
   
   virtual bool OverflowWarning();
   virtual bool WidgetWouldOverflowLayout(WidgetBase* w);
};


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
   
   if (rules == BOX_EXPAND) {
      Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();
      t->Scale(InnerArea().W()/(float)maxprefw , InnerArea().H()/(float)totalprefh , 1.0);
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
            rowcount++;
            oy += perrow;
            if (leftover >= rowcount) {
               oy++;
            }
            Rectangle* r = &rcsizes[i];
            r->MoveBy(0 , oy);
         }
      }
   }
   else if (rules == BOX_SPACE_EVEN) {
      int nrows = (int)rowsizes.size();
      if (nrows > 0) {
         int perrow = rowheightleft/2*nrows;
         int leftover = rowheightleft % (2*nrows);
         int leftovertop = leftover/2;
         int leftoverbottom = leftover/2 + (leftover % 2 == 1)?1:0;
         rowcount = 0;
         int oy = perrow + leftovertop;
         for (unsigned int i = 0 ; i < rcsizes.size() ; ++i) {
            WidgetBase* w = wchildren[i];
            if (!w) {continue;}
            rowcount++;
            Rectangle* r = &rcsizes[i];
            r->MoveBy(0 , oy);
            oy += 2*perrow + (leftovertop >= colcount)?1:0 + (leftoverbottom >= colcount)?1:0;
         }
      }
   }
   else if (rules == BOX_ALIGN_ONLY) {
      VALIGNMENT val = valign;
      if (anchor == BOX_ANCHOR_S) {
         if (val == VALIGN_TOP) {val = VALIGN_BOTTOM;}
         else if (val == VALIGN_BOTTOM) {val = VALIGN_TOP;}
      }
      int oy = 0;
      if (val == VALIGN_CENTER) {
         oy = rowheightleft/2;
      }
      else if (val == VALIGN_BOTTOM) {
         oy = rowheightleftleft;
      }
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (!w) {continue;}
         Rectangle* r = &rcsizes[i];
         r->MoveBy(0 , oy);
      }
   }
   
   
}

VBoxLayout::VBoxLayout(std::string classname , std::string objname) :
      BoxLayout(classname , objname),
      anchor(VBOX_ANCHOR_N),
      rules(BOX_ALIGN_ONLY),
      maxprefw(0),
      totalprefh(0),
      rowcount(0),
      rowsizes(),
      colwidthleft(0),
      rowheightleft(0),
      defwidth(20),
      defheight(40),
      overflow(false)
{
   
}

void VBoxLayout::SetAnchorPosition(BOX_ANCHOR_POINT p) {
   switch (p) {
   case BOX_ANCHOR_N :
   case BOX_ANCHOR_S :
      anchor = p;
      break;
   default :
      anchor = BOX_ANCHOR_N;
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

bool VBoxLayout::OverflowWarning() {
   return overflow;
}
bool VBoxLayout::WidgetWouldOverflowLayout(WidgetBase* w) {
   if (!w) {return false;}
   return ((w->PreferredWidth() > InnerArea().W() || w->PreferredHeight() > rowheightleft);
}







#endif // BoxLayout_HPP




 
 
 
