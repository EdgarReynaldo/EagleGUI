
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Area_H
#define Area_H


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



class Rectangle;
class EagleGraphicsContext;


class AreaBase {

protected :

public :
   AreaBase() {}
   virtual ~AreaBase() {}
   
   virtual bool Contains(int xpos , int ypos) const =0;
   virtual AreaBase& MoveBy(int dx , int dy)=0;
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const =0;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const =0;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const =0;
   // CreateBoundingRectangles creates a vector of NEW Rectangles, destroy with DestroyBoundingRectangles
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const =0;
   virtual AreaBase* Clone() const =0;// returns a newed copy of itself
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   friend std::ostream& operator<<(std::ostream& os , const AreaBase& area); 
};




void DestroyBoundingRectangles(std::vector<Rectangle*>& vrect);



/// ----------------------------     AreaGroup class      --------------------------------



class AreaGroup : public AreaBase {
   
protected :
   std::vector<AreaBase*> areas;
   std::map<AreaBase* , bool> del_map;
   
   
public :
   AreaGroup();
   ~AreaGroup();
   
   AreaGroup& operator=(const AreaGroup& a) {
      (void)a;
      throw EagleException("TODO : Implement copy constructor for AreaGroup class.");
   }
   
   /// Overloads from AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;


   /// Member functions
   void AddArea(AreaBase* a , bool delete_area = true);
   void RemoveArea(AreaBase* a);
   void ClearAreas();
   void CloneAreaGroup(const AreaGroup& ag);
   void AdoptAreaGroup(AreaGroup& ag);// takes ownership of ag's areas and clears ag of data
   
   int size() {return areas.size();}

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};




/// -------------------------------------     Line class       -----------------------------------------



class Line : public AreaBase {
protected :
   Pos2F p1;
   Pos2F p2;
   double thickness;
   
public :
   /// TODO : Implement line class?

///   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};



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

public :
   Triangle();
   Triangle(const Triangle& t);
   Triangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3);
   
   virtual bool Contains(int xpos , int ypos) const;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;
   
   
   void SetPos(int x1 , int y1 , int x2 , int y2 , int x3 , int y3);
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};


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
   
   /// Overloads from AreaBase
   virtual bool Contains(int xpos , int ypos) const;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;

   /// Member functions
   void SetCenter(int xpos , int ypos);
   void SetRadius(int radius);
   

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;

};


/// -----------------------------------      Rectangle class      ------------------------------------------


enum RECT_CORNER_POS {
   RECT_CORNER_TL = 0,
   RECT_CORNER_TR = 1,
   RECT_CORNER_BR = 2,
   RECT_CORNER_BL = 3
};

#warning Eagle::Rectangle has issues with OpenGL headers


class Rectangle : public AreaBase {
protected :
   int x,y,w,h,brx,bry;
   
   inline void RenewCornerPos();
   inline void RenewDimensions();

   
public :
   
///   Rectangle();
   Rectangle() : 
      AreaBase() , 
      x(0) , 
      y(0) , 
      w(0) , 
      h(0) , 
      brx(0) , 
      bry(0) 
   {}
   
///   Rectangle(int xpos , int ypos , int width , int height);
   Rectangle(int xpos , int ypos , int width , int height) :
         AreaBase() ,
         x(xpos) , 
         y(ypos) , 
         w(width) , 
         h(height) ,
         brx(0),
         bry(0)
   {
      RenewCornerPos();
   }

///   Rectangle(const Rectangle& r);
   Rectangle(const Rectangle& r) : 
         AreaBase() ,
         x(r.x) , 
         y(r.y) , 
         w(r.w) , 
         h(r.h) , 
         brx(r.brx) , 
         bry(r.bry)
   {}

   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;// returns a newed copy of itself

   inline bool DimensionsEqual(const Rectangle& r) const {
      return ((w == r.w) && (h == r.h));
   }
   
   inline bool AreaEqual(const Rectangle& r) const {
      return (Area() == r.Area());
   }

   int X()   const {return x;}
   int Y()   const {return y;}
   int W()   const {return w;}
   int H()   const {return h;}
   int BRX() const {return brx;}
   int BRY() const {return bry;}
   int CX()  const {return (x + w/2);}
   int CY()  const {return (y + h/2);}
   int Area() const {return w*h;}


   void SetPos       (int xpos  , int ypos);
   void SetCenterPos (int cxpos , int cypos);
   void SetCorners   (int x1 , int y1 , int x2 , int y2);
   void SetCornerPos (RECT_CORNER_POS rcpos , int xpos , int ypos);
   void SetDimensions(int width , int height);
   void SetArea      (int xpos  , int ypos , int width , int height);

   inline void SetArea(const Rectangle& r);

   inline AreaBase& MoveBy(int dx , int dy);
   inline Rectangle MoveBy(const Pos2I& p);

   inline Rectangle Shrink(int delta);
   inline Rectangle Grow(int delta);
   
   void Scale(double scale);

   inline bool Overlaps(const Rectangle& r) const;

   inline virtual bool Contains(int xpos , int ypos) const;
   inline bool Contains(const Rectangle& r) const;
   
   void Draw(EagleGraphicsContext* win , EagleColor color) const;
   void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;

   inline void OffsetDraw(EagleGraphicsContext* win , int ox , int oy , EagleColor color) const;

   void Fill(EagleGraphicsContext* win , EagleColor color) const;

   void DrawInnerFrame(EagleGraphicsContext* win , unsigned int width , EagleColor color) const ;
   void RoundedFill   (EagleGraphicsContext* win , int hrad , int vrad  , EagleColor color) const ;
   void RoundedOutline(EagleGraphicsContext* win , int hrad , int vrad  , EagleColor color) const ;
   void RoundedOutline(EagleGraphicsContext* win , int hrad , int vrad  , double thickness , EagleColor color) const;
   void DottedOutline(EagleGraphicsContext* win , EagleColor color , bool even = true) const;

   /// DrawGui* functions that draw the shape with a shadow underneath to the right
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

   /// Gradient drawing for solid colors
   void DrawShaded(EagleGraphicsContext* win , EagleColor tlcol , EagleColor trcol , EagleColor brcol , EagleColor blcol) const ;
   void DrawGradientFrameTo(EagleGraphicsContext* win , const Rectangle* r2 , EagleColor start_color , EagleColor finish_color) const ;


   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};


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

inline Rectangle Rectangle::MoveBy(const Pos2I& p) {
   MoveBy(p.X() , p.Y());
   return *this;
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



class RoundedRectangle : public Rectangle {

private :
   int hrad;
   int vrad;
///   float thickness;
   
public :

   RoundedRectangle(Rectangle r , int hradius , int vradius);/// , float linethickness = 1.0f);
   RoundedRectangle(Rectangle r , float hrad_percent , float vrad_percent);/// , float linethickness = 1.0f);
   RoundedRectangle(Rectangle r , const RoundedRectangle& rr);

   virtual bool Contains(int xpos , int ypos) const ;
   virtual AreaBase& MoveBy(int dx , int dy);
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual std::vector<Rectangle*> CreateBoundingRectangles() const;
   virtual AreaBase* Clone() const;
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   void SetRadii(int hradius , int vradius);
   void SetRoundingPercent(float hp , float vp);

   
};



/// Global methods 
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

enum RECT_CONNECT_TYPE {
   RECT_CONNECT_NONE         = 0,
   RECT_CONNECT_R2_ON_TOP    = 1,
   RECT_CONNECT_R2_ON_BOTTOM = 2,
   RECT_CONNECT_R2_ON_RIGHT  = 3,
   RECT_CONNECT_R2_ON_LEFT   = 4
};

enum RECT_CONTAINS_TYPE {
   CONTAINS_NONE  = 0,
   R1_CONTAINS_R2 = 1,
   R2_CONTAINS_R1 = 2
};

// returns true if the Rectangles are adjacent and aligned
RECT_CONNECT_TYPE DoRectanglesConnect(const Rectangle& r1 , const Rectangle& r2);

RECT_CONTAINS_TYPE DoRectanglesContain(const Rectangle& r1 , const Rectangle& r2);

std::list<Rectangle> CombineRectangles(Rectangle r1 , Rectangle r2);

std::list<Rectangle> ConsolidateRectangles(std::list<Rectangle> rectlist);


/// ------------------------------     Ellipse     ----------------------------------------



class Ellipse : public Rectangle {
   
public :

   Ellipse(Rectangle r);
   Ellipse(int ex , int ey , int ew , int eh);

   virtual bool Contains(int xpos , int ypos) const ;
   virtual void Draw(EagleGraphicsContext* win , EagleColor color) const;
   virtual void Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const ;
   virtual void Fill(EagleGraphicsContext* win , EagleColor color) const;
   virtual AreaBase* Clone() const;
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};


#endif // Area_H


