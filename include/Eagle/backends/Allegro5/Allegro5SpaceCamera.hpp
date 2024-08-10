
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
 * @file Allegro5SpaceCamera.hpp
 * @brief Allegro5SpaceCamera implementation
 */




#ifndef Allegro5SpaceCamera_HPP
#define Allegro5SpaceCamera_HPP

#include "Eagle/SpaceCamera.hpp"

class Allegro5SpaceCamera : public SpaceCamera {

   /// These affect the view and projection matrix
   virtual void Setup2D(int w , int h);
   virtual void Setup3D(bool orthographic);
   virtual void SetupSkyboxView();
   
   /// Draw the three axes at the origin
   virtual void DrawAxes(float axis_length);

};

#endif // Allegro5SpaceCamera_HPP
