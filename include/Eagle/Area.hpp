
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
 * @file Area.hpp
 * @brief This file contains the interface for the different classes that store a geometric area
 * 
 * All concrete classes inherit from @ref AreaBase, for a common interface
 * 
 */

#ifndef Area_H
#define Area_H


#ifdef __GL_H__
   #warning Eagle::Rectangle has issues with OpenGL headers. Include Eagle headers first.
#endif

#include "Eagle/Color.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/Position.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <cmath>



/// -----------------------------     AreaBase class     --------------------------------


/// Forward declarations

class Rectangle;
class EagleGraphicsContext;



/*! @class AreaBase
 *  @brief The base class for all area objects
 *
 *
 */

class AreaBase {

protected :

public :
   AreaBase() {}
   virtual ~AreaBase() {}
   
   ///< Pure virtual function that indicates whether an area contains a point
   virtual bool Contains(int xpos , int ypos) const =0;

   ///< Pure virtual function that moves the area by dx and dy
   virtual AreaBase& MoveBy(int dx , int dy)=0;

   ///< Pure virtual function that draws an outline of the shape
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const =0;

   ///< Pure virtual function that draws an outline shaped with thickness
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const =0;

   ///< Pure virtual function that fills a shape with color
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const =0;

   ///< Pure virtual function that creates a vector of NEW Rectangles, destroy with DestroyBoundingRectangles
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const =0;///< TODO : Use objects instead of pointers?

   /// Pure virtual function to create a copy of the current shape and class
   virtual AreaBase* Clone() const =0;// returns a newed copy of itself
   
   ///< Virtual function to describe an area to a stream
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   ///< Overloaded stream operator
   friend std::ostream& operator<<(std::ostream& os , const AreaBase& area); 
};



///< Destroys the rectangles contained in the vector and empties it
void DestroyBoundingRectangles(std::vector<Rectangle*>& vrect);



/// ----------------------------     AreaGroup class      --------------------------------


/*! @class AreaGroup
 *  @brief This class allows you to conglomerate areas into a group that then acts like a single area
 *
 * TODO : Use shared pointers instead of a delete map?
 */

class AreaGroup : public AreaBase {
   
protected :
   std::vector<AreaBase*> areas;
   std::map<AreaBase* , bool> del_map;
   
   
public :
   AreaGroup();
   ~AreaGroup();
   
   AreaGroup& operator=(const AreaGroup& a) {
      (void)a;
      
      /// Must make a deep copy of all the areas
      ClearAreas();
      
      for (unsigned int i = 0 ; i < a.areas.size() ; ++i) {
         areas.push_back(a.areas[i]->Clone());
         del_map[areas.back()] = true;
      }
      
      
      throw EagleException("TODO : Implement copy constructor for AreaGroup class.");
   }
   
   ///< Overloads from @ref AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;


   /// Member functions
   void AddArea(AreaBase* a , bool delete_area = true);///< Add an area to the group
   void RemoveArea(AreaBase* a);///< Remove an area
   void ClearAreas();///< Clear all areas
   void CloneAreaGroup(const AreaGroup& ag);///< Helper function to clone an area group
   void AdoptAreaGroup(AreaGroup& ag);///< Takes ownership of ag's areas and clears ag of data
   
   int size() {return areas.size();}///< The area count

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Output to a stream, with optional indentation
};




/// -------------------------------------     Line class       -----------------------------------------


/**
class Line : public AreaBase {
protected :
   Pos2F p1;
   Pos2F p2;
   double thickness;
   
public :
   /// TODO : Implement line class?

///   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};

*/

/// -------------------------------------     Triangle class     ----------------------------------------



class Triangle : public AreaBase {

   
protected :
   Pos2D p1;
   Pos2D p2;
   Pos2D p3;
   double angle_ab;
   double angle_bc;
   double angle_ca;
   bool inside_is_positive_angle;
   bool points_unique;
   bool points_make_triangle;

   void CheckPoints();
   void RenewAngles();
   friend bool Overlaps(const Triangle&, const Triangle&);
   
public :
   Triangle();
   Triangle& operator=(const Triangle& t);
   Triangle(const Triangle& t);
   Triangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3);
   
   ///< See @ref AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;
   
   
   void SetPos(int x1 , int y1 , int x2 , int y2 , int x3 , int y3);///< Set the corners of the triangle
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Output the triangle to a stream
};

bool Overlaps(const Triangle& t1 , const Triangle& t2);

/// -----------------------------------      Circle clsss         -----------------------------------------



class Circle : public AreaBase {
protected :
   int x;
   int y;
   int r;
   
public :
   
   Circle();
   Circle(int xpos , int ypos , int radius);
   ~Circle() {}
   
   ///< Overloads from @ref AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;

   /// Member functions
   void SetCenter(int xpos , int ypos);///< Set the center of the circle
   void SetRadius(int radius);///< Set the radius
   

   int X() const {return x;}
   int Y() const {return y;}
   int R() const {return r;}
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Output this circle to a stream

};


/// -----------------------------------      Rectangle class      ------------------------------------------

/**! @enum RECT_CORNER_POS
 *   @brief The specified corner of the rectangle
 */
enum RECT_CORNER_POS {
   RECT_CORNER_TL = 0,///< Top left corner
   RECT_CORNER_TR = 1,///< Top right corner
   RECT_CORNER_BR = 2,///< Bottom right corner
   RECT_CORNER_BL = 3 ///< Bottom left corner
};



class Rectangle : public AreaBase {
protected :
   int x,y,w,h,brx,bry;
   
   inline void RenewCornerPos();
   inline void RenewDimensions();

   
public :
   
   Rectangle();
   Rectangle(int xpos , int ypos , int width , int height);
   Rectangle(const Rectangle& r);

   Rectangle& operator=(const Rectangle& r);

   bool operator==(const Rectangle& rhs) {
      return ((x == rhs.x) &&
              (y == rhs.y) &&
              (w == rhs.w) &&
              (h == rhs.h));
   }
   
   bool operator!=(const Rectangle& rhs) {
      return !(*this == rhs);
   }
   
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;// returns a newed copy of itself

   inline bool DimensionsEqual(const Rectangle& r) const {///< True if same size
      return ((w == r.w) && (h == r.h));
   }
   
   inline bool AreaEqual(const Rectangle& r) const {///< True if same area
      return (Area() == r.Area());
   }

   int X()      const {return x;}
   int Y()      const {return y;}
   int W()      const {return w;}
   int H()      const {return h;}
   int Width()  const {return W();}
   int Height() const {return H();}
   int RX()     const {return brx;}
   int BRX()    const {return brx;}
   int BY()     const {return bry;}
   int BRY()    const {return bry;}
   int CX()     const {return (x + w/2);}
   int CY()     const {return (y + h/2);}
   int Area()   const {return w*h;}
   
   Pos2I TopLeft()      {return Pos2I(x       , y);}
   Pos2I TopCenter()    {return Pos2I(x + w/2 , y);}
   Pos2I TopRight()     {return Pos2I(x + w   , y);}
   Pos2I MiddleLeft()   {return Pos2I(x       , y + h/2);}
   Pos2I MiddleCenter() {return Pos2I(x + w/2 , y + h/2);}
   Pos2I MiddleRight()  {return Pos2I(x + w   , y + h/2);}
   Pos2I BottomLeft()   {return Pos2I(x       , y + h);}
   Pos2I BottomCenter() {return Pos2I(x + w/2 , y + h);}
   Pos2I BottomRight()  {return Pos2I(x + w   , y + h);}

   void SetPos       (int xpos  , int ypos);///< Set the top left corner
   void SetCenterPos (int cxpos , int cypos);///< Set the center position
   void SetCorners   (int x1 , int y1 , int x2 , int y2);///< Set the two corners
   void SetCornerPos (RECT_CORNER_POS rcpos , int xpos , int ypos);///< Set the specified corner position
   void SetDimensions(int width , int height);///< Change the size
   void SetArea      (int xpos  , int ypos , int width , int height);///< Redefine this rectangle's area and position

   inline void SetArea(const Rectangle& r);///< Sets the area

   inline AreaBase& MoveBy(int dx , int dy);///< Alters the rectangle's position by dx,dy
   inline Rectangle MovedBy(const Pos2I& p);///< Returns a copy of the rectangle moved by dx,dy

   inline Rectangle Shrink(int delta);///< Shrink the outside in by delta pixels
   inline Rectangle Grow(int delta);///< Grow the outside out by delta pixels
   
   void Scale(double scale);///< Scale the rectangle

   inline bool Overlaps(const Rectangle& r) const;///< True if overlap occurs

   inline virtual bool Contains(int xpos , int ypos) const;///< True if this rectangle contains the specified point
   inline bool Contains(const Rectangle& r) const;///< True if this rectangle contains the specified rectangle
   
   void Draw(EagleGraphicsContext* win , EagleColor color) const;///< Draw an outlined rectangle
   void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;///< Draw an outlined rectangle with varying thickness

   inline void OffsetDraw(EagleGraphicsContext* win , int ox , int oy , EagleColor color) const;///< Draw at an offset

   void Fill(EagleGraphicsContext* win , EagleColor color) const;///< Fill this rectangle with 'color'

   ///< Draws a frame 'width' thick from outside to inside
   void DrawInnerFrame(EagleGraphicsContext* win , unsigned int width , EagleColor color) const ;

   ///< Draws a filled rectangle with the specified radii forming quarter elliptical corners
   void RoundedFill   (EagleGraphicsContext* win , int hrad , int vrad  , EagleColor color) const ;
   ///< Draws an outlined rectangle with the specified radii forming quarter elliptical corners
   void RoundedOutline(EagleGraphicsContext* win , int hrad , int vrad  , EagleColor color) const ;
   ///< Draws an outlined rectangle with the specified thickness, with the specified radii forming quarter elliptical corners
   void RoundedOutline(EagleGraphicsContext* win , int hrad , int vrad  , double thickness , EagleColor color) const;
   
   ///< Draw a square dotted outline, skippping odd pixels if even is true
   void DottedOutline(EagleGraphicsContext* win , EagleColor color , bool even = true) const;

   ///< DrawGui* functions that draw the shape with a shadow underneath to the right

   void DrawGuiRectUp     (EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiRectDown   (EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiCircleUp   (EagleGraphicsContext* win , int radius   , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiCircleDown (EagleGraphicsContext* win , int radius   , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiRoundedUp  (EagleGraphicsContext* win , int hrad , int vrad , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiRoundedDown(EagleGraphicsContext* win , int hrad , int vrad , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiEllipseUp  (EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const ;
   void DrawGuiEllipseDown(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const ;

   /// Draws outlined triangular buttons
   void DrawGuiTriangleFaceUp   (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const ;
   void DrawGuiTriangleFaceRight(EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const ;
   void DrawGuiTriangleFaceDown (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const ;
   void DrawGuiTriangleFaceLeft (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const ;

   ///< Gradient drawing for solid colors

   ///< Draw a shaded quad with the specified corner colors
   void DrawShaded(EagleGraphicsContext* win , EagleColor tlcol , EagleColor trcol , EagleColor brcol , EagleColor blcol) const ;

   ///< Draws a frame from this rectangle in our out to another rectangle using shaded quads forming a gradient frame
   void DrawGradientFrameTo(EagleGraphicsContext* win , const Rectangle& r , EagleColor start_color , EagleColor finish_color) const ;


   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Output this rectangle to a stream
};


extern const Rectangle BADRECTANGLE;///< BADRECTANGLE stores invalid values for comparision with functions that return a Rectangle

Rectangle Overlap(const Rectangle& r1 , const Rectangle& r2);///< Returns the rectangle of overlap or BADRECTANGLE if none



/// Inline Rectangle:: class methods

inline void Rectangle::RenewCornerPos() {
   brx = x + w - 1;// w = brx - x + 1
   bry = y + h - 1;// h = bry - y + 1
}

inline void Rectangle::RenewDimensions() {
   w = brx - x + 1;
   h = bry - y + 1;
}

inline void Rectangle::SetArea(const Rectangle& r) {
   *this = r;
}

inline AreaBase& Rectangle::MoveBy(int dx    , int dy) {
   SetPos(x + dx , y + dy);
   return *this;
}

inline Rectangle Rectangle::MovedBy(const Pos2I& p) {
   Rectangle r = *this;
   r.MoveBy(p.X() , p.Y());
   return r;
}


inline Rectangle Rectangle::Shrink(int delta) {
   x += delta;
   y += delta;
   w -= 2*delta;
   h -= 2*delta;
   RenewCornerPos();
   return *this;
}

inline Rectangle Rectangle::Grow(int delta) {
   x -= delta;
   y -= delta;
   w += 2*delta;
   h += 2*delta;
   RenewCornerPos();
   return *this;
}

inline bool Rectangle::Overlaps(const Rectangle& r) const {
   return !(((x > r.brx) || (brx < r.x)) || ((y > r.bry) || (bry < r.y)));
   // if true, (x <= r.brx) && (brx >= r.x) && (y <= r.bry) && (bry >= r.y)
}

inline bool Rectangle::Contains(int xpos , int ypos) const {
   return !(((xpos < x) || (xpos > brx)) || ((ypos < y) || (ypos > bry)));
}

inline bool Rectangle::Contains(const Rectangle& r) const {
   return !((r.x < x) || (r.y < y) || (r.brx > brx) || (r.bry > bry));
}

inline void Rectangle::OffsetDraw(EagleGraphicsContext* win , int ox , int oy , EagleColor color) const {
   Rectangle r = *this;
   r.MoveBy(ox,oy);
   r.Draw(win , color);
///   rect(bmp , x + ox , y + oy , brx + ox , bry + oy , color);
}



/// -------------------------------------      RoundedRectangle     ------------------------------------------------


/**! @class RoundedRectangle
 *   @brief A rounded version of a rectangle with elliptical corners, like a pill
 */

class RoundedRectangle : public Rectangle {

private :
   int hrad;///< Horizontal radius for corners
   int vrad;///< Vertical radius for corners
   
public :

   ///< Build a rounded rectangle from a rectangle and specified radii
   RoundedRectangle(Rectangle r , int hradius , int vradius);

   ///< Build a rounded rectangle by specifying the percent of rounding on the specified axis 0 is square 1 is round
   RoundedRectangle(Rectangle r , float hrad_percent , float vrad_percent);

   ///< Copy constructor
   RoundedRectangle(const RoundedRectangle& rr);
   
   ///< Copy constructor that allows you to specify a base rectangle
   RoundedRectangle(Rectangle r , const RoundedRectangle& rr);

   ///< Assignment operator
   RoundedRectangle& operator=(const RoundedRectangle& rr);
   
   ///< @ref AreaBase overrides
   virtual bool Contains(int xpos , int ypos) const ;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   void SetRadii(int hradius , int vradius);///< Set the rounding radii
   void SetRoundingPercent(float hp , float vp);///< Set the rounding percent [0.0,1.0]
};



///< Global methods 




inline bool DimensionsEqual(const Rectangle& r1 , const Rectangle& r2) {
   return r1.DimensionsEqual(r2);
}

inline bool Overlaps(const Rectangle& r1 , const Rectangle& r2) {
   return r1.Overlaps(r2);
}

inline bool Overlaps(int min1 , int max1 , int min2 , int max2) {
   return !((min2 > max1) || (max2 < min1));
}

inline bool Contains(int min1 , int max1 , int value) {
   return !((value > max1) || (value < min1));
}

inline bool Contains(const Rectangle& r , int xpos , int ypos) {
   return r.Contains(xpos,ypos);
}

inline Rectangle OffsetRectangle(const Rectangle& r , int dx , int dy) {
   Rectangle R(r);
   R.MoveBy(dx,dy);
   return R;
}

/**! @enum RECT_CONNECT_TYPE
 *   @brief Information for specifying where rectangle 2 connects to rectangle 1
 */

enum RECT_CONNECT_TYPE {
   RECT_CONNECT_NONE         = 0,///< These rectangles don't connect
   RECT_CONNECT_R2_ON_TOP    = 1,///< R2 connects to the top of R1
   RECT_CONNECT_R2_ON_BOTTOM = 2,///< R2 connects to the bottom of R1
   RECT_CONNECT_R2_ON_RIGHT  = 3,///< R2 connects to the right of R1
   RECT_CONNECT_R2_ON_LEFT   = 4 ///< R2 connects to the left of R1
};

/**! @enum RECT_CONTAINS_TYPE
 *   @brief Whether one of two rectangles contains the other, or neither
 */

enum RECT_CONTAINS_TYPE {
   CONTAINS_NONE  = 0,
   R1_CONTAINS_R2 = 1,
   R2_CONTAINS_R1 = 2
};

///< Returns true if the Rectangles are adjacent and aligned
///< Returns the connection type of these two rectangles
RECT_CONNECT_TYPE DoRectanglesConnect(const Rectangle& r1 , const Rectangle& r2);

///< Returns whether or not one rectangle contains the other
RECT_CONTAINS_TYPE DoRectanglesContain(const Rectangle& r1 , const Rectangle& r2);

///< Returns the overlapping area of the two rectangles, BADRECTANGLE if none
Rectangle GetOverlap(const Rectangle& r1 , const Rectangle& r2);

///< Return a list of rectangles created by combining r1 and r2
std::list<Rectangle> CombineRectangles(Rectangle r1 , Rectangle r2);

///< Combine rectangles while they overlap or connect and return a list of the new rectangles that cover the old
std::list<Rectangle> ConsolidateRectangles(std::list<Rectangle> rectlist);

///< Remove a rectangular area from another Rectangle, returning the areas left of the from Rectangle and the sub Rectangle
bool SubtractRectangle(Rectangle sub , Rectangle from , std::list<Rectangle>& sublist , std::list<Rectangle>& fromlist);

///< Remove a rectangular area from another Rectangle
std::list<Rectangle> SubtractRectangle(Rectangle sub , Rectangle from);



/// ------------------------------     Ellipse     ----------------------------------------


/**! @class Ellipse
 *   @brief A class to store an elliptical shape
 */

class Ellipse : public Rectangle {
   
public :

   Ellipse(Rectangle r);
   Ellipse(int ex , int ey , int ew , int eh);

   ///< AreaBase overrides
   
   virtual bool Contains(int xpos , int ypos) const ;
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual AreaBase* Clone() const;
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};



#endif // Area_H



