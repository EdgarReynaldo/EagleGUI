
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
	

/**
   virtual void RepositionAllChildren();
   virtual void RepositionChild(int slot);
	void RepositionChild(int cellx , int celly);
//*/

protected :
	
   Rectangle RequestWidgetRectangle(WidgetBase* widget) const;
   Rectangle RequestWidgetRectangle(int slot_index) const;

   void CalculateGrid();

public :
	GridLayout();
	GridLayout(string name);
	GridLayout(int numcolumns , int numrows);
	virtual ~GridLayout();

   /// WidgetBase
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

   /// LayoutBase
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;
   
   /// GridLayout
	void ResizeGrid(int numcolumns , int numrows);/// Preserves top left of grid, frees widgets removed

	void SetPadding(unsigned int hpad , unsigned int vpad);
   void SetSpacing(unsigned int hspace , unsigned int vspace);

   Rectangle GetCellRectangle(int cellx , int celly);

   virtual std::string GetWidgetClassName() {return "GridLayout object";}

};


#endif // GridLayout_HPP




