
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
	
	int colhgap;
	int rowvgap;
	
	GRID_OPTIONS options;
	


   virtual void RepositionAllChildren();
   virtual void RepositionChild(int slot);
	void RepositionChild(int cellx , int celly);


protected :
	
   Rectangle RequestWidgetRectangle(WidgetBase* widget);

   void CalculateGrid();

///	void ResizeCell(int cell_width , int cell_height);

public :
	GridLayout();
	GridLayout(int numcolumns , int numrows);
	virtual ~GridLayout();
	


	/// LayoutBase
   virtual Rectangle RequestPosition   (WidgetBase* widget , int newx , int newy);
   virtual Rectangle RequestSize       (WidgetBase* widget , int newwidth , int newheight);
   virtual Rectangle RequestArea       (WidgetBase* widget , int newx , int newy , int newwidth , int newheight);

   /// WidgetBase
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);


	void ResizeGrid(int numcolumns , int numrows);/// Has to ClearLayout()...sorry use Insert Row/Column instead

	void SetPadding(unsigned int hpad , unsigned int vpad);
   void SetGaps(unsigned int hgap , unsigned int vgap);

   Rectangle GetCellRectangle(int cellx , int celly);

};


#endif // GridLayout_HPP




