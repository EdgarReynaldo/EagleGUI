
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



#ifndef EagleGuiWidgetArea_HPP
#define EagleGuiWidgetArea_HPP


#include "Eagle/Area.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"


enum WAREA_TYPE {
   WAREA_TYPE_MARGIN  = 0,
   WAREA_TYPE_BORDER  = 1,
   WAREA_TYPE_PADDING = 2,
   WAREA_TYPE_INNER   = 3
};

enum BOX_TYPE {
   BOX_TYPE_MARGIN  = 0,
   BOX_TYPE_BORDER  = 1,
   BOX_TYPE_PADDING = 2
};

enum CELL_AREA {
   CELL_AREA_OUTSIDE      = -1,
   CELL_AREA_TOPLEFT      = 0,
   CELL_AREA_TOPMIDDLE    = 1,
   CELL_AREA_TOPRIGHT     = 2,
   CELL_AREA_MIDDLELEFT   = 3,
   CELL_AREA_MIDDLEMIDDLE = 4,
   CELL_AREA_MIDDLERIGHT  = 5,
   CELL_AREA_BOTTOMLEFT   = 6,
   CELL_AREA_BOTTOMMIDLE  = 7,
   CELL_AREA_BOTTOMRIGHT   = 8
};

enum HCELL_AREA {
	HCELL_LEFT = 0,
	HCELL_CENTER = 1,
	HCELL_RIGHT = 2
};
enum VCELL_AREA {
	VCELL_TOP = 0,
	VCELL_CENTER = 1,
	VCELL_BOTTOM = 2
};


class BOXAREA {
public :
   int left;
   int right;
   int top;
   int bottom;
   
   BOXAREA();
   
   BOXAREA(unsigned int l , unsigned int r , unsigned int t , unsigned int b);
   void Set(unsigned int l , unsigned int r , unsigned int t , unsigned int b);

   inline int Width() const {return left + right;}
   inline int Height() const {return top + bottom;}
};

class NPAREA {
public :
   Pos2I pos;
   int left;
   int width;
   int right;
   int top;
   int height;
   int bottom;
   
   NPAREA(Rectangle area , BOXAREA box);
   Rectangle GetNPCell(HCELL_AREA hcell , VCELL_AREA vcell) const ;

   Rectangle GetRow(VCELL_AREA vcell) const;
   Rectangle GetColumn(HCELL_AREA hcell) const;
   
   inline int Width() const {return left + width + right;}
   inline int Height() const {return top + height + bottom;}
   inline Rectangle Area() const {return Rectangle(pos.X() , pos.Y() , Width() , Height());}
};


#define WIDGETAREA WidgetArea2

class WIDGETAREA {

protected :

   Pos2I pos;
   BOXAREA margin;
   BOXAREA border;
   BOXAREA padding;
   int inner_width;
   int inner_height;

   void SetBoxArea(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b);
   void SetBoxArea(BOX_TYPE box , BOXAREA b);

public :

///   WIDGETAREA();
   WIDGETAREA() :
         pos(0,0),
         margin(0,0,0,0),
         border(0,0,0,0),
         padding(0,0,0,0),
         inner_width(0),
         inner_height(0)
   {}

   
   WIDGETAREA& operator=(const WIDGETAREA& wa);
   

   /// Setters
   
   void SetBoxAreaContractFromOuter(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b);
   void SetBoxAreaContractFromOuter(BOX_TYPE box , BOXAREA b);

   void SetBoxAreaExpandFromInner(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b);
   void SetBoxAreaExpandFromInner(BOX_TYPE box , BOXAREA b);

	void SetOuterArea(Rectangle oa);
	void SetInnerArea(Rectangle ia);

   void SetWidgetArea(const WIDGETAREA& wa);

   /// Getters
   
   NPAREA OuterNP() const;
   NPAREA BorderNP() const;
   NPAREA PaddingNP() const;
   
	Rectangle OuterArea()   const ;
	Rectangle BorderArea()  const ;
	Rectangle PaddingArea() const ;
	Rectangle InnerArea()   const ;

   Rectangle CellBox(BOX_TYPE box , CELL_AREA area) const;
   Rectangle CellBox(BOX_TYPE box , VCELL_AREA vcell , HCELL_AREA hcell) const;

	
	
	int OuterAreaWidth()    const;
	int OuterAreaHeight()   const;
	int BorderAreaWidth()   const;
	int BorderAreaHeight()  const;
	int PaddingAreaWidth()  const;
	int PaddingAreaHeight() const;
	int InnerAreaWidth()    const;
	int InnerAreaHeight()   const;

	
	int MarginWidth()   const;
	int MarginHeight()  const;
	int BorderWidth()   const;
	int BorderHeight()  const;
	int PaddingWidth()  const;
	int PaddingHeight() const;

	Rectangle GetAreaRectangle(WAREA_TYPE atype) const;
   BOXAREA GetAreaBox(BOX_TYPE btype) const;
	
	
	
	
	
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

   friend std::ostream& operator<<(std::ostream& os , const WIDGETAREA& wa);

};




#endif // EagleGuiWidgetArea_HPP






