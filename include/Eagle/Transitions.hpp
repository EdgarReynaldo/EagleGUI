



#ifndef Transitions_HPP
#define Transitions_HPP

#include "Eagle/AnimationBase.hpp"
#include "Eagle/Interpolate.hpp"
#include "Eagle/Position.hpp"
#include "Eagle/Area.hpp"

class Interpolation {
   
   virtual void Recalculate(double pct)=0;
public :
   virtual ~Interpolation() {}
   void Recalc(double pct);
};



class Translate2 : public Interpolation {
protected :
   
   virtual void Recalculate(double pct);

public :
   Pos2D spt;/// Source point
   Pos2D dpt;/// Destination point
   Pos2D cpt;/// Current point
   
   Translate2(Pos2D src , Pos2D dest);
   Pos2D GetPosition();
};

class Scale2 : public Interpolation {
protected :
   Pos2D sscale;/// Source scale
   Pos2D dscale;/// Destination scale

   virtual void Recalculate(double pct);

public :
   Pos2D cscale;/// Current scale
   
   Scale2(Pos2D src_scale , Pos2D dest_scale);
   Pos2D GetScale();
};

class Rotate2 : public Interpolation {
   
protected :
   double stheta;
   double dtheta;

   virtual void Recalculate(double pct);

public :
   double ctheta;
   
   Rotate2(double start_theta , double dest_theta);
      
   double GetRotation();
};



class Transition : public Interpolation {
protected :
public :
   Translate2 move;
   Scale2 size;
   
   virtual void Recalculate(double pct);
public :
   Transition(Pos2D startpos , Pos2D endpos , Pos2D startsize , Pos2D endsize);
   
   double X() {return move.cpt.X();}
   double Y() {return move.cpt.Y();}
   double W() {return size.cscale.X();}
   double H() {return size.cscale.Y();}
   Rectangle Rect() {return Rectangle(move.cpt.X() , move.cpt.Y() , size.cscale.X() , size.cscale.Y());}
};

#endif // Transitions_HPP



