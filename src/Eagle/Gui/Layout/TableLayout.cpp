
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
 * @file TableLayout.cpp
 * @brief The implementations for the table layout classes
 */


#include "Eagle/Gui/Layout/TableLayout.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Gui/WidgetBase.hpp"




TableCell::TableCell() :
      align(false),
      halign(HALIGN_CENTER),
      valign(VALIGN_CENTER),
      startcol(0),
      startrow(0),
      colspan(0),
      rowspan(0)
{
   
}



TableCell::TableCell(unsigned int col , unsigned int row , unsigned int ncols_to_span , unsigned int nrows_to_span) :
      align(false),
      halign(HALIGN_CENTER),
      valign(VALIGN_CENTER),
      startcol(col),
      startrow(row),
      colspan(ncols_to_span),
      rowspan(nrows_to_span)
{
   
}



TableCell& TableCell::Align(HALIGNMENT horizontal_align , VALIGNMENT vertical_align) {
   align = true;
   halign = horizontal_align;
   valign = vertical_align;
   return *this;
}



TableCell& TableCell::DoNotAlign() {
   align = false;
   return *this;
}



TableCell& TableCell::Place(unsigned int col , unsigned int row) {
   startcol = col;
   startrow = row;
   return *this;
}



TableCell& TableCell::Size(unsigned int ncols , unsigned int nrows) {
   colspan = ncols;
   rowspan = nrows;
   return *this;
}



/// -----------------------------     TableBodyLayout     -------------------------------



Rectangle TableBodyLayout::WidgetRectangle(unsigned int columnx , unsigned int rowy) {
   Rectangle rcell = CellRectangle(columnx , rowy);
   
   WidgetBase* widget = GetWidget(rowy*ncols + columnx);
   
   if (!widget) {
      return BADRECTANGLE;
   }
   
   const TableCell* tcell = &cell_map[widget];
   
   if (tcell->align) {
      HALIGNMENT halignc = tcell->halign;
      VALIGNMENT valignc = tcell->valign;
      int wx = rcell.X();
      int wy = rcell.Y();
      int ww = widget->OuterArea().W();
      int wh = widget->OuterArea().H();
      if (halignc == HALIGN_CENTER) {
         wx += (rcell.W() - ww)/2;
      }
      else if (halignc == HALIGN_RIGHT) {
         wx += rcell.W() - ww;
      }
      if (valignc == VALIGN_CENTER) {
         wy += (rcell.H() - wh)/2;   
      }
      else if (valignc == VALIGN_BOTTOM) {
         wy += rcell.H() - wh;
      }
      Rectangle wrect(wx,wy,ww,wh);
      return wrect;
   }
   else {
      /// Fill cell span
      return rcell;
   }
   return BADRECTANGLE;/// Not reached
}



Rectangle TableBodyLayout::CellRectangle(unsigned int columnx , unsigned int rowy) {
   if (columnx >= colwidths.size()) {
      return BADRECTANGLE;
   }
   if (rowy >= rowheights.size()) {
      return BADRECTANGLE;
   }
   WidgetBase* widget = GetWidget(rowy*ncols + columnx);
   
   if (widget) {
      const TableCell* cell = &cell_map[widget];
      unsigned int spanwidth = 0;
      unsigned int spanheight = 0;
      unsigned int xpos = InnerArea().X();
      unsigned int ypos = InnerArea().Y();

      for (unsigned int col = 1 ; col < cell->startcol ; ++col) {
         xpos += colwidths[col];
      }
      for (unsigned int row = 0 ; row < cell->startrow ; ++row) {
         ypos += rowheights[row];
      }

      for (unsigned int col = 0 ; col < cell->colspan ; ++col) {
         spanwidth += colwidths[col + cell->startcol];
      }
      for (unsigned int row = 0 ; row < cell->rowspan ; ++row) {
         spanheight += rowheights[row + cell->startrow];
      }

      Rectangle r(xpos , ypos , spanwidth , spanheight);
      
      return r;
   }
   return SlotRectangle(columnx , rowy);
}



Rectangle TableBodyLayout::SlotRectangle(unsigned int columnx , unsigned int rowy) {
   if (columnx >= colwidths.size()) {
      return BADRECTANGLE;
   }
   if (rowy >= rowheights.size()) {
      return BADRECTANGLE;
   }
   Rectangle r(InnerArea().X() , InnerArea().Y() , colwidths[columnx] , rowheights[rowy]);
   for (unsigned int x = 1 ; x <= columnx ; ++x) {
      r.MoveBy(colwidths[x - 1] , 0);
   }
   for (unsigned int y = 1 ; y <= rowy ; ++y) {
      r.MoveBy(0 , rowheights[y - 1]);
   }
   return r;
}






void TableBodyLayout::PlaceWidgetInTable(WidgetBase* w , int slot , TableCell cell) {
   if (slot >= (int)wchildren.size()) {
      throw EagleException(StringPrintF("TableBodyLayout::PlaceWidgetInTable() : slot (%d) out of range! (%lu)\n" , slot , wchildren.size()));
   }
   
   if (cell.Area() == 0) {
      throw EagleException("TableBodyLayoyut::PlaceWidgetInTable() : cell area is zero!\n");
   }
   
   Rectangle rtable(0,0,ncols,nrows);
   Rectangle rcell(cell.startcol , cell.startrow , cell.colspan , cell.rowspan);
   if (!rtable.Contains(rcell)) {
      throw EagleException("TableBodyLayout::PlaceWidgetInTable() : cell doesn't fit the table!\n");
   }
   
   WidgetBase* old = GetWidget(slot);
   if (old) {
      TableCell tcell = cell_map[old];
      for (unsigned int r = tcell.startrow ; r < tcell.startrow + tcell.rowspan ; ++r) {
         for (unsigned int c = tcell.startcol ; c < tcell.startcol + tcell.colspan ; ++c) {
            cell_table[r][c] = 0;
         }
      }
      cell_map.erase(cell_map.find(old));
   }
   if (w) {
      TableCell tcell = cell_map[w];
      for (unsigned int r = tcell.startrow ; r < tcell.startrow + tcell.rowspan ; ++r) {
         for (unsigned int c = tcell.startcol ; c < tcell.startcol + tcell.colspan ; ++c) {
            cell_table[r][c] = w;
         }
      }
      cell_map[w] = cell;
   }

   LayoutBase::PlaceWidget(w,slot);
}



TableCell TableBodyLayout::NextFreeCell() {
   
   /// Start with blank table
   TableVec2D<TableCell> celltable(nrows , ncols , TableCell());
   for (unsigned int i = 0 ; i < nrows ; ++i) {
      for (unsigned int j = 0 ; j < ncols ; ++j) {
         celltable[i][j].Place(i,j).Size(0,0);
      }
   }
   
   /// Remove used cells
   for (std::map<WidgetBase* , TableCell>::iterator it = cell_map.begin() ; it != cell_map.end() ; ++it) {
      const TableCell& cell = it->second;
      for (unsigned int i = cell.startrow ; i < cell.startrow + cell.rowspan ; ++i) {
         for (unsigned int j = cell.startcol ; j < cell.startcol + cell.colspan ; ++j) {
            celltable[i][j].Size(1,1);
         }
      }
   }
   
   /// Find first unused cell
   for (unsigned int i = 0 ; i < celltable.NRows() ; ++i) {
      for (unsigned int j = 0 ; j < celltable.NCols() ; ++j) {
         if (celltable[i][j].Area() == 0) {
            return celltable[i][j];
         }
      }
   }
   return TableCell();
}



void TableBodyLayout::RefreshChildrenFromTable() {
   wchildren.clear();
   wchildren.resize(nsize);
   unsigned int i = 0;
   while (i < nsize) {
      wchildren[i] = cell_table[i/ncols][i%ncols];
      ++i;
   }
}



Rectangle TableBodyLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   WidgetBase* widget = GetWidget(widget_slot);
   if (!widget) {
      return BADRECTANGLE;
   }
   Rectangle wrect = WidgetRectangle(widget_slot%ncols , widget_slot/ncols);
   if (wrect != BADRECTANGLE) {
      if (newwidth != INT_MAX || newheight != INT_MAX) {
         ///< Adjust the widget rectangle for the new width and height, newx and newy are ignored
         
         const TableCell* tcell = &cell_map[widget];
         
         int ox = 0;
         int oy = 0;
         int dw = 0;
         int dh = 0;
         
         if (newwidth == INT_MAX) {
            dw = newwidth - widget->OuterArea().W();
            newwidth = widget->OuterArea().W();
         }
         if (newheight == INT_MAX) {
            dh = newheight - widget->OuterArea().H();
            newheight = widget->OuterArea().H();
         }
         
         if (tcell->align) {
            if (tcell->halign == HALIGN_CENTER) {
               /// We need to center the widget based on the new width
               ox += -dw/2;
            }
            else if (tcell->halign == HALIGN_RIGHT) {
               ox += -dw;
            }
            if (tcell->valign == VALIGN_CENTER) {
               /// We need to center the widget based on the new width
               oy += -dh/2;
            }
            else if (tcell->valign == VALIGN_BOTTOM) {
               oy += -dh;
            }
            wrect.MoveBy(ox,oy);
            wrect.SetDimensions(newwidth , newheight);
         }
      }
   }
   return wrect;
}



void TableBodyLayout::PlaceWidget(WidgetBase* w , int slot) {
   PlaceWidgetCell(w , slot , TableCell(slot%ncols , slot/ncols , 1 , 1));
}



int TableBodyLayout::AddWidget(WidgetBase* w) {
   TableCell nextcell = NextFreeCell();
   if (nextcell.Area() == 0) {
      return -1;///< No free cells
   }
   return AddWidgetCell(w , nextcell);
}



void TableBodyLayout::PlaceWidgetCell(WidgetBase* w , int slot , TableCell c) {
   PlaceWidgetInTable(w,slot,c);
}



int TableBodyLayout::AddWidgetCell(WidgetBase* w , TableCell c) {
   int slot = NextFreeSlot();
   if (slot == -1) {return -1;}///< No free slots
   PlaceWidgetInTable(w,slot,c);
   return slot;
}



void TableBodyLayout::Resize(unsigned int numcols , unsigned int numrows) {
   LayoutBase::Resize(numcols*numrows);
   colwidths.resize(ncols , 0);
   rowheights.resize(nrows , 0);
   cell_table.Resize(numcols , numrows , (WidgetBase*)0);
   ncols = numcols;
   nrows = numrows;
   nsize = ncols*nrows;
   RefreshChildrenFromTable();
   
   RepositionAllChildren();
}



void TableBodyLayout::InsertColumn(std::vector<WidgetBase*> colvec , unsigned int new_col_num) {

   cell_table.InsertColumn(std::vector<WidgetBase*>(colvec.size() , 0) , new_col_num);
   
   unsigned int row = 0;
   while (row < nrows) {
      PlaceWidget(colvec[row] , row*ncols + new_col_num);
      ++row;
   }
   ++ncols;
   nsize = ncols*nrows;
   RefreshChildrenFromTable();
}



void TableBodyLayout::InsertRow(std::vector<WidgetBase*> row , unsigned int new_row_num) {

   cell_table.InsertRow(std::vector<WidgetBase*>(ncols , (WidgetBase*)0) , new_row_num);
   unsigned int col = 0;
   while (col < ncols) {
      PlaceWidget(row[col] , new_row_num*ncols + col);
      ++col;
   }
   ++nrows;
   nsize = ncols*nrows;
   RefreshChildrenFromTable();
}



void TableBodyLayout::DeleteColumn(unsigned int old_col_num) {
   for (unsigned int row = 0 ; row < nrows ; ++row) {
      PlaceWidget(0 , row*ncols + old_col_num);
   }
   cell_table.DeleteColumn(old_col_num);
   --ncols;
   nsize = ncols*nrows;
   RefreshChildrenFromTable();
}



void TableBodyLayout::DeleteRow(unsigned int old_row_num) {
   for (unsigned int col = 0 ; col < ncols ; ++col) {
      PlaceWidget(0 , old_row_num*ncols + col);
   }
   cell_table.DeleteRow(old_row_num);
   --nrows;
   nsize = ncols*nrows;
   RefreshChildrenFromTable();
}






