



#ifndef Orient_HPP
#define Orient_HPP

#include "Vec3.hpp"


class Orient {

protected :
   Vec3 theta;/// yaw,pitch,roll
   
   Vec3 fw,rt,up;

   void CalcTheta();
   void CalcAxes();
   
public :
   
   
   Orient() : theta(0,0,0) , fw(FORWARD) , rt(RIGHT) , up(UP) {}
   Orient(Vec3 angles) : theta() , fw(FORWARD) , rt(RIGHT) , up(UP) {SetTheta(angles);}
   
   void Reset() {*this = Orient();}
   
   void SetTheta(Vec3 t) {theta = t;CalcAxes();}
   
   void Turn(Vec3 omega , double dt);
   void TurnLocal(Vec3 omega , double dt);
   
   inline Vec3 Fw() const {return fw;}
   inline Vec3 Rt() const {return rt;}
   inline Vec3 Up() const {return up;}
   inline Vec3 Theta() const {return theta;}
   
   friend Orient LocalTurn(const Orient& o , Vec3 omega , double dt);
   friend Vec3 CalcTheta(const Orient& o);
};


Vec3 CalcTheta(const Orient& o);




/**
class TurnMove {
public :
   Vec3 pos,vel,acc;
   
   TurnMove() : pos() , vel() , acc() {}
   TurnMove(Vec3 p) : pos(p) , vel() , acc() {}
   TurnMove(Vec3 p , Vec3 v , Vec3 a) : pos(p) , vel(v) , acc(a) {}
   
   
   inline TurnMove Future(double dt) const {
      return TurnMove(pos + vel*dt + acc*0.5*dt*dt , acc*dt , acc);
   }
   
   void Update(double dt) {
      *this = Future(dt);
   }
};
*/


#endif // Orient_HPP
