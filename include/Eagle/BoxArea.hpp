
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2022+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BoxArea.hpp
 * @brief A few simple storage classes for box like areas and np cells
 * 
 */



#ifndef BoxArea_HPP
#define BoxArea_HPP


#include <ostream>
#include "Eagle/Logging.hpp"


/**! @enum BOX_TYPE
 *   @brief Whether this box refers to the margin box, the border box, or the padding box
 */
enum BOX_TYPE {
   BOX_TYPE_MARGIN  = 0,///< This box type refers to the outer margin box
   BOX_TYPE_BORDER  = 1,///< This box type refers to the middle border box
   BOX_TYPE_PADDING = 2 ///< This box type refers to the inner padding box
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





#endif // BoxArea_HPP
