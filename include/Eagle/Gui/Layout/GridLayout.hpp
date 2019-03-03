
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
#ifndef GridLayout_HPP
#define GridLayout_HPP


#include "Eagle/Gui/Layout/Layout.hpp"



enum GRID_OPTIONS {
	GRID_ALIGNMENT_ONLY = 0,
	GRID_FILL_CELL = 1
};



class GridLayout : public Layout {
	
private :
	int ncols;
	int nrows;
	int size;
	
	int colwidth;
	int rowheight;
	
	int cellhpad;
	int cellvpad;
	
	int colhspace;
	int rowvspace;
	
	GRID_OPTIONS options;
	
protected :
	
   Rectangle RequestWidgetRectangle(WidgetBase* widget) const;
   Rectangle RequestWidgetRectangle(int slot_index) const;

   void CalculateGrid();

   /// WIDGETBASE callback
   virtual void OnAreaChanged();
   
public :
	GridLayout(int numcolumns = 0 , int numrows = 0 , std::string objclass = "GridLayout" , std::string objname = "Nemo");
	virtual ~GridLayout();

   /// LayoutBase
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   virtual void Resize(unsigned int nsize) {throw EagleException("Use ResizeGrid instead!");}

   /// GridLayout
	void ResizeGrid(int numcolumns , int numrows);/// Preserves top left of grid, frees widgets removed

	void SetGlobalPadding(unsigned int hpad , unsigned int vpad);
   void SetGlobalSpacing(unsigned int hspace , unsigned int vspace);

   Rectangle GetCellRectangle(int cellx , int celly);

   virtual std::string GetWidgetClassName() {return "GridLayout";}

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};


#endif // GridLayout_HPP




