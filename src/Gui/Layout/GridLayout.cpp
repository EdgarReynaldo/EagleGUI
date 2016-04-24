
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


#include "Eagle/Gui/Layout/GridLayout.hpp"



/** --------------------     Grid Layout     ------------------------- */



void GridLayout::RepositionAllChildren() {
	
	for (int y = 0 ; y < nrows ; ++y) {
		for (int x = 0 ; x < ncols ; ++x) {
			RepositionChild(x , y);
		}
	}
	
}



void GridLayout::RepositionChild(int slot) {
   RepositionChild(slot % ncols , slot / ncols);
}



void GridLayout::RepositionChild(int cellx , int celly) {
	
	int basex = area.InnerArea().X();
	int basey = area.InnerArea().Y();
	int index = celly*ncols + cellx;
	
	EAGLE_ASSERT((cellx >= 0) && (celly >= 0) && (celly*ncols + cellx < (int)wchildren.size()));
	
	WidgetBase* widget = wchildren[index];
	if (!widget) {return;}// WHAT DID YOU SEE HERE? HMM? NADA, EL CAPITAN!

   




	int widget_width = widget->Area().W();
	int widget_height = widget->Area().H();

	int xpos = basex + cellx*colwidth;
	int ypos = basey + celly*rowheight;
	
	switch (options) {
		case GRID_ALIGNMENT_ONLY :
			
			if (halign == HALIGN_LEFT) {
				xpos += cellhpad;
			}
			if (halign == HALIGN_CENTER) {
				xpos += colwidth/2 - widget_width/2;
			}
			if (halign == HALIGN_RIGHT) {
				xpos += colwidth - widget_width - cellhpad;
			}
			
			if (valign == VALIGN_TOP) {
				ypos += cellvpad;
			}
			if (valign == VALIGN_CENTER) {
				ypos += rowheight/2 - widget_height/2;
			}
			if (valign == VALIGN_BOTTOM) {
				ypos += rowheight - widget_height - cellvpad;
			}
			widget->SetDrawPos(xpos , ypos);
			break;
		case GRID_FILL_CELL :
			widget->SetArea(xpos + cellhpad , ypos + cellvpad , colwidth - 2*cellhpad , rowheight - 2*cellvpad);
			break;
	};
}



Rectangle GridLayout::RequestWidgetRectangle(WidgetBase* widget) {
   EAGLE_ASSERT(widget);
   int index = WidgetIndex(widget);
   EAGLE_ASSERT(index != -1);
   int col = index % nrows;
   int row = index / nrows;
   int basex = area.InnerArea().X() + col*(colwidth + colhgap);
   int basey = area.InnerArea().Y() + row*(rowheight + rowvgap);
   
   int wwidth = widget->OuterArea().W();
   int wheight = widget->OuterArea().H();
   
   switch (options) {
      case GRID_ALIGNMENT_ONLY :
      {
         switch (halign) {
            case HALIGN_LEFT :
               basex += cellhpad;
               break;
            case HALIGN_CENTER :
               basex += (colwidth - wwidth)/2;
               break;
            case HALIGN_RIGHT :
               basex += (colwidth - wwidth) - cellhpad;
               break;
            default : EAGLE_ASSERT(0);break;
         }
         switch (valign) {
            case VALIGN_TOP :
               basey += cellvpad;
               break;
            case VALIGN_CENTER :
               basey += (rowheight - wheight)/2;
               break;
            case VALIGN_BOTTOM :
               basey += (rowheight - wheight) - cellvpad;
               break;
            default : EAGLE_ASSERT(0);break;
         }
      }; break;

      case GRID_FILL_CELL :
         basex += cellhpad;
         basey += cellvpad;
         
         wwidth = colwidth - 2*cellhpad;
         if (wwidth < widget->MinWidth()) {wwidth = widget->MinWidth();}
         wheight = rowheight - 2*cellvpad;
         if (wheight < widget->MinHeight()) {wheight = widget->MinHeight();}
         break;
      default : EAGLE_ASSERT(0);break;
   }
   
   
   return Rectangle(basex,basey,wwidth,wheight);
}



void GridLayout::CalculateGrid() {
   colwidth = 0;
   rowheight = 0;
   int tw = area.InnerArea().W();
   int th = area.InnerArea().H();
   if (tw > 0 && ncols > 0) {
      colwidth = (tw - (ncols - 1)*colhgap)/ncols;
   }
   if (th > 0 && nrows > 0) {
      rowheight = (th - (nrows - 1)*rowvgap)/nrows;
   }
}



GridLayout::GridLayout() :
		ncols(0),
		nrows(0),
		size(0),
		colwidth(0),
		rowheight(0),
		cellhpad(0),
		cellvpad(0),
		colhgap(0),
		rowvgap(0),
		options(GRID_FILL_CELL)
{}



GridLayout::GridLayout(int numcolumns , int numrows) :
		ncols(0),
		nrows(0),
		size(0),
		colwidth(0),
		rowheight(0),
		cellhpad(0),
		cellvpad(0),
		options(GRID_FILL_CELL)
{
	ResizeGrid(numcolumns , numrows);
}



GridLayout::~GridLayout() {
   ClearLayout();
}



Rectangle GridLayout::RequestPosition   (WidgetBase* widget , int newx , int newy) {
   (void)newx;
   (void)newy;
   return RequestWidgetRectangle(widget);
}



Rectangle GridLayout::RequestSize       (WidgetBase* widget , int newwidth , int newheight) {
   (void)newwidth;
   (void)newheight;
   return RequestWidgetRectangle(widget);
}



Rectangle GridLayout::RequestArea       (WidgetBase* widget , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   return RequestWidgetRectangle(widget);
}



void GridLayout::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos , ypos);
   RepositionAllChildren();
}



void GridLayout::SetDrawDimensions(int width , int height) {
   WidgetBase::SetDrawDimensions(width , height);
   CalculateGrid();
   RepositionAllChildren();
}



void GridLayout::SetArea(int xpos , int ypos , int width , int height) {
   WidgetBase::SetArea(xpos , ypos , width , height);
   CalculateGrid();
   RepositionAllChildren();
}



void GridLayout::ResizeGrid(int numcolumns , int numrows) {
	int newsize = numcolumns*numrows;
	EAGLE_ASSERT((numcolumns >= 1) && (numrows >= 1));

   ClearLayout();
   wchildren.resize(newsize , 0);
	ncols = numcolumns;
	nrows = numrows;
	size = newsize;
	CalculateGrid();
}



void GridLayout::SetPadding(unsigned int hpad , unsigned int vpad) {
	cellhpad = hpad;
	cellvpad = vpad;
	RepositionAllChildren();
}



void GridLayout::SetGaps(unsigned int hgap , unsigned int vgap) {
   colhgap = hgap;
   rowvgap = vgap;
   if (ncols > 0) {
      colwidth = area.InnerArea().W() - (ncols-1)*hgap;
   }
   if (nrows > 0) {
      rowheight = area.InnerArea().H() - (nrows - 1)*vgap;
   }
   RepositionAllChildren();
}



Rectangle GridLayout::GetCellRectangle(int cellx , int celly) {
   int x = cellx*(colwidth + colhgap) + cellhpad;
   int y = celly*(rowheight + rowvgap) + cellvpad;
   int w = colwidth - 2*cellhpad;
   int h = rowheight - 2*cellvpad;
   return Rectangle(x,y,w,h);
}




