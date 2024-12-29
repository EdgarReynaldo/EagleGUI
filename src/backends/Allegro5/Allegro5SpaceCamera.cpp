
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
 * @file Allegro5SpaceCamera.cpp
 * @brief Allegro 5 implementation of the space camera
 */



#include <cmath>

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle/backends/Allegro5/Allegro5SpaceCamera.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_opengl.h"
#include "GL/gl.h"
#include "GL/glu.h"



void Allegro5SpaceCamera::Setup2D(int w , int h) {
   ALLEGRO_TRANSFORM tr;
   al_identity_transform(&tr);
   al_orthographic_transform(&tr , 0 , 0 , -1.0 , w , h , 1.0);
   al_use_projection_transform(&tr);
   
   al_identity_transform(&tr);
   al_use_transform(&tr);
}



void Allegro5SpaceCamera::Setup3D(bool orthographic) {
   ortho = orthographic;
   ALLEGRO_TRANSFORM proj;
   ALLEGRO_TRANSFORM cam;
   
   const double nearby = 0.1;
   const double w = nearby*tan(hfov/2.0);

///   const double l = -w;
///   const double r = w;
   
   /** l and r are reversed to fix the axes */
   const double l = w;
   const double r = -w;
   
   const double top = w/aspect;
   const double bot = -w/aspect;
   const double farther = 100.0;

   
   al_identity_transform(&proj);
   if (!ortho) {
      al_perspective_transform(&proj , l , top , nearby , r , bot , farther);
   }
   else {
      int diam = 500;
      al_orthographic_transform(&proj , -diam , diam , -diam , diam , -diam , diam);
   }
   al_use_projection_transform(&proj);
   
   Vec3 eye = info.pos;
   Vec3 look = info.pos + info.orient.Fw();
   Vec3 up = info.orient.Up();
   
   al_identity_transform(&cam);
   al_build_camera_transform(&cam , eye.x , eye.y , eye.z , look.x , look.y , look.z , up.x , up.y , up.z);
///   al_scale_transform_3d(&cam , 1.0 , 1.0 , 1.0);
   al_use_transform(&cam);
}



void Allegro5SpaceCamera::SetupSkyboxView() {
   /// Perspective matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60 , (float)4.0/3.0 , 1 , 2000.0);
   
   /// View matrix
   /// For our skybox, we always want to render as if we were looking in the same direction, but from the origin
   Vec3 look = info.pos + info.orient.Fw();
   Vec3 up = info.orient.Up();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0 , 0 , 0 , look.x , look.y , look.z , up.x , up.y , up.z);
}



void Allegro5SpaceCamera::DrawAxes(float axis_length) {
   float d = axis_length;
   glBegin(GL_LINES);
   glColor3f(1.0f , 0.0f , 0.0f);
   glVertex3f(0.0f , 0.0f , 0.0f);
   glVertex3f(   d , 0.0f , 0.0f);
   glColor3f(0.0f , 1.0f , 0.0f);
   glVertex3f(0.0f , 0.0f , 0.0f);
   glVertex3f(0.0f ,    d , 0.0f);
   glColor3f(0.0f , 0.0f , 1.0f);
   glVertex3f(0.0f , 0.0f , 0.0f);
   glVertex3f(0.0f , 0.0f ,    d);
   glEnd();
}


