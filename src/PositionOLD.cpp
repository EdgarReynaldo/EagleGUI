



#include "Eagle/Position.hpp"
#include "Eagle/GraphicsContext.hpp"


/// -------------------------------------   Pos2D    -------------------------------------------



void Pos2d::Draw(EagleGraphicsContext* win , EagleColor color) const {
   EAGLE_ASSERT(win);
   win->PutPixel(x , y , color);
}



void Pos2d::DrawLineTo(EagleGraphicsContext* win , const Pos2d& p , EagleColor color) const {
   EAGLE_ASSERT(win);
   win->DrawLine(x , y , p.x , p.y , color);
}



void Pos2d::DrawLineTo(EagleGraphicsContext* win , const Pos2d& p , double thickness , EagleColor color) const {
   EAGLE_ASSERT(win);
   win->DrawLine(x , y , p.x , p.y , thickness , color);
}



std::ostream& operator<<(std::ostream& os , const Pos2d& p) {
   os << p.x << " , " << p.y;
   return os;
}


