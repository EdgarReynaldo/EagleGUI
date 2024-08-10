
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
 * @file Vec3.cpp
 * @brief Implementation of the Vec3 vector class
 */




#include "Eagle/Vec3.hpp"
#include "Eagle/Transforms.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Lib.hpp"

#include <cstdio>


const Vec3 ORIGIN(0,0,0);

const Vec3 UP(0,1,0);
const Vec3 RIGHT(1,0,0);
const Vec3 FORWARD(0,0,1);



Vec3 Rotate3D(const Vec3& vec , const Vec3& axis , const double theta_radians) {
/**
   /// TODO : This uses allegro's transformation matrices, which only use floating point precision
   /// TODO : This could be a source of rounding errors
   ALLEGRO_TRANSFORM t;
   
   al_identity_transform(&t);
   al_rotate_transform_3d(&t , axis.x , axis.y , axis.z , theta_radians);
   
   /// Note the use of floats here
   float fx = vec.x;
   float fy = vec.y;
   float fz = vec.z;
   
   al_transform_coordinates_3d(&t , &fx , &fy , &fz);
*/

   double dx = vec.x;
   double dy = vec.y;
   double dz = vec.z;

   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   Transform t = sys->GetSystemTransformer()->GetIdentityTransform();
   t.Rotate(theta_radians , axis.x , axis.y , axis.z);
   t.ApplyTransformation(&dx , &dy , &dz);
   
   return Vec3(dx,dy,dz);
}



bool ScanVec3(const char* str , Vec3& v) {
   return 3 == sscanf(str , "%lf %lf %lf" , &v.x , &v.y , &v.z);
}



