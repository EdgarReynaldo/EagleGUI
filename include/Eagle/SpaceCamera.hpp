
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
 * @file SpaceCamera.hpp
 * @brief A camera for moving and viewing space in your universe
 * 
 *
 */


#include "Eagle/SpatialInfo.hpp"




Vec3 OrbitOrigin(double distance , double yaxisrotation , double ascension);/// Pass degrees (0-360) , (-90,90)



class SpaceCamera {

protected :
   SpatialInfo info;/// position and orientation

   double hfov;/// In degrees
   double aspect;/// w/h eg 1.3333333 (4/3)
   
   bool ortho;/// whether or not we are in orthographic view
   
public :
   SpaceCamera();
   SpaceCamera(Vec3 pos , Orient o , double hview , double aspect_ratio);

   void SetPosition(double x , double y , double z);
   void SetPosition(Vec3 p);

   void SetOrientation(Orient o);

   void SetHFOV(double v);
   void SetAspect(double a);
   void LookAt(double x , double y , double z);

   void Move(Vec3 vel , double dt);/// relative to orientation
   void Turn(Vec3 omega , double dt);/// relative to orientation

   /// Our current position and orientation
   inline const SpatialInfo& Info() {return info;}
   inline Vec3 Pos() {return info.pos;}
   inline Orient Orientation() {return info.orient;}
   inline Vec3 Forward() {return info.orient.Fw();}
   inline Vec3 Up() {return info.orient.Up();}
   inline Vec3 Right() {return info.orient.Rt();}
   inline double HFOV() {return hfov;}
   inline double Aspect() {return aspect;}
   inline bool Ortho() {return ortho;}


   
   /// These affect the view and projection matrix
   virtual void Setup2D(int w , int h)=0;
   virtual void Setup3D(bool orthographic)=0;
   virtual void SetupSkyboxView()=0;
   
   /// Draw the three axes at the origin
   virtual void DrawAxes(float axis_length)=0;

};

