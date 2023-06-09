
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
 * @file NinePatch.hpp
 * @brief A simple interface and class for working with nine patch images, like buttons
 */

#ifndef NinePatch_HPP
#define NinePatch_HPP


#include "Position.hpp"
#include "Eagle/Area.hpp"
#include "Eagle/BoxArea.hpp"



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
   
   
   /// Getters
   
   CELL_AREA GetCellArea(int xpos , int ypos) const;///< Returns the CELL_AREA specified by the passed position values
   
   Rectangle GetNPCell(HCELL_AREA hcell , VCELL_AREA vcell) const ;///< Gets the rectangle for a specified cell
   Rectangle GetNPCell(CELL_AREA cell) const ;///< Gets the rectangle for a specified cell

   Rectangle GetRow(VCELL_AREA vcell) const;///< Gets the row rectangle for a specified VCELL_AREA
   Rectangle GetColumn(HCELL_AREA hcell) const;///< Gets the column rectangle for a specified HCELL_AREA
   
   inline int Width() const {return left + width + right;}///< Returns the total width of the nine patch
   inline int Height() const {return top + height + bottom;}///< Returns the total height of the nine patch
   inline Rectangle Area() const {return Rectangle(pos.X() , pos.Y() , Width() , Height());}///< Returns the outer area of the nine patch
   inline Rectangle OuterArea() const {return Area();}
   inline Rectangle InnerArea() const {return GetNPCell(HCELL_CENTER , VCELL_CENTER);}
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes the nine patch object to a stream

   friend std::ostream& operator<<(std::ostream& os , const NPAREA& np);///< Stream output

};


/*! @class NPRAREA
 *  @brief NPRAREA holds a relative nine patch area, using doubles as the relative width and height of the center cell
 *  
 *  The NPRAREA class allows you to create a normal nine patch area using relative values. Retrive the @ref NPAREA using @ref NPRAREA::GetNP<> .
 *  The widths of the left and right and the heights of the top and bottom cells are determined by halving what is left from the center.
 *  Minimum NPAREA dimensions will be 3x3, so creating a nine patch image will always use at least 1x1 sub bitmaps.
*/


class NPRAREA {
protected :
   Rectangle outer;
   double rcw;///< Relative center width
   double rch;///< Relative center height
   NPAREA nparea;///< Storage for precalculated NPAREA using relative values
   
   void ResetNP();
   
   
public :
   
   NPRAREA(Rectangle outer_area , double rel_center_width , double rel_center_height);///< Create a relative nine patch using a rectangle and relative center width and height
   void SetCenterArea(double rel_center_width , double rel_center_height);///< Set the relative center width and height of the NPRAREA
   void SetArea(const Rectangle& outer_area);///< Change the area of the NPRAREA without changing the relative widths and heights of the cells
   
   NPAREA GetNP();///< Get the stored pre-calculated NPAREA for this NPRAREA
};



/// Paint functions

class EagleGraphicsContext;

void PaintOutsideSolid(EagleGraphicsContext* win , NPAREA np , EagleColor c);///< Paint the margin of the nine patch a solid color
void PaintOutsideRounded(EagleGraphicsContext* win , NPAREA np , EagleColor c);///< Paint the margin of the nine patch a solid color that is rounded
void PaintOutsideContrast(EagleGraphicsContext* win , NPAREA np , EagleColor outer , EagleColor inner);///< Paints a two tone margin
void PaintOutsideGradient(EagleGraphicsContext* win , NPAREA np , EagleColor outer , EagleColor inner);///< Paints a gradient border


class EagleImage;

/**! @class NinePatch
 *   @brief A class to store nine patch images
 *   It is recommended to make your ninepatch images entirely black and white so they can later be tinted any color. Regular color works too.
 */

class NinePatch {

public :
   friend EagleGraphicsContext;

   EagleGraphicsContext* window;///< Our graphics context window
   EagleImage* srcs[3][3];///< EagleImage array, row major from top to bottom, left to right
   EagleImage* srcimage;
   NPAREA srcarea;
   
   
   
   bool Create(EagleGraphicsContext* win , EagleImage* src , NPAREA nparea);

   EagleImage* operator[](CELL_AREA carea) const;///< Get a direct pointer to image 0-8

   NinePatch();

   
public :
   ~NinePatch();///< Public destructor, frees any stored images
   
   void Free();///< Free early if desired, will be freed on destruction

   NinePatch(const NinePatch& np);///< Copy construct a nine patch
   NinePatch& operator=(const NinePatch& np);///< Assign one nine patch to another

   bool Valid() {return srcs[0][0];}
};

/**! @fn DrawNinePatch <
 *   @brief Draws the nine patch to the specified widget area using the specified optional offset
 */



#endif // NinePatch_HPP



