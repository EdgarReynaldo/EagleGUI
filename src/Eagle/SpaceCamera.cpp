
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file SpaceCamera.cpp
 * @brief Implementation of the space camera class
 */


#include "Eagle/SpaceCamera.hpp"

#include "Eagle/Math.hpp"



SpaceCamera::SpaceCamera() :
      info(START),
      hfov(90.0),
      aspect(1.6),
      ortho(false)
{}

SpaceCamera::SpaceCamera(Vec3 pos , Orient o , double hview , double aspect_ratio) :
      info(pos,o),
      hfov(hview),
      aspect(aspect_ratio),
      ortho(false)
{}

void SpaceCamera::SetPosition(double x , double y , double z) {
   info.pos.x = x;
   info.pos.y = y;
   info.pos.z = z;
}
void SpaceCamera::SetPosition(Vec3 p) {
   info.pos = p;
}


void SpaceCamera::SetOrientation(Orient o) {
   info.orient = o;
}
void SpaceCamera::SetHFOV(double v) {
   hfov = v;
}
void SpaceCamera::SetAspect(double a) {
   aspect = a;
}
void SpaceCamera::LookAt(double x , double y , double z) {

}

void SpaceCamera::Move(Vec3 vel , double dt) {
   vel*=dt;
   Vec3 fw = info.orient.Fw();
   Vec3 rt = info.orient.Rt();
   Vec3 up = info.orient.Up();
   info.pos += rt*vel.x + up*vel.y + fw*vel.z;
}



void SpaceCamera::Turn(Vec3 omega , double dt) {
   info.orient.TurnLocal(omega , dt);
}


Vec3 OrbitOrigin(double distance , double yaxisrotation , double ascension) {
   /// sine(ascension) = yheight/distance;
   double y = distance*sin(ascension*M_PI/180.0);
   double proj = distance*cos(ascension*M_PI/180.0);
   double z = proj*sin(yaxisrotation*M_PI/180.0);
   double x = proj*cos(yaxisrotation*M_PI/180.0);
   return Vec3(x,y,z);
}




