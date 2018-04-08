


#include "Eagle/Position.hpp"

Pos2F VectorF(const Pos2F& p , float r , float theta) {
   return Pos2F(p.X() + r*cos(theta) , p.Y() + r*sin(theta));
}



Pos2D Vector(const Pos2D& p , double r , double theta) {
   return Pos2D(p.X() + r*cos(theta) , p.Y() + r*sin(theta));
}
