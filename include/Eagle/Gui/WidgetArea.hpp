
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



#ifndef EagleGuiWidgetArea_HPP
#define EagleGuiWidgetArea_HPP


#include "Eagle/Area.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"



enum MARGIN_CELL {
   MARGIN_CELL_OUTSIDE      = -1,
   MARGIN_CELL_TOPLEFT      = 0,
   MARGIN_CELL_TOPMIDDLE    = 1,
   MARGIN_CELL_TOPRIGHT     = 2,
   MARGIN_CELL_MIDDLELEFT   = 3,
   MARGIN_CELL_MIDDLEMIDDLE = 4,
   MARGIN_CELL_MIDDLERIGHT  = 5,
   MARGIN_CELL_BOTTOMLEFT   = 6,
   MARGIN_CELL_BOTTOMMIDLE  = 7,
   MARGIN_CELL_BOTTOMRIGHT   = 8
};

enum MARGIN_HCELL {
	MARGIN_HCELL_LEFT = 0,
	MARGIN_HCELL_CENTER = 1,
	MARGIN_HCELL_RIGHT = 2
};
enum MARGIN_VCELL {
	MARGIN_VCELL_TOP = 0,
	MARGIN_VCELL_CENTER = 1,
	MARGIN_VCELL_BOTTOM = 2
};

class WidgetArea {

private :

	// margins on left right top and bottom - 9 cells
	int mleft;
	int mright;
	int mtop;
	int mbot;
	
	Rectangle outer_area;
	Rectangle inner_area;

	EagleImage* cell_images[3][3];
	
	bool has_image_alpha;

public :

	WidgetArea();
	WidgetArea(const WidgetArea& a);

   WidgetArea& operator=(const WidgetArea& a);

   void MoveBy(int dx , int dy);
	const Rectangle& OuterArea() const {return outer_area;}
	const Rectangle& InnerArea() const {return inner_area;}
///	Rectangle& OuterArea() {return outer_area;}
///	Rectangle& InnerArea() {return inner_area;}
	int       W() const {return outer_area.W();}
	int       H() const {return outer_area.H();}
	Rectangle   GetCellRectangle (MARGIN_CELL cell) const ;
	Rectangle   GetCellRectangle (MARGIN_HCELL hcell , MARGIN_VCELL vcell) const ;
	EagleImage* GetCellImage     (MARGIN_HCELL hcell , MARGIN_VCELL vcell) const ;
	
	void Paint(EagleGraphicsContext* win , MARGIN_HCELL hcell , MARGIN_VCELL vcell, EagleColor col , int x , int y) const ;
	void PaintAll(EagleGraphicsContext* win , EagleColor col , int x , int y) const;
	void PaintImage(EagleGraphicsContext* win , MARGIN_HCELL hcell , MARGIN_VCELL vcell , int x , int y , int flags) const;
	void PaintImages(EagleGraphicsContext* win , int x , int y) const;
	void SetImage(EagleImage* img , MARGIN_HCELL hcell , MARGIN_VCELL vcell);
	void SetImages(EagleImage* imgs[3][3]);
   void SetImagesHaveAlpha(bool has_alpha);

   /// Changes inner area, but preserves margins
	void SetOuterPos(int xpos , int ypos);
	void SetOuterDim(unsigned int width , unsigned int height);
	void SetOuterArea(Rectangle r);
	void SetOuterArea(int xpos , int ypos , unsigned int width , unsigned int height);

   void SetInnerDimensions(unsigned int width , unsigned int height);/// Changes outer area, preserves margins
	
	void SetRelativeInnerPosition(int xpos , int ypos);/// Changes margins, probably not very useful
	void SetRelativeInnerDimensions(unsigned int width , unsigned int height);/// Changes margins, probably not very useful
	void SetRelativeInnerArea(Rectangle r);/// Changes margins!!!!
	void SetRelativeInnerArea(int xpos , int ypos , unsigned int width , unsigned int height);/// changes margins!!!!
   void SetFractionalInnerArea(float fx , float fy , float fw , float fh);/// Sets inner area based on fractions of outer area's width and height
	
	/// Changes position and outer area!!!
	void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	void SetMarginsContractFromOuter(int left , int right , int top , int bottom);
	
	int MaxHMargin() const {return outer_area.W() - inner_area.W();}
	int MaxVMargin() const {return outer_area.H() - inner_area.H();}
	
	int MLeft()  const {return mleft;}
	int MRight() const {return mright;}
	int MTop()   const {return mtop;}
	int MBot()   const {return mbot;}

	int OuterX()   const {return outer_area.X();}
	int OuterY()   const {return outer_area.Y();}
	int OuterBRX() const {return outer_area.BRX();}
	int OuterBRY() const {return outer_area.BRY();}
	
	int InnerX()   const {return inner_area.X();}
	int InnerY()   const {return inner_area.Y();}
	int InnerBRX() const {return inner_area.BRX();}
	int InnerBRY() const {return inner_area.BRY();}
	
	bool HasImageAlpha() {return has_image_alpha;}

   // GetCellIndex returns 0,1,2 , 3,4,5 , 6,7,8 or -1 for outside
   int GetCellIndex(int px , int py);


   bool Contains(int px , int py);
   bool BorderContains(int px , int py);
   bool SideContains(int px , int py);
   bool CornerContains(int px , int py);


   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

   friend std::ostream& operator<<(std::ostream& os , const WidgetArea& wa);

};






#endif // EagleGuiWidgetArea_HPP
