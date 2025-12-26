



#ifndef Artillery_HPP
#define Artillery_HPP


#include "Eagle/Color.hpp"



class AreaBase;
class EagleGraphicsContext;


class Artillery {
public :
   double cx;
   double cy;
   AreaBase* area;
   EagleColor fillcolor;
   EagleColor drawcolor;
   
   Artillery();
   void Setup(double cxpos , double cypos , AreaBase* shape , EagleColor fill , EagleColor draw);
   
   
   virtual void Launch(double angle_degrees , double power);

   virtual void DisplayOn(EagleGraphicsContext* win);
};

#endif // Artillery_HPP
