
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
 * @file WidgetArea.hpp
 * @brief This file contains the interfaces for the different types of widget areas used in Eagle
 * 
 * Include this header to get access to the @ref BOXAREA, @ref NPAREA, and @ref WIDGETAREA classes
 *
 * Eagle widgets use the CSS Box model. See here for details : https://www.w3schools.com/Css/css_boxmodel.asp
 * Note: A WIDGETAREA includes the margin, border, padding, and inner (client) areas
 * Note: A BOXAREA only defines the edges of a box, not the box itself, ie. left/right/top/bottom values
 */

#ifndef EagleGuiWidgetArea_HPP
#define EagleGuiWidgetArea_HPP



#include "Eagle/Area.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"



/**! @enum WAREA_TYPE
 *   @brief This enum specifies the type of widget area being referred to, whether it is
 *          the margin, the border, the padding, or the inner area of a widget.
 */
enum WAREA_TYPE {
   WAREA_TYPE_MARGIN  = 0,///< This widget area refers to the margin, the outermost box in the CSS model
   WAREA_TYPE_BORDER  = 1,///< This widget area refers to the border, the second outermost box in the CSS model
   WAREA_TYPE_PADDING = 2,///< This widget area refers to the padding, the third outermost box in the CSS model
   WAREA_TYPE_INNER   = 3 ///< This widget area refers to the client area, the innermost box in the CSS model
};


/**! @enum BOX_TYPE
 *   @brief Whether this box refers to the margin box, the border box, or the padding box
 */
enum BOX_TYPE {
   BOX_TYPE_MARGIN  = 0,///< This box type refers to the margin box
   BOX_TYPE_BORDER  = 1,///< This box type refers to the border box
   BOX_TYPE_PADDING = 2 ///< This box type refers to the padding box
};


/**! @enum CELL_AREA
 *   @brief This enum defines the 10 possible areas of a nine patch cell (left/center/right)/(top/vcenter/bottom) or outside
 */
enum CELL_AREA {
   CELL_AREA_OUTSIDE       = -1,///< This cell area is outside the nine patch
   CELL_AREA_TOPLEFT       = 0, ///< This is the top left cell
   CELL_AREA_TOPMIDDLE     = 1, ///< This is the top middle cell
   CELL_AREA_TOPRIGHT      = 2, ///< This is the top right cell
   CELL_AREA_MIDDLELEFT    = 3, ///< This is the middle left cell
   CELL_AREA_MIDDLEMIDDLE  = 4, ///< This is the middle middle cell
   CELL_AREA_MIDDLERIGHT   = 5, ///< This is the middle right cell
   CELL_AREA_BOTTOMLEFT    = 6, ///< This is the bottom leftcell
   CELL_AREA_BOTTOMMIDDLE  = 7, ///< This is the bottom middle cell
   CELL_AREA_BOTTOMRIGHT   = 8  ///< This is the bottom right cell
};


/**! @enum HCELL_AREA
 *   @brief Simple enum to specify the left, center, or right cell of a nine patch area
 */
enum HCELL_AREA {
	HCELL_LEFT   = 0,///< This refers to the left most horizontal cell
	HCELL_CENTER = 1,///< This refers to the center most horizontal cell
	HCELL_RIGHT  = 2 ///< This refers to the right most horizontal cell
};



/**! @enum VCELL_AREA
 *   @brief Simple enum to specify the top, vcenter, or bottom cell of a nine patch area
 */
enum VCELL_AREA {
	VCELL_TOP = 0,   ///< This refers to the top most vertical cell
	VCELL_CENTER = 1,///< This refers to the center most vertical cell
	VCELL_BOTTOM = 2 ///< This refers to the bottom most vertical cell
};



///< Translates a horizontal and vertical cell specification into a @ref CELL_AREA value
CELL_AREA GetCell(HCELL_AREA hcell , VCELL_AREA vcell);



/*! \class BOXAREA
 *  \brief The BOXAREA class holds the information needed to create a nine patch around a rectangle
 */

class BOXAREA {
public :
   int left;
   int right;
   int top;
   int bottom;
   
   /// Constructors
   
   ///< Default constructor that initializes everything to 0
   BOXAREA();

   ///< Side constructor - each side will have the value passed
   BOXAREA(int side);

   ///< Constructor for horizontal size and vertical size
   BOXAREA(int hsize , int vsize);

   ///< Explicit constructor that specifies left, right, top, and bottom size values
   BOXAREA(unsigned int l , unsigned int r , unsigned int t , unsigned int b);


   ///< Set the top, left, bottom, and right sizes
   void Set(unsigned int l , unsigned int r , unsigned int t , unsigned int b);

   inline int Width() const {return left + right;}///< Returns the width (left + right)
   inline int Height() const {return top + bottom;}///< Returns the height (top + bottom)

   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes the BOXAREA to the stream

   friend std::ostream& operator<<(std::ostream& os , const BOXAREA& ba);///< Stream output
};



/*! \class NPAREA
 *  \brief The NPAREA class holds the information needed to store the attributes of a nine patch area
 * 
 * The NPAREA class is to facilitate creating and painting nine patch areas
 */

class NPAREA {
public :
   Pos2I pos;
   int left;
   int width;
   int right;
   int top;
   int height;
   int bottom;
   
   NPAREA();///< Default empty constructor initializes to zero
   NPAREA(Rectangle area , BOXAREA box);///< Constructs the Nine patch area so that area is the OuterArea, and box defines the margins
   
   /// Paint functions

   void PaintOutsideSolid(EagleGraphicsContext* win , EagleColor c);///< Paint the margin of the nine patch a solid color
   void PaintOutsideRounded(EagleGraphicsContext* win , EagleColor c);///< Paint the margin of the nine patch a solid color that is rounded
   void PaintOutsideContrast(EagleGraphicsContext* win , EagleColor outer , EagleColor inner);///< Paints a two tone margin
   void PaintOutsideGradient(EagleGraphicsContext* win , EagleColor outer , EagleColor inner);///< Paints a gradient border
   
   /// Getters
   
   CELL_AREA GetCellArea(int xpos , int ypos) const;///< Returns the CELL_AREA specified by the passed position values
   
   Rectangle GetNPCell(HCELL_AREA hcell , VCELL_AREA vcell) const ;///< Gets the rectangle for a specified cell
   Rectangle GetNPCell(CELL_AREA cell) const ;///< Gets the rectangle for a specified cell

   Rectangle GetRow(VCELL_AREA vcell) const;///< Gets the row rectangle for a specified VCELL_AREA
   Rectangle GetColumn(HCELL_AREA hcell) const;///< Gets the column rectangle for a specified HCELL_AREA
   
   inline int Width() const {return left + width + right;}///< Returns the total width of the nine patch
   inline int Height() const {return top + height + bottom;}///< Returns the total height of the nine patch
   inline Rectangle Area() const {return Rectangle(pos.X() , pos.Y() , Width() , Height());}///< Returns the outer area of the nine patch

   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes the nine patch object to a stream

   friend std::ostream& operator<<(std::ostream& os , const NPAREA& np);///< Stream output

};



/*! \class WIDGETAREA
 *  \brief The WIDGETAREA class holds the information needed to implement the CSS Box model for Widgets
 */

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

   WIDGETAREA();///< Empty constructor

   ///< Constructs a widget area from an outer area and margin, border, and padding boxes */
   WIDGETAREA(Rectangle outerarea , BOXAREA marginbox = BOXAREA() , BOXAREA borderbox = BOXAREA() , BOXAREA paddingbox = BOXAREA());
   
   ///< Constructs a widget area from an inner area and surrounding margin, border, and padding boxes */
   WIDGETAREA(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox , Rectangle innerarea);

   
   WIDGETAREA& operator=(const WIDGETAREA& wa);///< Copies wa
   
   WIDGETAREA& MoveBy(Pos2I p);///< Moves the object by p.x , p.y
   WIDGETAREA MovedBy(Pos2I p);///< Returns a copy of the object, moved by p.x , p.y

   /// Setters
   
   ///< Sets the widget area from an outer area and margin, border, and padding boxes
   WIDGETAREA& SetBoxesContract(Rectangle outerarea , BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox);

   ///< Sets the widget area from an outer area and uniform margin, border, and padding sizes
   WIDGETAREA& SetBoxesContract(Rectangle outerarea , int marginsize , int bordersize , int paddingsize);
   
   ///< Sets the margin, border, and padding boxes used for this area, contracting from the outer area
   WIDGETAREA& SetBoxesContract(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox);

   ///< Sets a uniform margin, border, and padding size used for this area, contracting from the outer area
   WIDGETAREA& SetBoxesContract(int marginsize , int bordersize , int paddingsize);

   ///< Sets the area using margin, border, and padding boxes, expanding from the inner area
   WIDGETAREA& SetBoxesExpand(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox , Rectangle innerarea);

   ///< Sets the area using uniform margin, border, and padding sizes, expanding from the inner area
   WIDGETAREA& SetBoxesExpand(int marginsize , int bordersize , int paddingsize , Rectangle innerarea);

   ///< Sets the margin, border, and padding boxes, expanding from the stored inner area
   WIDGETAREA& SetBoxesExpand(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox);

   ///< Sets a uniform margin, border, and padding size, expanding from the stored inner area
   WIDGETAREA& SetBoxesExpand(int marginsize , int bordersize , int paddingsize);


   ///< Sets the specified box dimensions, contracting from the stored outer area
   WIDGETAREA& SetBoxAreaContractFromOuter(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b);

   ///< Sets the specified box, contracting from the stored outer area
   WIDGETAREA& SetBoxAreaContractFromOuter(BOX_TYPE box , BOXAREA b);

   ///< Sets the specified box dimensions, expanding from the stored inner area
   WIDGETAREA& SetBoxAreaExpandFromInner(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b);

   ///< Sets the specified box, expanding from the stored inner area
   WIDGETAREA& SetBoxAreaExpandFromInner(BOX_TYPE box , BOXAREA b);

   ///< Sets the outer area to oa, preserving margin, border, and padding boxes if possible
	WIDGETAREA& SetOuterArea(Rectangle oa);

   ///< Sets the inner area to ia, preserving margin, border, and padding boxes
	WIDGETAREA& SetInnerArea(Rectangle ia);

	///< Sets a new widget area
   WIDGETAREA& SetWidgetArea(const WIDGETAREA& wa);

   /// Getters
   
   NPAREA OuterNP() const;///< Returns the nine patch for the outer area (the margin)
   NPAREA BorderNP() const;///< Returns the nine patch for the border area
   NPAREA PaddingNP() const;///< Returns the nine patch for the padding area
   
   BOXAREA MarginBox()  const;///< Returns the margin box
   BOXAREA BorderBox()  const;///< Returns the border box
   BOXAREA PaddingBox() const;///< Returns the padding box
   
	Rectangle OuterArea()   const ;///< Returns the whole outer area
	Rectangle BorderArea()  const ;///< Returns the border area
	Rectangle PaddingArea() const ;///< Returns the padding area
	Rectangle InnerArea()   const ;///< Returns only the innermost area

   Rectangle CellBox(BOX_TYPE box , CELL_AREA area) const;///< Returns the rectangle of the specified box and cell
   Rectangle CellBox(BOX_TYPE box , VCELL_AREA vcell , HCELL_AREA hcell) const;///< Returns the rectangle of the specified box and cells

	int OuterAreaWidth()    const;///< Returns the outer area width
	int OuterAreaHeight()   const;///< Returns the outer area height
	int BorderAreaWidth()   const;///< Returns the border area width
	int BorderAreaHeight()  const;///< Returns the border area height
	int PaddingAreaWidth()  const;///< Returns the padding area width
	int PaddingAreaHeight() const;///< Returns the padding area height
	int InnerAreaWidth()    const;///< Returns the inner area width
	int InnerAreaHeight()   const;///< Returns the inner area height

	
	int MarginWidth()   const;///< Returns the combined size of the left and right margins
	int MarginHeight()  const;///< Returns the combined size of the top and bottom margins
	int BorderWidth()   const;///< Returns the combined size of the left and right borders
	int BorderHeight()  const;///< Returns the combined size of the top and bottom borders
	int PaddingWidth()  const;///< Returns the combined size of the left and right padding
	int PaddingHeight() const;///< Returns the combined size of the top and bottom padding

	Rectangle GetAreaRectangle(WAREA_TYPE atype) const;///< Returns the rectangle corresponding to atype
   BOXAREA GetAreaBox(BOX_TYPE btype)           const;///< Returns the BOXAREA specified by btype
	
	int LeftIndent()   const;///< Returns the sum of the left margin, border, and padding combined
	int RightIndent()  const;///< Returns the sum of the right margin, border, and padding combined
	int TopIndent()    const;///< Returns the sum of the top margin, border, and padding combined
	int BottomIndent() const;///< Returns the sum of the bottom margin, border, and padding combined
	
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes the widget area to a stream

   friend std::ostream& operator<<(std::ostream& os , const WIDGETAREA& wa);///< Stream output

};



std::ostream& operator<<(std::ostream& os , const WIDGETAREA& wa);///< Allows you to stream widget areas



typedef WIDGETAREA WidgetArea;



#endif // EagleGuiWidgetArea_HPP



