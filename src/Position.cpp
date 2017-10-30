


#include "Eagle/Position.hpp"

Pos2D Vector(const Pos2D& p , double r , double theta) {
   Pos2D p2 = p;
   p2.MoveBy(r*cos(theta) , r*sin(theta));
   return p2;
}
