



#ifndef KObject_HPP
#define KObject_HPP


#include "Eagle/Area.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"



class KObject {
friend class KMaster;

protected:
   LayoutRectangle objarea;
   double vx;
   double vy;
   double speed;
   double heading_degrees;
   double bearing_degrees;
   
   
   
   
public:
   
   
};

#endif // KObject_HPP
