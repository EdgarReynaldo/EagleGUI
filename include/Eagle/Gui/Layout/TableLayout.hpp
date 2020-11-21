
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
 * @file TableLayout.hpp
 * @brief The interface for table layouts
 *
 * Way better than bullshit html tables
 */

#ifndef RadioLayout_HPP
#define RadioLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Layout/DumbLayout.hpp"


#include "Eagle/TableVec.hpp"



/**! @class RadioLayout
 *   @brief A simple interface for a layout that turns a group of buttons into a radio button
 *
 *   RadioLayout is a decorator class for Layouts, you can use any layout you like with it, and it will handle
 *   forwarding layout functions to the real layout, as well as deselecting all non selected buttons when one is toggled.
 */

/**! @class TableCell
 *   @brief Used by TableBodyLayout to store properties of cells
 */

class TableCell {
public :
   
   bool align;
   HALIGNMENT halign;
   VALIGNMENT valign;
   
   unsigned int startcol;
   unsigned int startrow;
   unsigned int colspan;
   unsigned int rowspan;

   TableCell();
   TableCell(unsigned int col , unsigned int row , unsigned int ncols_to_span = 1 , unsigned int nrows_to_span = 1);
   
   TableCell& Align(HALIGNMENT horizontal_align = HALIGN_CENTER , VALIGNMENT vertical_align = VALIGN_CENTER);

   TableCell& DoNotAlign();
   
   TableCell& Place(unsigned int col , unsigned int row);

   TableCell& Size(unsigned int ncols , unsigned int nrows);
   
   unsigned int Area() {return colspan*rowspan;}

};

/**! @class TableBodyLayout
 *   @brief A layout used for the bodies of tables
 */

class TableBodyLayout : public LayoutBase {
   
   unsigned int nrows;
   unsigned int ncols;
   unsigned int nsize;
   
   std::vector<unsigned int> colwidths;
   std::vector<unsigned int> rowheights;
   
   TableVec2D<WidgetBase*> cell_table;
   std::map<WidgetBase* , TableCell> cell_map;
   
   

   Rectangle WidgetRectangle(unsigned int columnx , unsigned int rowy);
   Rectangle CellRectangle(unsigned int columnx , unsigned int rowy);
   Rectangle SlotRectangle(unsigned int columnx , unsigned int rowy);

   void PlaceWidgetInTable(WidgetBase* w , int slot , TableCell cell);

   TableCell NextFreeCell();
   
   void RefreshChildrenFromTable();

   
public :
   
//   TableBodyLayout(unsigned int numcols , unsigned int numrows);
   TableBodyLayout(unsigned int numcols , unsigned int numrows) :
         LayoutBase("TableBodyLayout" , "Body"),
         nrows(numrows),
         ncols(numcols),
         nsize(numcols*numrows),
         colwidths(),
         rowheights(),
         cell_table(0 , 0 , (WidgetBase*)0),
         cell_map()
   {
      Resize(numcols , numrows);
   }
   
   /// LayoutBase overrides
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

   void PlaceWidgetCell(WidgetBase* w , int slot , TableCell c);
   int AddWidgetCell(WidgetBase* w , TableCell c);
   
   /// Member functions
   void Resize(unsigned int numcols , unsigned int numrows);
   
   void InsertColumn(std::vector<WidgetBase*> colvec , unsigned int new_col_num);
   void InsertRow(std::vector<WidgetBase*> row , unsigned int new_row_num);
   
   void DeleteColumn(unsigned int old_col_num);
   void DeleteRow(unsigned int old_row_num);

   unsigned int NRows() {return nrows;}
   unsigned int NCols() {return ncols;}
   unsigned int Size() {return nsize;}
   
   
   
};



#endif // RadioLayout_HPP



