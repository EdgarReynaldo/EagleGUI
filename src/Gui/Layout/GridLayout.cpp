
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Gui/Layout/GridLayout.hpp"

#include "Eagle/StringWork.hpp"




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
	
	if (cellx < 0 || cellx > ncols) {
      throw EagleError(StringPrintF("GridLayout::RepositionChild(%d,%d) : cellx (%d) is out of bounds [%d,%d)",
                                    cellx , celly , cellx , 0 , ncols));
	}
	if (celly < 0 || celly > nrows) {
      throw EagleError(StringPrintF("GridLayout::RepositionChild(%d,%d) : celly (%d) is out of bounds [%d,%d)",
                                    celly , celly , celly , 0 , nrows));
	}
	
	int index = celly*ncols + cellx;
	WidgetBase* widget = wchildren[index];
   if (widget) {
      widget->SetWidgetArea(RequestWidgetRectangle(widget));
   }
   
}
/**

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
			widget->SetWidgetArea(xpos + cellhpad , ypos + cellvpad , colwidth - 2*cellhpad , rowheight - 2*cellvpad);
			break;
	};

}
*/



Rectangle GridLayout::RequestWidgetRectangle(WidgetBase* widget) {
   EAGLE_ASSERT(widget);
   int index = WidgetIndex(widget);
   EAGLE_ASSERT(index != -1);
   int col = index % ncols;
   int row = index / ncols;
   int basex = area.InnerArea().X() + col*(colwidth + colhspace);
   int basey = area.InnerArea().Y() + row*(rowheight + rowvspace);
   
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
      colwidth = (tw - (ncols - 1)*colhspace)/ncols;
   }
   if (th > 0 && nrows > 0) {
      rowheight = (th - (nrows - 1)*rowvspace)/nrows;
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
		colhspace(0),
		rowvspace(0),
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
   ClearLayoutAndFreeWidgets();
   /// In case we go out of scope before our WidgetHandler
   DetachFromGui();
}


/**
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
//*/


void GridLayout::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos , ypos , width , height , notify_layout);
   CalculateGrid();
   RepositionAllChildren();
}



/// Resize grid will keep all widgets that overlap the new grid, but all others will be removed and possibly freed
void GridLayout::ResizeGrid(int newcolumns , int newrows) {
   if (newcolumns < 0) {newcolumns = 0;}
   if (newrows < 0) {newrows = 0;}

	const int newsize = newcolumns*newrows;

	if (newsize == 0) {
      ClearLayoutAndFreeWidgets();
	}
   else {
      std::vector<WidgetBase*> keep_widgets((unsigned int)newsize , (WidgetBase*)0);
      std::vector<WidgetBase*> removed_widgets;
      int oldindex = 0;
      int newindex = 0;
      const int oldrows = nrows;
      const int oldcols = ncols;
      int row = 0;
      int col = 0;
      for (row = 0 ; row < oldrows ; ++row) {
         /// If the current row is outside the new grid, remove the widgets in that row
         if (row >= newrows) {
            for (col = 0 ; col < oldcols ; ++col) {
               oldindex = row*oldcols + col;
               RemoveWidgetFromLayout(wchildren[oldindex]);/// Optionally frees widget and places NULL in its slot
            }
         }
         else {
            for (col = 0 ; col < oldcols ; ++col) {
               oldindex = row*oldcols + col;
               /// If the current column is outside the new grid, remove the widget in the current row and column
               if (col >= newcolumns) {
                  RemoveWidgetFromLayout(wchildren[oldindex]);/// Optionally frees widget and places NULL in its slot
               }
               else {
                  newindex = row*newcolumns + col;
                  /// Keep this widget
                  keep_widgets[newindex] = wchildren[oldindex];
               }
            }
         }
      }
      wchildren = keep_widgets;
      RepositionAllChildren();
   }
	
	ncols = newcolumns;
	nrows = newrows;
	size = newsize;
	CalculateGrid();
}



void GridLayout::SetPadding(unsigned int hpad , unsigned int vpad) {
	cellhpad = hpad;
	cellvpad = vpad;
	RepositionAllChildren();
}



void GridLayout::SetSpacing(unsigned int hspace , unsigned int vspace) {
   colhspace = hspace;
   rowvspace = vspace;
   if (ncols > 0) {
      colwidth = area.InnerArea().W() - (ncols-1)*hspace;
   }
   if (nrows > 0) {
      rowheight = area.InnerArea().H() - (nrows - 1)*vspace;
   }
   RepositionAllChildren();
}



Rectangle GridLayout::GetCellRectangle(int cellx , int celly) {
   int x = cellx*(colwidth + colhspace) + cellhpad;
   int y = celly*(rowheight + rowvspace) + cellvpad;
   int w = colwidth - 2*cellhpad;
   int h = rowheight - 2*cellvpad;
   return Rectangle(x,y,w,h);
}




