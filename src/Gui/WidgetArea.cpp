
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Logging.hpp"

#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/StringWork.hpp"



WidgetArea::WidgetArea() : 
		mleft(0),
		mright(0),
		mtop(0),
		mbot(0),
		outer_area(),
		inner_area(),
		cell_images(),
		has_image_alpha(false)
{
	for (int celly = 0 ; celly < 3 ; ++celly) {
		for (int cellx = 0 ; cellx < 3 ; ++cellx) {
			cell_images[celly][cellx] = (EagleImage*)0;
		}
	}
}



WidgetArea::WidgetArea(const WidgetArea& a) :
   mleft(a.mleft),
   mright(a.mright),
   mtop(a.mtop),
   mbot(a.mbot),
   outer_area(a.outer_area),
   inner_area(a.inner_area),
   cell_images(),
   has_image_alpha(false)
{
   for (unsigned int i = 0 ; i < 3 ; ++i) {
      for (unsigned int j = 0 ; j < 3 ; ++j) {
         cell_images[i][j] = a.cell_images[i][j];
      }
   }
}



WidgetArea& WidgetArea::WidgetArea::operator=(const WidgetArea& a) {
   mleft = a.mleft;
   mright = a.mright;
   mtop = a.mtop;
   mbot = a.mbot;
   for (unsigned int i = 0 ; i < 3 ; ++i) {
      for (unsigned int j = 0 ; j < 3 ; ++j) {
         cell_images[i][j] = a.cell_images[i][j];
      }
   }
   has_image_alpha = a.has_image_alpha;
   return *this;
}



Rectangle WidgetArea::GetCellRectangle (MARGIN_CELL cell) const {
   return GetCellRectangle((MARGIN_HCELL)(cell%3) , (MARGIN_VCELL)(cell/3));
}



Rectangle WidgetArea::GetCellRectangle(MARGIN_HCELL hcell , MARGIN_VCELL vcell) const {

	int xoffsets[3] = {0 , mleft , mleft + inner_area.W()};
	int yoffsets[3] = {0 , mtop , mtop + inner_area.H()};
	int widths[3] = {mleft , inner_area.W() , mright};
	int heights[3] = {mtop , inner_area.H() , mbot};
	
	return Rectangle(outer_area.X() + xoffsets[hcell] , outer_area.Y() + yoffsets[vcell] , widths[hcell] , heights[vcell]);
	
}



EagleImage* WidgetArea::GetCellImage(MARGIN_HCELL hcell , MARGIN_VCELL vcell) const {
	///return cell_images[hcell][vcell];
	return cell_images[vcell][hcell];
}



void WidgetArea::Paint(EagleGraphicsContext* win , MARGIN_HCELL hcell , MARGIN_VCELL vcell, EagleColor col , int x , int y) const {
	Rectangle r = GetCellRectangle(hcell , vcell);
	r.MoveBy(x,y);
	r.Fill(win , col);
}



void WidgetArea::PaintAll(EagleGraphicsContext* win , EagleColor col , int x , int y) const {
	Rectangle r = outer_area;
	r.MoveBy(x,y);
	r.Fill(win , col);
}



void WidgetArea::PaintImage(EagleGraphicsContext* win , MARGIN_HCELL hcell , MARGIN_VCELL vcell , int x , int y , int flags) const {
	EagleImage* img = cell_images[vcell][hcell];
	if (!img) {return;}
	Rectangle r = GetCellRectangle(hcell , vcell);
	if (r.W() && r.H()) {
      win->DrawStretchedRegion(img , 0 , 0 , img->W() , img->H() , r.X() + x , r.Y() + y , r.W() , r.H() , flags);
	}
}



void WidgetArea::PaintImages(EagleGraphicsContext* win , int x , int y) const {
	for (int celly = 0 ; celly < 3 ; ++celly) {
		for (int cellx = 0 ; cellx < 3 ; ++cellx ) {
			PaintImage(win , (MARGIN_HCELL)cellx , (MARGIN_VCELL)celly , x , y , 0);
		}
	}
}



void WidgetArea::SetImage(EagleImage* img , MARGIN_HCELL hcell , MARGIN_VCELL vcell) {
	cell_images[vcell][hcell] = img;
}



void WidgetArea::SetImages(EagleImage* imgs[3][3]) {
   for (unsigned int x = 0 ; x < 3 ; ++x) {
      for (unsigned int y = 0 ; y < 3 ; ++y) {
         cell_images[x][y] = imgs[x][y];
      }
   }
}



void WidgetArea::SetImagesHaveAlpha(bool has_alpha) {
   has_image_alpha = has_alpha;
}



void WidgetArea::SetOuterPos(int xpos , int ypos) {
	outer_area.SetPos(xpos,ypos);
	inner_area.SetPos(xpos + mleft , ypos + mtop);
}



void WidgetArea::SetOuterDim(unsigned int width , unsigned int height) {
	unsigned int minwidth = (unsigned int)(mleft + mright);
	unsigned int minheight = (unsigned int)(mtop + mbot);
	if (width < minwidth) {width = minwidth;}
	if (height < minheight) {height = minheight;}
	outer_area.SetDimensions(width , height);
	inner_area.SetArea(outer_area.X() + mleft , outer_area.Y() + mtop , outer_area.W() - minwidth , outer_area.H() - minheight);
}



void WidgetArea::SetOuterArea(Rectangle r) {
	SetOuterArea(r.X() , r.Y() , r.W() , r.H());
}



void WidgetArea::SetOuterArea(int xpos , int ypos , unsigned int width , unsigned int height) {
	unsigned int minwidth = (unsigned int)(mleft + mright);
	unsigned int minheight = (unsigned int)(mtop + mbot);
	if (width < minwidth) {width = minwidth;}
	if (height < minheight) {height = minheight;}
	outer_area.SetArea(xpos , ypos , width , height);
	inner_area.SetArea(outer_area.X() + mleft , outer_area.Y() + mtop , outer_area.W() - minwidth , outer_area.H() - minheight);
}



void WidgetArea::SetInnerPos(int xpos , int ypos) {
	inner_area.SetPos(xpos , ypos);
	outer_area.SetPos(xpos - mleft , ypos - mtop);
}



void WidgetArea::SetInnerDim(unsigned int width , unsigned int height) {
//	if (width < 0) {width = 0;}
//	if (height < 0) {height = 0;}
	inner_area.SetDimensions(width , height);
	outer_area.SetDimensions(mleft + width + mright , mtop + height + mbot);
}



void WidgetArea::SetInnerArea(Rectangle r) {
	SetInnerArea(r.X() , r.Y() , r.W() , r.H());
}



void WidgetArea::SetInnerArea(int xpos , int ypos , unsigned int width , unsigned int height) {
//	if (width < 0) {width = 0;}
//	if (height < 0) {height = 0;}
	
	// resize
	inner_area.SetDimensions(width , height);
	outer_area.SetDimensions(mleft + width + mright , mtop + height + mbot);
	
	// reposition
	inner_area.SetPos(xpos , ypos);
	outer_area.SetPos(xpos - mleft , ypos - mtop);
}



void WidgetArea::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
	if (left < 0) {left = 0;}
	if (right < 0) {right = 0;}
	if (top < 0) {top = 0;}
	if (bottom < 0) {bottom = 0;}
	mleft = left;
	mright = right;
	mtop = top;
	mbot = bottom;
	
	/// expand out from inner area, changing the outer area
	outer_area.SetArea(inner_area.X() - left , inner_area.Y() - top , inner_area.W() + left + right , inner_area.H() + top + bottom);
}



void WidgetArea::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
	if (left < 0) {left = 0;}
	if (right < 0) {right = 0;}
	if (top < 0) {top = 0;}
	if (bottom < 0) {bottom = 0;}
	
	/// contract margins inward from the boundaries of the outer area, thus changing the inner area
	// check limits first
	
	int hmargin = left + right;
	int vmargin = top + bottom;
	
	if (hmargin > outer_area.W()) {
		// need to shrink horizontal margins, attempt to preserve original ratio of margins
		left = (outer_area.W()*left)/hmargin;
		right = (outer_area.W()*right)/hmargin;
		EAGLE_ASSERT((left + right) <= outer_area.W());
	}
	if (vmargin > outer_area.H()) {
		// need to shrink vertical margins, attempt to preserve original ratio of margins
		top = (outer_area.H()*top)/vmargin;
		bottom = (outer_area.H()*bottom)/vmargin;
		EAGLE_ASSERT((top + bottom) <= outer_area.H());
	}
	
	mleft = left;
	mright = right;
	mtop = top;
	mbot = bottom;
	
	// now set inner area
	inner_area.SetArea(outer_area.X() + left , outer_area.Y() + top , outer_area.W() - (left + right) , outer_area.H() - (top + bottom));
}



int WidgetArea::GetCellIndex(int px , int py) {
   int xoffset[3] = {0 , mleft , mleft + inner_area.W()};
   int yoffset[3] = {0 , mtop , mtop + inner_area.H()};
   int xindex = -1;
   int yindex = -1;
   
   int xpos = outer_area.X();
   int ypos = outer_area.Y();
   
   for (int i = 0 ; i < 3 ; ++i) {
      xoffset[i] += xpos;
      yoffset[i] += ypos;
   }
   
   if (px < xoffset[0]) {
      xindex = -1;
   }
   else if (px < xoffset[1]) {
      xindex = 0;
   }
   else if (px < xoffset[2]) {
      xindex = 1;
   }
   else if (px < xoffset[2] + mright) {
      xindex = 2;
   }
   else {
      xindex = -1;
   }

   if (py < yoffset[0]) {
      yindex = -1;
   }
   else if (py < yoffset[1]) {
      yindex = 0;
   }
   else if (py < yoffset[2]) {
      yindex = 1;
   }
   else if (py < yoffset[2] + mbot) {
      yindex = 2;
   }
   else {
      yindex = -1;
   }
   
   if (xindex != -1 && yindex != -1) {
      return yindex*3 + xindex;
   }
   
   return -1;
}



bool WidgetArea::Contains(int px , int py) {
   return outer_area.Contains(px,py);
}



bool WidgetArea::BorderContains(int px , int py) {
   return SideContains(px,py) || CornerContains(px,py);
}



bool WidgetArea::SideContains(int px , int py) {
   Rectangle left = GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_CENTER);
   Rectangle right = GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_CENTER);
   Rectangle top = GetCellRectangle(MARGIN_HCELL_CENTER , MARGIN_VCELL_TOP);
   Rectangle bottom = GetCellRectangle(MARGIN_HCELL_CENTER , MARGIN_VCELL_BOTTOM);
   return (left.Contains(px,py) ||
           right.Contains(px,py) ||
           top.Contains(px,py) ||
           bottom.Contains(px,py) );
}



bool WidgetArea::CornerContains(int px , int py) {
   Rectangle topleft = GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_TOP);
   Rectangle topright = GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_TOP);
   Rectangle bottomright = GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_BOTTOM);
   Rectangle bottomleft = GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_BOTTOM);
   return (topleft.Contains(px,py) ||
           topright.Contains(px,py) ||
           bottomright.Contains(px,py) ||
           bottomleft.Contains(px,py) );
}



std::ostream& WidgetArea::DescribeTo(std::ostream& os , Indenter indent) const {
   using std::endl;
   os << indent << "Outer Area : " << outer_area << endl;
   os << indent << StringPrintF("Margins (L,R,T,B) = %i,%i,%i,%i" , mleft , mright , mtop , mbot) << endl;
   os << indent << "Inner Area : " << inner_area << endl;
   return os;
}



std::ostream& operator<<(std::ostream& os , const WidgetArea& wa) {
   wa.DescribeTo(os);
   return os;
}



