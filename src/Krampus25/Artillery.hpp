



#ifndef Artillery_HPP
#define Artillery_HPP


#include "Eagle/Color.hpp"

class Area;
class ArtilleryGame;

class Artillery {
public :
   double cx;
   double cy;
   AreaBase* area;
   EagleColor color;
   
   Artillery();
   void Setup(double cxpos , double cypos , AreaBase* shape , EagleColor c)
   virtual bool Launch(double angle_degrees , double pps , double power);


};

#endif // Artillery_HPP
