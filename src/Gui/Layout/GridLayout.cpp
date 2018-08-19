
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


#include "Eagle/Gui/Layout/GridLayout.hpp"

#include "Eagle/StringWork.hpp"



/// --------------------     Grid Layout     -------------------------



Rectangle GridLayout::RequestWidgetRectangle(WidgetBase* widget) const {
   EAGLE_ASSERT(widget);
   int index = WidgetIndex(widget);
   return RequestWidgetRectangle(index);
}


Rectangle GridLayout::RequestWidgetRectangle(int slot_index) const {

   const WidgetBase* widget = GetWidget(slot_index);

   if (!widget) {
      return BADRECTANGLE;
   }

   int col = slot_index % ncols;
   int row = slot_index / ncols;

   int basex = InnerArea().X() + col*(colwidth + colhspace);
   int basey = InnerArea().Y() + row*(rowheight + rowvspace);
   
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
         if (wwidth < widget->AbsMinWidth()) {wwidth = widget->AbsMinWidth();}
         wheight = rowheight - 2*cellvpad;
         if (wheight < widget->AbsMinHeight()) {wheight = widget->AbsMinHeight();}
         break;
      default : EAGLE_ASSERT(0);break;
   }
   
   
   return Rectangle(basex,basey,wwidth,wheight);
}



void GridLayout::CalculateGrid() {
   colwidth = 0;
   rowheight = 0;
   int tw = InnerArea().W();
   int th = InnerArea().H();
   if (tw > 0 && ncols > 0) {
      colwidth = (tw - (ncols - 1)*colhspace)/ncols;
   }
   if (th > 0 && nrows > 0) {
      rowheight = (th - (nrows - 1)*rowvspace)/nrows;
   }
}



void GridLayout::OnAreaChanged() {
   CalculateGrid();
   RepositionAllChildren();
}



GridLayout::GridLayout(int numcolumns , int numrows , std::string objclass , std::string objname) :
      Layout(objclass , objname),
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
   (void)0;
}



Rectangle GridLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const {
   const WidgetBase* widget = GetWidget(widget_slot);
   if (!widget) {
      return BADRECTANGLE;
   }
   Rectangle cell_rect = RequestWidgetRectangle(widget_slot);
   newx = cell_rect.X();
   newy = cell_rect.Y();
   newwidth = cell_rect.W();
   newheight = cell_rect.H();
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   return Rectangle(newx , newy , newwidth , newheight);
}



/// Resize grid will keep all widgets that overlap the new grid, but all others will be removed
void GridLayout::ResizeGrid(int newcolumns , int newrows) {
   if (newcolumns < 0) {newcolumns = 0;}
   if (newrows < 0) {newrows = 0;}

	const int newsize = newcolumns*newrows;

	if (newsize == 0) {
      ClearWidgets();
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



void GridLayout::SetGlobalPadding(unsigned int hpad , unsigned int vpad) {
	cellhpad = hpad;
	cellvpad = vpad;
	RepositionAllChildren();
}



void GridLayout::SetGlobalSpacing(unsigned int hspace , unsigned int vspace) {
   colhspace = hspace;
   rowvspace = vspace;
   if (ncols > 0) {
      colwidth = InnerArea().W() - (ncols-1)*hspace;
   }
   if (nrows > 0) {
      rowheight = InnerArea().H() - (nrows - 1)*vspace;
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




