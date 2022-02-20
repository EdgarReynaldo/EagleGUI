
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

#include "Eagle/Position.hpp"
#include "Eagle/NinePatch.hpp"

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
   WIDGETAREA(const WIDGETAREA& wa);///< Copy constructor

   ///< Constructs a widget area from an outer area and margin, border, and padding boxes */
   WIDGETAREA(Rectangle outerarea , BOXAREA marginbox = BOXAREA() , BOXAREA borderbox = BOXAREA() , BOXAREA paddingbox = BOXAREA());
   
   ///< Constructs a widget area from an inner area and surrounding margin, border, and padding boxes */
   WIDGETAREA(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox , Rectangle innerarea);

   
   WIDGETAREA& operator=(const WIDGETAREA& wa);///< Copies wa
   
   WIDGETAREA& MoveBy(Pos2I p);///< Moves the object by p.x , p.y
   WIDGETAREA MovedBy(Pos2I p) const ;///< Returns a copy of the object, moved by p.x , p.y

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



