
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



/// -------------------------------------   Pos2D    -------------------------------------------



void Pos2d::Draw(EagleGraphicsContext* win , EagleColor color) const {
   EAGLE_ASSERT(win);
   win->PutPixel(x , y , color);
}



void Pos2d::DrawLineTo(EagleGraphicsContext* win , const Pos2d& p , EagleColor color) const {
   EAGLE_ASSERT(win);
   win->DrawLine(x , y , p.x , p.y , color);
}



std::ostream& operator<<(std::ostream& os , const Pos2d& p) {
   os << p.x << " , " << p.y;
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
      angle_ab = AngleToP2(p1,p2);// -PI to PI
      angle_bc = AngleToP2(p2,p3);// -PI to PI
      angle_ca = AngleToP2(p3,p1);// -PI to PI
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
   Pos2d p4(xpos,ypos);
   const Pos2d* pts[3] = {&p1 , &p2 , &p3};
   double angles[3] = {angle_ab , angle_bc , angle_ca};
   double turn_angle = 0.0;
   double angle_to_p4 = 0.0;
   for (int i = 0 ; i < 3 ; ++i) {
      if (*pts[i] == p4) {return true;}
      angle_to_p4 = AngleToP2(*pts[i] , p4);// -PI to PI
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



void Triangle::Fill(EagleGraphicsContext* win , EagleColor color) const {
   EAGLE_ASSERT(win);
   if (points_make_triangle) {
      win->DrawFilledTriangle(p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y() , color);
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




/// -----------------------------------      Circle clsss         -----------------------------------------



Circle::Circle() : x(0) , y(0) , r(0) {}



Circle::Circle(int xpos , int ypos , int radius) : x(xpos) , y(ypos) , r(radius) {}



bool Circle::Contains(int xpos , int ypos) const {
   float dx = (float)(xpos - x);
   float dy = (float)(ypos - y);
   float dist = sqrt(dx*dx + dy*dy);
   if (dist <= (float)r) {return true;}
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


using std::ostream;
using std::endl;


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



void Rectangle::Draw(EagleGraphicsContext* win , EagleColor color) const {
	win->DrawRectangle(x,y,w,h,1,color);
}



void Rectangle::Fill(EagleGraphicsContext* win , EagleColor color) const {
   win->DrawFilledRectangle(x , y , w , h , color);
///   rectfill(bmp , x , y , brx , bry , color);
}



void Rectangle::DrawInnerFrame(EagleGraphicsContext* win , unsigned int width , EagleColor color) const {
   EAGLE_ASSERT(win);
   const int len = width - 1;
   win->DrawFilledRectangle(x         , y         , brx     , y + len     , color);
   win->DrawFilledRectangle(x         , y + width , x + len , bry - width , color);
   win->DrawFilledRectangle(brx - len , y + width , brx     , bry - width , color);
   win->DrawFilledRectangle(x         , bry - len , brx     , bry         , color);

/** Old
   ASSERT(bmp);
   for (unsigned int i = 0 ; i < width ; ++i) {
      rect(bmp , x + i , y + i , brx - i , bry - i , color);
   }
//*/
}



void Rectangle::RoundedFill(EagleGraphicsContext* win , int corner_radius , EagleColor color) const {
   EAGLE_ASSERT(win);
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
   win->DrawFilledRoundedRectangle(x , y , w , h , corner_radius , corner_radius , color);
   
}



void Rectangle::RoundedOutline(EagleGraphicsContext* win , int corner_radius  , EagleColor color) const {
	EAGLE_ASSERT(win);
	win->DrawRoundedRectangle(x , y , w , h , corner_radius , corner_radius , 1.0 , color);
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



void Rectangle::DottedOutline(EagleGraphicsContext* win , EagleColor color , bool even) const {
	EAGLE_ASSERT(win);
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
   Rectangle upper(x , y , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.Fill(win , sd_color);
   upper.Fill(win , fg_color);
}



void Rectangle::DrawGuiRectDown(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   Rectangle upper(x + 1 , y + 1 , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.Fill(win , sd_color);
   upper.Fill(win , fg_color);
}



void Rectangle::DrawGuiCircleUp(EagleGraphicsContext* win , int radius , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   int cx = x + w/2;
   int cy = y + h/2;
   win->DrawFilledCircle(cx     , cy     , radius     , sd_color);
   win->DrawFilledCircle(cx + 1 , cy + 1 , radius - 1 , sd_color);
   win->DrawFilledCircle(cx - 1 , cy - 1 , radius - 2 , fg_color);
}



void Rectangle::DrawGuiCircleDown(EagleGraphicsContext* win , int radius , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   int cx = x + w/2;
   int cy = y + h/2;
   win->DrawFilledCircle(cx , cy , radius     , sd_color);
   win->DrawFilledCircle(cx , cy , radius - 2 , fg_color);
}



void Rectangle::DrawGuiRoundedUp(EagleGraphicsContext* win , int radius , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   Rectangle upper(x , y , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.RoundedFill(win , radius , sd_color);
   upper.RoundedFill(win , radius , fg_color);
}



void Rectangle::DrawGuiRoundedDown(EagleGraphicsContext* win , int radius , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   Rectangle upper(x + 1 , y + 1 , w - 2 , h - 2);
   Rectangle lower(x + 2 , y + 2 , w - 2 , h - 2);
   lower.RoundedFill(win , radius , sd_color);
   upper.RoundedFill(win , radius , fg_color);
}



void Rectangle::DrawGuiEllipseUp(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   int cx = x + w/2;
   int cy = y + h/2;
   if (w > 1 && h > 1) {
      win->DrawFilledEllipse(cx + 1 , cy + 1 , w/2 , h/2 , sd_color);
      win->DrawFilledEllipse(cx - 1 , cy - 1 , w/2 - 1 , h/2 - 1 , fg_color);
   }
}



void Rectangle::DrawGuiEllipseDown(EagleGraphicsContext* win , EagleColor fg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
   int cx = x + w/2;
   int cy = y + h/2;
   win->DrawFilledEllipse(cx , cy , w/2 - 1 , h/2 - 1 , sd_color);
   win->DrawFilledEllipse(cx , cy , w/2 - 2 , h/2 - 2 , fg_color);
}



void Rectangle::DrawGuiTriangleFaceUp   (EagleGraphicsContext* win , EagleColor fg_color , EagleColor bg_color , EagleColor sd_color) const {
	EAGLE_ASSERT(win);
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
	win->DrawShadedRectangle(this , tlcol , trcol , brcol , blcol);
}


   
void Rectangle::DrawGradientFrameTo(EagleGraphicsContext* win , const Rectangle* r2 , EagleColor start_color , EagleColor finish_color) const {
	const Rectangle* r1 = this;
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







