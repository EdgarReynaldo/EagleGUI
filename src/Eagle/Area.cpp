
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Area.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/StringWork.hpp"

#include <cmath>
#include <vector>
#include <map>
#include <list>

using std::list;
using std::vector;
using std::map;

/// NOTE : std=c++11 doesn't define M_PI

#include "Eagle/Math.hpp"


void DestroyBoundingRectangles(vector<Rectangle*>& vrect) {
   for (unsigned int i = 0 ; i < vrect.size() ; ++i) {
      delete vrect[i];
   }
   vrect.clear();
}


/// -----------------------------------    Area Base     ----------------------------------------



std::ostream& AreaBase::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "AreaBase::DescribeTo - This classes DescribeTo has not yet been implented.";
   return os;
}



std::ostream& operator<<(std::ostream& os , const AreaBase& area) {
   return area.DescribeTo(os);
}



/// ----------------------------------     Area Group class     -----------------------------------



AreaGroup::AreaGroup() :
      areas(),
      del_map()
{}



AreaGroup::~AreaGroup() {
   ClearAreas();
}



bool AreaGroup::Contains(int xpos , int ypos) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      if (a->Contains(xpos,ypos)) {return true;}
   }
   return false;
}



AreaBase& AreaGroup::MoveBy(int dx , int dy) {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->MoveBy(dx,dy);
   }
   return *this;
}
   


void AreaGroup::Draw(EagleGraphicsContext* win , EagleColor color) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->Draw(win , color);
   }
}



void AreaGroup::Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->Draw(win , thickness , color);
   }
}



void AreaGroup::Fill(EagleGraphicsContext* win , EagleColor color) const {
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      a->Fill(win , color);
   }
}



vector<Rectangle*> AreaGroup::CreateBoundingRectangles() const {
   vector<Rectangle*> vtotal;
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      vector<Rectangle*> vrect = a->CreateBoundingRectangles();
      for (unsigned int j = 0 ; j < vrect.size() ; ++j) {
         Rectangle* r = vrect[j];
         vtotal.push_back(r);
      }
   }
   return vtotal;
}



AreaBase* AreaGroup::Clone() const {
   AreaGroup* a = new AreaGroup();
   a->CloneAreaGroup(*this);
   return a;
}



void AreaGroup::AddArea(AreaBase* a , bool delete_area) {
   if (a) {
      if (del_map.find(a) == del_map.end()) {
         areas.push_back(a);
      }
      del_map[a] = delete_area;
   }
}



void AreaGroup::RemoveArea(AreaBase* a) {
   if (!a) {return;}
   map<AreaBase* , bool>::iterator it = del_map.find(a);
   if (it != del_map.end()) {
      bool del = it->second;
      if (del) {delete a;}
      del_map.erase(it);
      for (vector<AreaBase*>::iterator vit = areas.begin() ; vit != areas.end() ; ++vit) {
         AreaBase* area = *vit;
         if (a == area) {
            areas.erase(vit);
            break;
         }
      }
   }
}



void AreaGroup::ClearAreas() {
   for (map<AreaBase* , bool>::iterator it = del_map.begin() ; it != del_map.end() ; ++it) {
      AreaBase* a = it->first;
      bool del = it->second;
      if (del) {delete a;}
   }
   del_map.clear();
   areas.clear();
}



void AreaGroup::CloneAreaGroup(const AreaGroup& ag) {
   ClearAreas();
   const vector<AreaBase*>& varea = ag.areas;
   for (unsigned int i = 0 ; i < varea.size() ; ++i) {
      AreaBase* a = varea[i];
      AddArea(a->Clone() , true);
   }
}



void AreaGroup::AdoptAreaGroup(AreaGroup& ag) {
   ClearAreas();
   areas = ag.areas;
   del_map = ag.del_map;
   ag.areas.clear();
   ag.del_map.clear();
}



std::ostream& AreaGroup::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << areas.size() << " areas. Area = ";
   if (!areas.size()) {
      os << "Undefined";
   }
   for (unsigned int i = 0 ; i < areas.size() ; ++i) {
      AreaBase* a = areas[i];
      os << "[";
      a->DescribeTo(os);
      os << "] ";
   }
   return os;
}




/// -------------------------------------     Triangle class     ----------------------------------------



void Triangle::CheckPoints() {
   points_unique = true;
   if (p1 == p2) {points_unique = false;}
   if (p1 == p3) {points_unique = false;}
   if (p2 == p3) {points_unique = false;}
   points_make_triangle = points_unique;
}



void Triangle::RenewAngles() {
   CheckPoints();
   if (points_unique) {
      angle_ab = p1.AngleToPoint(p2);// -PI to PI
      angle_bc = p2.AngleToPoint(p3);// -PI to PI
      angle_ca = p3.AngleToPoint(p1);// -PI to PI
      double turn_angle = angle_bc - angle_ab;//[-PI - PI , PI - -PI] = [-2PI , 2PI]
      // Normalize turn angle to (-M_PI , M_PI];
      if (turn_angle <= -M_PI) {turn_angle += 2.0*M_PI;}
      if (turn_angle > M_PI) {turn_angle -= 2.0*M_PI;};
      if ((turn_angle == 0.0) || (turn_angle == M_PI)) {
         // collinear points
         points_make_triangle = false;
      }
      inside_is_positive_angle = (turn_angle > 0.0);
   }
}



Triangle::Triangle() :
      p1(),
      p2(),
      p3(),
      angle_ab(0.0),
      angle_bc(0.0),
      angle_ca(0.0),
      inside_is_positive_angle(false),
      points_unique(false),
      points_make_triangle(false)
{
   
}



Triangle& Triangle::operator=(const Triangle& t) {
   p1 = t.p1;
   p2 = t.p2;
   p3 = t.p3;
   angle_ab = t.angle_ab;
   angle_bc = t.angle_bc;
   angle_ca = t.angle_ca;
   inside_is_positive_angle = t.inside_is_positive_angle;
   points_unique = t.points_unique;
   points_make_triangle = t.points_make_triangle;
   return *this;
}



Triangle::Triangle(const Triangle& t) :
      p1(t.p1),
      p2(t.p2),
      p3(t.p3),
      angle_ab(t.angle_ab),
      angle_bc(t.angle_bc),
      angle_ca(t.angle_ca),
      inside_is_positive_angle(t.inside_is_positive_angle),
      points_unique(t.points_unique),
      points_make_triangle(t.points_make_triangle)
{}



Triangle::Triangle(int x1 , int y1 , int x2 , int y2 , int x3 , int y3) :
      p1(x1,y1),
      p2(x2,y2),
      p3(x3,y3),
      angle_ab(0.0),
      angle_bc(0.0),
      angle_ca(0.0),
      inside_is_positive_angle(false),
      points_unique(false),
      points_make_triangle(false)
{
   RenewAngles();
}



bool Triangle::Contains(int xpos , int ypos) const {
   if (!points_make_triangle) {return false;}
   Pos2D p4(xpos,ypos);
   const Pos2D* pts[3] = {&p1 , &p2 , &p3};
   double angles[3] = {angle_ab , angle_bc , angle_ca};
   double turn_angle = 0.0;
   double angle_to_p4 = 0.0;
   for (int i = 0 ; i < 3 ; ++i) {
      if (*pts[i] == p4) {return true;}
      angle_to_p4 = (*pts[i]).AngleToPoint(p4);// -PI to PI
      turn_angle = angle_to_p4 - angles[i];// -2PI to 2PI
      // Normalize turn angle to (-M_PI , M_PI];
      if (turn_angle <= -M_PI) {turn_angle += 2.0*M_PI;}
      if (turn_angle > M_PI) {turn_angle -= 2.0*M_PI;};
      if (inside_is_positive_angle && (turn_angle < 0.0)) {return false;}
      if (!inside_is_positive_angle && (turn_angle > 0.0)) {return false;}
   }
   return true;
}



AreaBase& Triangle::MoveBy(int dx , int dy) {
   p1.MoveBy(dx,dy);
   p2.MoveBy(dx,dy);
   p3.MoveBy(dx,dy);
   return *this;
}



void Triangle::Draw(EagleGraphicsContext* win , EagleColor color) const {
   EAGLE_ASSERT(win);
   p1.DrawLineTo(win , p2 , color);
   p2.DrawLineTo(win , p3 , color);
   p3.DrawLineTo(win , p1 , color);
}



void Triangle::Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const {
   EAGLE_ASSERT(win);
   p1.DrawLineTo(win , p2 , thickness , color);
   p2.DrawLineTo(win , p3 , thickness , color);
   p3.DrawLineTo(win , p1 , thickness , color);
}



void Triangle::Fill(EagleGraphicsContext* win , EagleColor color) const {
   EAGLE_ASSERT(win);
   if (points_make_triangle) {
      win->DrawFilledTriangle((float)p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y() , color);
   }
}



vector<Rectangle*> Triangle::CreateBoundingRectangles() const {
   int lx = p1.X();
   int rx = p1.X();
   int ty = p1.Y();
   int by = p1.Y();
   if (p2.X() < lx) {lx = p2.X();}
   if (p3.X() < lx) {lx = p3.X();}
   if (p2.X() > rx) {rx = p2.X();}
   if (p3.X() > rx) {rx = p3.X();}
   if (p2.Y() < ty) {ty = p2.Y();}
   if (p3.Y() < ty) {ty = p3.Y();}
   if (p2.Y() > by) {by = p2.Y();}
   if (p3.Y() > by) {by = p3.Y();}
   vector<Rectangle*> vrect;
   vrect.push_back(new Rectangle(lx , ty , rx - lx + 1 , by - ty + 1));
   return vrect;
}



AreaBase* Triangle::Clone() const {
   return new Triangle(p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y());
}



void Triangle::SetPos(int x1 , int y1 , int x2 , int y2 , int x3 , int y3) {
   p1.SetPos(x1,y1);
   p2.SetPos(x2,y2);
   p3.SetPos(x3,y3);
   RenewAngles();
}



std::ostream& Triangle::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "Triangle points = [" << p1 << "] [" << p2 << "] [" << p3 << "]";
   return os;
}



bool Overlaps(const Triangle& t1 , const Triangle& t2) {
   return t1.Contains(t2.p1.tx,t2.p1.ty) || t1.Contains(t2.p2.tx,t2.p2.ty) || t1.Contains(t2.p3.tx,t2.p3.ty) ||
          t2.Contains(t1.p1.tx,t1.p3.ty) || t2.Contains(t1.p2.tx,t1.p2.ty) || t2.Contains(t1.p3.tx,t1.p3.ty);
}



/// -----------------------------------      Circle clsss         -----------------------------------------



Circle::Circle() : x(0) , y(0) , r(0) {}



Circle::Circle(int xpos , int ypos , int radius) : x(xpos) , y(ypos) , r(radius) {}



bool Circle::Contains(int xpos , int ypos) const {
   float dx = (float)(xpos - x);
   float dy = (float)(ypos - y);
   float dsq = dx*dx + dy*dy;
   if (dsq <= (float)r*r) {return true;}
   return false;
}



AreaBase& Circle::MoveBy(int dx , int dy) {
   SetCenter(x + dx , y + dy);
   return *this;
}



void Circle::Draw(EagleGraphicsContext* win , EagleColor color) const {
	EAGLE_ASSERT(win);
   win->DrawCircle(x , y , r , 1.0 , color);
}



void Circle::Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const {
	EAGLE_ASSERT(win);
   win->DrawCircle(x , y , r , thickness , color);
}



void Circle::Fill(EagleGraphicsContext* win , EagleColor color) const {
   win->DrawFilledCircle(x , y , r , color);
}



vector<Rectangle*> Circle::CreateBoundingRectangles() const {
   int lx = x - r;
   int rx = x + r;
   int ty = y - r;
   int by = y + r;
   int w = rx - lx + 1;
   int h = by - ty + 1;
   vector<Rectangle*> vrect;
   vrect.push_back(new Rectangle(lx , ty , w , h));
   return vrect;
}



AreaBase* Circle::Clone() const {
   return new Circle(x,y,r);
}



void Circle::SetCenter(int xpos , int ypos) {
   x = xpos;
   y = ypos;
}



void Circle::SetRadius(int radius) {
   if (radius < 0) {radius = 0;}
   r = radius;
}



std::ostream& Circle::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "Circle (x,y,r) = (" << x << "," << y << "," << r << ")";
   return os;
}



/// --------------------------------      Rectangle class      ----------------------------------------




const Rectangle BADRECTANGLE = Rectangle();



using std::ostream;
using std::endl;



Rectangle::Rectangle() : 
      AreaBase(),
      x(-1),
      y(-1),
      w(-1),
      h(-1),
      brx(-1),
      bry(-1)
{}
   


Rectangle::Rectangle(int xpos , int ypos , int width , int height) :
      AreaBase(),
      x(xpos),
      y(ypos),
      w(width),
      h(height),
      brx(0),
      bry(0)
{
   RenewCornerPos();
}



Rectangle::Rectangle(const Rectangle& r) : 
      AreaBase() ,
      x(r.x) , 
      y(r.y) , 
      w(r.w) , 
      h(r.h) , 
      brx(r.brx) , 
      bry(r.bry)
{}



Rectangle& Rectangle::operator=(const Rectangle& r) {
   this->x = r.x;
   this->y = r.y;
   this->w = r.w;
   this->h = r.h;
   this->brx = r.brx;
   this->bry = r.bry;
   return *this;
}



vector<Rectangle*> Rectangle::CreateBoundingRectangles() const {
   vector<Rectangle*> vrect;
   vrect.push_back(new Rectangle(x,y,w,h));
   return vrect;
}



AreaBase* Rectangle::Clone() const {
   return new Rectangle(x,y,w,h);
}



void Rectangle::SetPos(int xpos , int ypos) {
   x = xpos;
   y = ypos;
   RenewCornerPos();
}



void Rectangle::SetCenterPos (int cxpos , int cypos) {
   SetPos(cxpos - w/2 , cypos - h/2);
}



void Rectangle::SetCorners   (int x1 , int y1 , int x2 , int y2) {
   x = (x1 < x2)?x1:x2;
   y = (y1 < y2)?y1:y2;
   brx = (x1 > x2)?x1:x2;
   bry = (y1 > y2)?y1:y2;
   RenewDimensions();
}



void Rectangle::SetCornerPos (RECT_CORNER_POS rcpos , int xpos , int ypos) {
   int x1 = x;
   int x2 = brx;
   int y1 = y;
   int y2 = bry;
   switch (rcpos) {
      case RECT_CORNER_TL :
         x1 = xpos;
         y1 = ypos;
         break;
      case RECT_CORNER_TR :
         x2 = xpos;
         y1 = ypos;
         break;
      case RECT_CORNER_BR :
         x2 = xpos;
         y2 = ypos;
         break;
      case RECT_CORNER_BL :
         x1 = xpos;
         y2 = ypos;
         break;
   }
   SetCorners(x1,y1,x2,y2);
}



void Rectangle::SetDimensions(int width , int height) {
   w = width;
   h = height;
   RenewCornerPos();
}



void Rectangle::SetArea(int xpos , int ypos , int width , int height) {
   x = xpos;
   y = ypos;
   w = width;
   h = height;
   RenewCornerPos();
}



void Rectangle::Scale(double scale) {
   w = (int)(w*scale);
   h = (int)(h*scale);
}



void Rectangle::Draw(EagleGraphicsContext* win , EagleColor color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
	win->DrawRectangle(x,y,w,h,1,color);
}



void Rectangle::Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
	win->DrawRectangle(x,y,w,h,thickness,color);
}



void Rectangle::Fill(EagleGraphicsContext* win , EagleColor color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   win->DrawFilledRectangle(x , y , w , h , color);
}



void Rectangle::DrawInnerFrame(EagleGraphicsContext* win , unsigned int width , EagleColor color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   if (width < 1) {return;}
   
   win->DrawFilledRectangle(x             , y         , x + w , y + width     , color);/// Top rectangle
   win->DrawFilledRectangle(x             , y + h     , x + w , y + h - w     , color);/// Bottom rectangle
   win->DrawFilledRectangle(x             , y + width , x     , y + h - width , color);/// Left rectangle
   win->DrawFilledRectangle(x + w - width , y + width , x + w , y + h - width , color);/// Right rectangle
}



void Rectangle::RoundedFill(EagleGraphicsContext* win , int hrad , int vrad , EagleColor color) const {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   EAGLE_ASSERT(hrad >= 0 && vrad >= 0);
   win->DrawFilledRoundedRectangle(x , y , w , h , hrad , vrad , color);
/*
   int mid_tlx = x + corner_radius;
   int mid_tly = y + corner_radius;
   int mid_brx = brx - corner_radius;
   int mid_bry = bry - corner_radius;

   circlefill(bmp , mid_tlx , mid_tly , corner_radius , color);
   circlefill(bmp , mid_brx , mid_tly , corner_radius , color);
   circlefill(bmp , mid_tlx , mid_bry , corner_radius , color);
   circlefill(bmp , mid_brx , mid_bry , corner_radius , color);
   
   rectfill(bmp , x     , mid_tly , mid_tlx , mid_bry , color);
   rectfill(bmp , brx     , mid_tly , mid_brx , mid_bry , color);
   rectfill(bmp , mid_tlx , y     , mid_brx , bry     , color);
*/   
}



void Rectangle::RoundedOutline(EagleGraphicsContext* win , int hrad , int vrad  , EagleColor color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   EAGLE_ASSERT(hrad >= 0 && vrad >= 0);
	win->DrawRoundedRectangle(x , y , w , h , hrad , vrad , 1.0 , color);
/*
   ASSERT(bmp);
   int mid_tlx = x + corner_radius;
   int mid_tly = y + corner_radius;
   int mid_brx = brx - corner_radius;
   int mid_bry = bry - corner_radius;
   
//void arc(BITMAP *bmp, int x, y, fixed ang1, ang2, int r, int color);   
   arc(bmp , mid_brx , mid_tly , itofix(0)   , itofix(64)  , corner_radius , color);
   arc(bmp , mid_tlx , mid_tly , itofix(64)  , itofix(128) , corner_radius , color);
   arc(bmp , mid_tlx , mid_bry , itofix(128) , itofix(192) , corner_radius , color);
   arc(bmp , mid_brx , mid_bry , itofix(192) , itofix(256) , corner_radius , color);
   
   hline(bmp , mid_tlx , y       , mid_brx , color);
   hline(bmp , mid_tlx , bry     , mid_brx , color);
   
   vline(bmp , x       , mid_tly , mid_bry , color);
   vline(bmp , brx     , mid_tly , mid_bry , color);
*/
}



void Rectangle::RoundedOutline(EagleGraphicsContext* win , int hrad , int vrad  , double thickness , EagleColor color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   EAGLE_ASSERT(hrad >= 0 && vrad >= 0);
	win->DrawRoundedRectangle(x , y , w , h , hrad , vrad , thickness , color);
}



void Rectangle::DottedOutline(EagleGraphicsContext* win , EagleColor color , bool even) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   const int start = (even?0:1);
   for (int xpos = x + start ; xpos < x + w ; xpos += 2) {
      win->PutPixel(xpos , y , color);
      win->PutPixel(xpos , y + h - 1 , color);
   }
   for (int ypos = y + start ; ypos < y + h ; ypos += 2) {
      win->PutPixel(x , ypos , color);
      win->PutPixel(x + w - 1 , ypos , color);
   }
}



void Rectangle::DrawGuiRectUp(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Rectangle upper(x , y , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.Fill(win , sd_color);
   upper.Fill(win , fg_color);
}



void Rectangle::DrawGuiRectDown(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Rectangle upper(x + 1 , y + 1 , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.Fill(win , sd_color);
   upper.Fill(win , fg_color);
}



void Rectangle::DrawGuiCircleUp(EagleGraphicsContext* win , int radius , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   int cx = x + w/2;
   int cy = y + h/2;
   win->DrawFilledCircle(cx     , cy     , radius     , sd_color);
   win->DrawFilledCircle(cx + 1 , cy + 1 , radius - 1 , sd_color);
   win->DrawFilledCircle(cx - 1 , cy - 1 , radius - 2 , fg_color);
}



void Rectangle::DrawGuiCircleDown(EagleGraphicsContext* win , int radius , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   if (radius < 2) {radius = 2;}
   int cx = x + w/2;
   int cy = y + h/2;
   win->DrawFilledCircle(cx , cy , radius     , sd_color);
   win->DrawFilledCircle(cx , cy , radius - 2 , fg_color);
}



void Rectangle::DrawGuiRoundedUp(EagleGraphicsContext* win , int hrad , int vrad , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   if (hrad < 1) {hrad = 1;}
   if (vrad < 1) {vrad = 1;}
   
   Rectangle upper(x , y , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.RoundedFill(win , hrad-1 , vrad-1 , sd_color);
   upper.RoundedFill(win , hrad-1 , vrad-1 , fg_color);
}



void Rectangle::DrawGuiRoundedDown(EagleGraphicsContext* win , int hrad , int vrad , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Rectangle upper(x + 1 , y + 1 , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.RoundedFill(win , hrad-1 , vrad-1 , sd_color);
   upper.RoundedFill(win , hrad-1 , vrad-1 , fg_color);
}



void Rectangle::DrawGuiEllipseUp(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   int cx = x + w/2;
   int cy = y + h/2;
   if (w > 1 && h > 1) {
      win->DrawFilledEllipse(cx + 1 , cy + 1 , w/2 , h/2 , sd_color);
      win->DrawFilledEllipse(cx - 1 , cy - 1 , w/2 - 1 , h/2 - 1 , fg_color);
   }
}



void Rectangle::DrawGuiEllipseDown(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   int cx = x + w/2;
   int cy = y + h/2;
   win->DrawFilledEllipse(cx , cy , w/2 - 1 , h/2 - 1 , sd_color);
   win->DrawFilledEllipse(cx , cy , w/2 - 2 , h/2 - 2 , fg_color);
}



void Rectangle::DrawGuiTriangleFaceUp   (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Fill(win , fg_color);
   int p1x = x + w/2;
   int p1y = y + h/5;
   int p2x = x + w/5;
   int p2y = y + (4*h)/5;
   int p3x = x + (4*w)/5;
   int p3y = p2y;
   win->DrawFilledTriangle(p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   win->DrawTriangle(p1x , p1y , p2x , p2y , p3x , p3y , 1 , bg_color);
}



void Rectangle::DrawGuiTriangleFaceRight(EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Fill(win , fg_color);
   int p1x = x + (4*w)/5;
   int p1y = y + h/2;
   int p2x = x + w/5;
   int p2y = y + h/5;
   int p3x = p2x;
   int p3y = y + (4*h)/5;
   win->DrawFilledTriangle(p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   win->DrawTriangle(p1x , p1y , p2x , p2y , p3x , p3y , 1 , bg_color);
}



void Rectangle::DrawGuiTriangleFaceDown (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Fill(win , fg_color);
   int p1x = x + w/2;
   int p1y = y + (4*h)/5;
   int p2x = x + w/5;
   int p2y = y + h/5;
   int p3x = x + (4*w)/5;
   int p3y = p2y;
   win->DrawFilledTriangle(p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   win->DrawTriangle(p1x , p1y , p2x , p2y , p3x , p3y , 1 , bg_color);
}



void Rectangle::DrawGuiTriangleFaceLeft (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   Fill(win , fg_color);
   int p1x = x + w/5;
   int p1y = y + h/2;
   int p2x = x + (4*w)/5;
   int p2y = y + h/5;
   int p3x = p2x;
   int p3y = y + (4*h)/5;
   win->DrawFilledTriangle(p1x , p1y , p2x , p2y , p3x , p3y , sd_color);
   win->DrawTriangle(p1x , p1y , p2x , p2y , p3x , p3y , 1 , bg_color);
}



void Rectangle::DrawShaded(EagleGraphicsContext* win , EagleColor tlcol , EagleColor trcol , EagleColor brcol , EagleColor blcol) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
	win->DrawShadedRectangle(this , tlcol , trcol , brcol , blcol);
}


   
void Rectangle::DrawGradientFrameTo(EagleGraphicsContext* win , const Rectangle& r , EagleColor start_color , EagleColor finish_color) const {
	EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
	const Rectangle* r1 = this;
	const Rectangle* r2 = &r;
	float tlx1 = r1->X();
	float tly1 = r1->Y();
	float brx1 = r1->X() + r1->W();
	float bry1 = r1->Y() + r1->H();
	float tlx2 = r2->X();
	float tly2 = r2->Y();
	float brx2 = r2->X() + r2->W();
	float bry2 = r2->Y() + r2->H();
	
	win->DrawShadedQuad(tlx1 , tly1 , start_color,
							  brx1 , tly1 , start_color, 
							  brx2 , tly2 , finish_color, 
							  tlx2 , tly2 , finish_color);
	win->DrawShadedQuad(brx1 , bry1 , start_color,
							  brx1 , tly1 , start_color, 
							  brx2 , tly2 , finish_color, 
							  brx2 , bry2 , finish_color);
	win->DrawShadedQuad(tlx1 , bry1 , start_color,
							  brx1 , bry1 , start_color, 
							  brx2 , bry2 , finish_color, 
							  tlx2 , bry2 , finish_color);
	win->DrawShadedQuad(tlx1 , tly1 , start_color,
							  tlx1 , bry1 , start_color, 
							  tlx2 , bry2 , finish_color,
							  tlx2 , tly2 , finish_color);
	
}




std::ostream& Rectangle::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("Rectangle (x,y,brx,bry,w,h) = (%i,%i,%i,%i,%i,%i)" , x , y , brx , bry , w , h);
   return os;
}



Rectangle Overlap(const Rectangle& r1 , const Rectangle& r2) {
   if (!Overlaps(r1,r2)) {return BADRECTANGLE;}
   int lx = (r1.X() > r2.X())?r1.X():r2.X();
   int ty = (r1.Y() > r2.Y())?r1.Y():r2.Y();

   int rx = (r1.BRX() < r2.BRX())?r1.BRX():r2.BRX();
   int by = (r1.BRY() < r2.BRY())?r1.BRY():r2.BRY();
   
   Rectangle r;
   r.SetCorners(lx,ty,rx,by);
   return r;
}



/// ----------------------------------     RoundedRectangle    -----------------------------



RoundedRectangle::RoundedRectangle(Rectangle r , int hradius , int vradius) :
      Rectangle(r),
      hrad(0),
      vrad(0)
///      thickness(1.0f)
{
   SetRadii(hradius , vradius);
///   SetThickness(linethickness);
}



RoundedRectangle::RoundedRectangle(Rectangle r , float hrad_percent , float vrad_percent) :
      Rectangle(r),
      hrad(0),
      vrad(0)
///      thickness(1.0f)
{
   SetRoundingPercent(hrad_percent , vrad_percent);
///   SetThickness(linethickness);
}



RoundedRectangle::RoundedRectangle(const RoundedRectangle& rr) :
      Rectangle(),
      hrad(0),
      vrad(0)
{
   *this = rr;
}



RoundedRectangle::RoundedRectangle(Rectangle r , const RoundedRectangle& rr) :
      Rectangle(r),
      hrad(rr.hrad),
      vrad(rr.vrad)
{}



RoundedRectangle& RoundedRectangle::operator=(const RoundedRectangle& rr) {
   dynamic_cast<Rectangle&>(*this) = dynamic_cast<const Rectangle&>(rr);
   hrad = rr.hrad;
   vrad = rr.vrad;
   return *this;
}



bool RoundedRectangle::Contains(int xpos , int ypos) const {
   Rectangle topmid(x + hrad , 0        , w - 2*hrad , vrad);
   Rectangle botmid(x + hrad , h - vrad , w - 2*hrad , vrad);
   Rectangle middle(x        , y + vrad , w          , h - 2*vrad);
   Ellipse topleft (x              , y              , 2*hrad , 2*vrad);
   Ellipse topright(x + w - 2*hrad , y              , 2*hrad , 2*vrad);
   Ellipse botleft (x              , y + h - 2*vrad , 2*hrad , 2*vrad);
   Ellipse botright(x + w - 2*hrad , y + h - 2*vrad , 2*hrad , 2*vrad);

   return (topmid.Contains  (xpos,ypos) ||
           botmid.Contains  (xpos,ypos) ||
           middle.Contains  (xpos,ypos) ||
           topleft.Contains (xpos,ypos) ||
           topright.Contains(xpos,ypos) ||
           botleft.Contains (xpos,ypos) ||
           botright.Contains(xpos,ypos));
}



AreaBase& RoundedRectangle::MoveBy(int dx , int dy) {
   return Rectangle::MoveBy(dx,dy);
}



void RoundedRectangle::Draw(EagleGraphicsContext* win , EagleColor color) const {
   win->DrawRoundedRectangle(x,y,w,h,hrad,vrad,1.0,color);
}



void RoundedRectangle::Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const {
   win->DrawRoundedRectangle(x,y,w,h,hrad,vrad,thickness,color);
}



void RoundedRectangle::Fill(EagleGraphicsContext* win , EagleColor color) const {
   win->DrawFilledRoundedRectangle(x,y,w,h,hrad,vrad,color);
}



std::vector<Rectangle*> RoundedRectangle::CreateBoundingRectangles() const {
   std::vector<Rectangle*> rects;
   rects.push_back(new Rectangle(x,y,w,h));
   return rects;
}



AreaBase* RoundedRectangle::Clone() const {
   return new RoundedRectangle(*this);
}



std::ostream& RoundedRectangle::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "Rounded ";
   Rectangle::DescribeTo(os);
   os << std::endl << indent << "(hrad,vrad) = (" << hrad << "," << vrad << ")" << std::endl;
   return os;
}



void RoundedRectangle::SetRadii(int hradius , int vradius) {
   if (hradius < 0) {hradius = 0;}
   if (vradius < 0) {vradius = 0;}
   if (hradius > w/2) {hradius = w/2;}
   if (vradius > h/2) {vradius = h/2;}
   hrad = hradius;
   vrad = vradius;
}



void RoundedRectangle::SetRoundingPercent(float hp , float vp) {
   if (hp < 0.0f) {hp = 0.0f;}
   if (hp > 1.0f) {hp = 1.0f;}
   if (vp < 0.0f) {vp = 0.0f;}
   if (vp > 1.0f) {vp = 1.0f;}
   float hradius = hp*w/2;
   float vradius = vp*h/2;
   SetRadii((int)hradius , (int)vradius);
}



/// -------------------------------       Globals      -----------------------------------------




RECT_CONNECT_TYPE DoRectanglesConnect(const Rectangle& r1 , const Rectangle& r2) {
   if ((r1.X() == r2.X()) && (r1.BRX() == r2.BRX())) {
      if (r2.BRY() + 1 == r1.Y()) {return RECT_CONNECT_R2_ON_TOP;}
      if (r1.BRY() + 1 == r2.Y()) {return RECT_CONNECT_R2_ON_BOTTOM;}
   }
   if ((r1.Y() == r2.Y()) && (r1.BRY() == r2.BRY())) {
      if (r1.BRX() + 1 == r2.X()) {return RECT_CONNECT_R2_ON_RIGHT;}
      if (r2.BRX() + 1 == r1.X()) {return RECT_CONNECT_R2_ON_LEFT;}
   }
   return RECT_CONNECT_NONE;
}



RECT_CONTAINS_TYPE DoRectanglesContain(const Rectangle& r1 , const Rectangle& r2) {
   if (r1.Contains(r2)) {return R1_CONTAINS_R2;}
   if (r2.Contains(r1)) {return R2_CONTAINS_R1;}
   return CONTAINS_NONE;
}



Rectangle GetOverlap(const Rectangle& r1 , const Rectangle& r2) {
   if (!r1.Overlaps(r2)) {
      return BADRECTANGLE;
   }
   /// Overlapping rectangle is created by adjoining the rightmost left edge with the leftmost right edge,
   /// and the topmost bottom edge and bottom most top edge
   const int lx = (r1.X() > r2.X())?r1.X():r2.X();
   const int rx = (r2.BRX() > r1.BRX())?r1.BRX():r2.BRX();
   const int ty = (r1.Y() > r2.Y())?r1.Y():r2.Y();
   const int by = (r2.BRY() > r1.BRY())?r1.BRY():r2.BRY();
   return Rectangle(lx , ty , rx - lx + 1 , by - ty + 1);
}



list<Rectangle> CombineRectangles(Rectangle r1 , Rectangle r2) {
   list<Rectangle> rlist;

   // Contained Rectangles
   RECT_CONTAINS_TYPE contype = DoRectanglesContain(r1,r2);
   if (contype) {
      if (contype == R1_CONTAINS_R2) {rlist.push_back(r1);}
      if (contype == R2_CONTAINS_R1) {rlist.push_back(r2);}
      return rlist;
   }

   // Connecting Rectangles
   RECT_CONNECT_TYPE rctype = DoRectanglesConnect(r1,r2);
   if (rctype) {
      if (rctype == RECT_CONNECT_R2_ON_TOP) {
         rlist.push_back(Rectangle(r2.X() , r2.Y() , r2.W() , r1.H() + r2.H()));
      }
      if (rctype == RECT_CONNECT_R2_ON_BOTTOM) {
         rlist.push_back(Rectangle(r1.X() , r1.Y() , r1.W() , r1.H() + r2.H()));
      }
      if (rctype == RECT_CONNECT_R2_ON_RIGHT) {
         rlist.push_back(Rectangle(r1.X() , r1.Y() , r1.W() + r2.W() , r1.H()));
      }
      if (rctype == RECT_CONNECT_R2_ON_LEFT) {
         rlist.push_back(Rectangle(r2.X() , r2.Y() , r1.W() + r2.W() , r2.H()));
      }
      return rlist;
   }
   
   // Overlapping Rectangles
   /// w = brx - x + 1
   /// h = bry - y + 1
   if (r1.Overlaps(r2)) {
      // Overlapping aligned Rectangles combine to produce one Rectangle
      // Vertical alignment
      if ((r1.X() == r2.X()) && (r1.BRX() == r2.BRX())) {
         const Rectangle* top = (r1.Y() < r2.Y())?&r1:&r2;
         const Rectangle* bot = (r1.BRY() > r2.BRY())?&r1:&r2;
         rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , bot->BRY() - top->Y() + 1));
         return rlist;
      }
      // Horizontal alignment
      if ((r1.Y() == r2.Y()) && (r1.BRY() == r2.BRY())) {
         const Rectangle* left = (r1.X() < r2.X())?&r1:&r2;
         const Rectangle* right = (r1.BRX() > r2.BRX())?&r1:&r2;
         rlist.push_back(Rectangle(left->X() , left->Y() , right->BRX() - left->X() + 1 , left->H()));
         return rlist;
      }
      // Overlapping offset Rectangles
      const Rectangle* temin = (r1.Y() < r2.Y())?&r1:&r2;// top edge min y value
      const Rectangle* temax = (r1.Y() > r2.Y())?&r1:&r2;// top edge max y value
      const Rectangle* lemin = (r1.X() < r2.X())?&r1:&r2;// left edge min x value
      const Rectangle* lemax = (r1.X() > r2.X())?&r1:&r2;// left edge max x value
      const Rectangle* bemin = (r1.BRY() < r2.BRY())?&r1:&r2;// bottom edge min y value
      const Rectangle* bemax = (r1.BRY() > r2.BRY())?&r1:&r2;// bottom edge max y value
      const Rectangle* remin = (r1.BRX() < r2.BRX())?&r1:&r2;// right edge min x value
      const Rectangle* remax = (r1.BRX() > r2.BRX())?&r1:&r2;// right edge max x value
      if (r1.Y() == r2.Y()) {
         // When the tops are even, only two unique Rectangles are formed
         // new top Rectangle formed by min left edge , max right edge , either top edge , and the min bottom edge
         rlist.push_back(Rectangle(lemin->X() , r1.Y() , remax->BRX() - lemin->X() + 1 , bemin->BRY() - r1.Y() + 1));
         // new bot Rectangle formed by bottom Rectangles left and right edges , top.BRY() + 1 , and bot.BRY()
         rlist.push_back(Rectangle(bemax->X() , bemin->BRY() + 1 , bemax->BRX() - bemax->X() + 1 , bemax->BRY() - (bemin->BRY() + 1) + 1));
      } else if (r1.BRY() == r2.BRY()) {
         // When the bottoms are even, only two unique Rectangles are formed
         // new top Rectangle formed by top Rectangles left and right edges , top.Y() and bot.Y() - 1
         rlist.push_back(Rectangle(temin->X() , temin->Y() , temin->BRX() - temin->X() + 1 , (temax->Y() - 1) - temin->Y() + 1));
         // new bot Rectangle formed by min left edge , max right edge , bot.Y() and either bottom edge
         rlist.push_back(Rectangle(lemin->X() , temax->Y() , remax->BRX() - lemin->X() + 1 , r1.BRY() - temax->Y() + 1));
      } else if (r1.X() == r2.X()) {
         // When the left edges are even, two vertical Rectangles are formed
         rlist.push_back(Rectangle(r1.X() , temin->Y() , remin->BRX() - r1.X() + 1 , bemax->BRY() - temin->Y() + 1));
         rlist.push_back(Rectangle(remin->BRX() + 1 , remax->Y() , remax->BRX() - (remin->BRX() + 1) + 1 , remax->H()));
      } else if (r1.BRX() == r2.BRX()) {
         // When the right edges are even, two vertical Rectangles are formed
         rlist.push_back(Rectangle(lemin->X() , lemin->Y() , (lemax->X() - 1) - lemin->X() + 1 , lemin->H()));
         rlist.push_back(Rectangle(lemax->X() , temin->Y() , r1.BRX() - lemax->X() + 1 , bemax->BRY() - temin->Y() + 1));
      } else {
         // One Rectangle is on top and one Rectangle is on left
         const Rectangle* top = temin;
         const Rectangle* bot = temax;
         const Rectangle* left = lemin;
         const Rectangle* right = lemax;
         if (((top->Y() <= bot->Y()) && (top->BRY() >= bot->BRY())) && ((left->X() <= right->X()) && (left->BRX() >= right->BRX()))) {
            // The two Rectangles cross in the middle, forming three Rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , (bot->Y() - 1) - top->Y() + 1));
            rlist.push_back(Rectangle(bot->X() , bot->Y() , bot->W() , bot->H()));
            rlist.push_back(Rectangle(top->X() , bot->BRY() + 1 , top->W() , top->BRY() - (bot->BRY() + 1) + 1));
         } else if ((top->X() <= bot->X()) && (top->BRX() >= bot->BRX())) {
            // Upper Rectangle horizontally surrounds lower Rectangle, forming two Rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , top->H()));
            rlist.push_back(Rectangle(bot->X() , top->BRY() + 1 , bot->W() , bot->BRY() - (top->BRY() + 1) + 1));
         } else if ((bot->X() <= top->X()) && (bot->BRX() >= top->BRX())) {
            // Lower Rectangle horizontally surrounds upper Rectangle, forming two Rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , (bot->Y() - 1) - top->Y() + 1));
            rlist.push_back(Rectangle(bot->X() , bot->Y() , bot->W() , bot->H()));
         } else if ((left->Y() <= right->Y()) && (left->BRY() >= right->BRY())) {
            // Left Rectangle vertically surrounds right Rectangle, forming two Rectangles
            rlist.push_back(Rectangle(left->X() , left->Y() , left->W() , left->H()));
            rlist.push_back(Rectangle((left->BRX() + 1) , right->Y() , right->BRX() - (left->BRX() + 1) + 1 , right->H()));
         } else if ((right->Y() <= left->Y()) && (right->BRY() >= left->BRY())) {
            // Right Rectangle vertically surrounds left Rectangle, forming two Rectangles
            rlist.push_back(Rectangle(left->X() , left->Y() , (right->X() - 1) - left->X() + 1 , left->H()));
            rlist.push_back(Rectangle(right->X() , right->Y() , right->W() , right->H()));
         } else {
            // only one corner of each overlaps the other, forming three Rectangles
            rlist.push_back(Rectangle(top->X() , top->Y() , top->W() , (bot->Y() - 1) - top->Y() + 1));
            rlist.push_back(Rectangle(lemin->X() , bot->Y() , remax->BRX() - lemin->X() + 1 , top->BRY() - bot->Y() + 1));
            rlist.push_back(Rectangle(bot->X() , top->BRY() + 1 , bot->W() , bot->BRY() - (top->BRY() + 1) + 1));
         }
      }
      return rlist;
   }

   /// If it gets this far, the two Rectangles can not be combined, so return an empty list to signal this
   ///rlist.push_back(r1);
   ///rlist.push_back(r2);
   return rlist;
}



/** Fallback version of ConsolidateRectangles, Keep for now
list<Rectangle> ConsolidateRectangles(list<Rectangle> rectlist) {
   if (rectlist.size() <= 1) {return rectlist;}
   
   list<Rectangle>::iterator it1 , it2 , add;
   for (it1 = rectlist.begin() ; it1 != rectlist.end() ; ++it1) {
      while (true) {
         ++it2;
         if (it2 == rectlist.end()) {break;}
         list<Rectangle> clist = CombineRectangles(*it1 , *it2);
         if (!clist.empty()) {
            rectlist.erase(it1);
            rectlist.erase(it2);
            for (add = clist.begin() ; add != clist.end() ; ++add) {
               rectlist.push_back(*add);
            }
            it1 = rectlist.begin();
            it2 = it1;
         }
      }
   }
   return rectlist;
}
//*/



list<Rectangle> ConsolidateRectangles(list<Rectangle> rectlist) {
   if (rectlist.size() <= 1) {return rectlist;}
   
   list<Rectangle>::iterator it1 , it2 , next;
   list<Rectangle> addlist;
   it1 = rectlist.begin();
   while (it1 != rectlist.end()) {
      it2 = it1;
      while (true) {
         ++it2;
         if (it2 == rectlist.end()) {break;}
         list<Rectangle> clist = CombineRectangles(*it1 , *it2);
         if (!clist.empty()) {
            next = it1;
            ++next;
            if (next == it2) {++next;}
            rectlist.erase(it1);
            rectlist.erase(it2);
            addlist.splice(addlist.end() , clist);
            it1 = next;
            it2 = it1;
            if (next == rectlist.end()) {break;}
         }
      }
      next = it1;
      if (it1 != rectlist.end()) {
         ++it1;
         ++next;
      }
      if (next == rectlist.end()) {
         if (!addlist.empty()) {
            rectlist.splice(rectlist.end() , addlist);
            it1 = rectlist.begin();
            it2 = it1;
         }
      }
   }
   
   return rectlist;
}




bool SubtractRectangle(Rectangle sub , Rectangle from , std::list<Rectangle>& sublist , std::list<Rectangle>& fromlist) {
   sublist.clear();
   fromlist.clear();
   Rectangle overlap = GetOverlap(sub , from);
   if (overlap == BADRECTANGLE) {
      /// No overlap, we can't subtract this Rectangle
      return false;
   }
   RECT_CONTAINS_TYPE ctype = DoRectanglesContain(sub , from);
   
   switch (ctype) {
   case CONTAINS_NONE :
      /// Rectangles overlap but do not consume each other
      sublist = SubtractRectangle(overlap , sub);
      fromlist = SubtractRectangle(overlap , from);
      break;
   case R2_CONTAINS_R1 :
      /// From rectangle surrounds sub rectangle
      fromlist = SubtractRectangle(sub , from);
      break;
   case R1_CONTAINS_R2 :
      /// Sub rectangle surrounds from rectangle
      sublist = SubtractRectangle(from , sub);
      break;
   default :
      return false;
      break;
   };
   return true;
}

std::list<Rectangle> SubtractRectangle(Rectangle sub , Rectangle from) {
   std::list<Rectangle> rlist;
   Rectangle overlap = GetOverlap(sub , from);
   if (overlap == BADRECTANGLE) {
      rlist.push_back(from);
      return rlist;
   }
   if (overlap == from) {
      return rlist;/// From is totally subtracted by overlap
   }
   int nmatch = 0;
   bool matchleft = overlap.X() == from.X();
   bool matchright = overlap.BRX() == from.BRX();
   bool matchtop = overlap.Y() == from.Y();
   bool matchbottom = overlap.BRY() == from.BRY();
   nmatch += matchleft?1:0;
   nmatch += matchright?1:0;
   nmatch += matchtop?1:0;
   nmatch += matchbottom?1:0;
   
   switch (nmatch) {
   case 0 :/// overlap is contained by the from rectangle, forming 4 rectangles
      rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// top
      rlist.push_back(Rectangle(from.X() , overlap.Y() , overlap.X() - from.X() , overlap.H()));/// left
      rlist.push_back(Rectangle(overlap.BRX() + 1 , overlap.Y() , from.BRX() - overlap.BRX() , overlap.H()));/// right
      rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.BRY() - overlap.BRY()));/// bottom
      break;
   case 1 :/// Only one side matches, creating 3 rectangles
      if (matchleft || matchright) {
         rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// above
         rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.BRY() - overlap.BRY()));/// below
         if (matchleft) {
            rlist.push_back(Rectangle(overlap.BRX() + 1 , overlap.Y() , from.BRX() - overlap.BRX() , overlap.H()));/// right
         }
         else {// matchright
            rlist.push_back(Rectangle(from.X() , overlap.Y() , overlap.X() - from.X() , overlap.H()));/// left
         }
      }
      else if (matchtop) {
         rlist.push_back(Rectangle(from.X() , from.Y() , overlap.X() - from.X() , overlap.H()));/// left
         rlist.push_back(Rectangle(overlap.BRX() + 1 , from.Y() , from.BRX() - overlap.BRX() , overlap.H()));/// right
         rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.Y() - overlap.Y()));/// bottom
      }
      else if (matchbottom) {
         rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// top
         rlist.push_back(Rectangle(from.X() , overlap.Y() , overlap.X() - from.X() , overlap.H()));/// left
         rlist.push_back(Rectangle(overlap.BRX() + 1 , overlap.Y() , from.BRX() - overlap.BRX() , overlap.H()));/// right
      }
      break;
   case 2 :
      if (matchright) {
         if (matchtop) {/// NE match
            rlist.push_back(Rectangle(from.X() , from.Y() , overlap.X() - from.X() , overlap.H()));/// NW corner
            rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.BRY() - overlap.BRY()));/// S rectangle
         }
         else if (matchbottom) {/// SE match
            rlist.push_back(Rectangle(from.X() , overlap.Y() , overlap.X() - from.X() , overlap.H()));/// SW corner
            rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// N rectangle
         }
         else if (matchleft) {/// EW match
            rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// top
            rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.BRY() - overlap.BRY()));/// bottom
         }
      }
      else if (matchleft) {
         if (matchtop) {/// NW match
            rlist.push_back(Rectangle(overlap.BRX() + 1 , from.Y() , from.BRX() - overlap.BRX() , overlap.H()));/// NE corner
            rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.BRY() - overlap.BRY()));/// S rectangle
         }
         else if (matchbottom) {/// SW match
            rlist.push_back(Rectangle(overlap.BRX() + 1 , overlap.Y() , from.BRX() - overlap.BRX() , overlap.H()));/// SE corner
            rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// N rectangle
         }
      }
      else if (matchtop && matchbottom) {/// NS match
         rlist.push_back(Rectangle(from.X() , from.Y() , overlap.X() - from.X() , from.H()));/// left
         rlist.push_back(Rectangle(overlap.BRX() + 1 , from.Y() , from.BRX() - overlap.BRX() , from.H()));/// right
      }
      break;
   case 3 :
      if (!matchbottom) {/// bottom
         rlist.push_back(Rectangle(from.X() , overlap.BRY() + 1 , from.W() , from.BRY() - overlap.BRY()));/// S rectangle
      }
      else if (!matchtop) {/// top
         rlist.push_back(Rectangle(from.X() , from.Y() , from.W() , overlap.Y() - from.Y()));/// N rectangle
      }
      else if (!matchleft) {/// right
         rlist.push_back(Rectangle(from.X() , from.Y() , overlap.X() - from.X() , from.H()));/// W rectangle
      }
      else if (!matchright) {/// left
         rlist.push_back(Rectangle(overlap.BRX() + 1 , from.Y() , from.BRX() - overlap.BRX() , from.H()));/// E rectangle
      }
      break;
   };

   return rlist;
}



/// --------------------------------     Ellipse    ---------------------------------------



Ellipse::Ellipse(Rectangle r) :
      Rectangle(r)
{}



Ellipse::Ellipse(int ex , int ey , int ew , int eh) :
      Rectangle(ex,ey,ew,eh)
{}


   
bool Ellipse::Contains(int xpos , int ypos) const {
   float a = w/2.0f;
   float b = h/2.0f;
   float cx = x + a;
   float cy = y + b;

   float x2 = xpos - cx;
   float y2 = ypos - cy;
   if (x2 == 0.0f && y2 == 0.0f) {return false;}
   
//   float theta = atan2(toypos,toxpos);
///   (y1 – y2)x + (x2 – x1)y + (x1y2 – x2y1) = 0
   /// (x^2 / a^2) + (y^2 / b^2) = 1;
   /// y^2 = (a^2*b^2*y2^2)/(b^2*x2^2 + y2^2*a^2)
   /// x^2 = (a^2*b^2*x2^2)/(b^2*x2^2 + y2^2*a^2)
   const float asqbsq = a*a*b*b;
   const float denom = b*b*x2*x2 + y2*y2*a*a;
   const float factor = asqbsq/denom;
   const float y2sq = y2*y2;
   const float x2sq = x2*x2;
   const float ysq = y2sq*factor;
   const float xsq = x2sq*factor;
   const float ellipse_distance_squared = xsq + ysq;
   const float radial_distance_squared = x2sq + y2sq;
   return radial_distance_squared < ellipse_distance_squared;
}



void Ellipse::Draw(EagleGraphicsContext* win , EagleColor color) const {
   float hrad = w/2.0f;
   float vrad = h/2.0f;
   float cx = x + hrad;
   float cy = y + vrad;
   win->DrawEllipse(cx,cy,hrad,vrad,1.0f,color);
}



void Ellipse::Draw(EagleGraphicsContext* win , double thickness , EagleColor color) const {
   float hrad = w/2.0f;
   float vrad = h/2.0f;
   float cx = x + hrad;
   float cy = y + vrad;
   win->DrawEllipse(cx,cy,hrad,vrad,thickness,color);
}



void Ellipse::Fill(EagleGraphicsContext* win , EagleColor color) const {
   float hrad = w/2.0f;
   float vrad = h/2.0f;
   float cx = x + hrad;
   float cy = y + vrad;
   win->DrawFilledEllipse(cx,cy,hrad,vrad,color);
}



AreaBase* Ellipse::Clone() const {
   return new Ellipse(*this);
}



std::ostream& Ellipse::DescribeTo(std::ostream& os , Indenter indent) const {
   float hrad = w/2.0f;
   float vrad = h/2.0f;
   float cx = x + hrad;
   float cy = y + vrad;
   os << indent << "Ellipse (cx,cy,hrad,vrad) = (" << cx << "," << cy << "," << hrad << "," << vrad << ") ";
   Rectangle::DescribeTo(os);
   os << endl;
   return os;
}






