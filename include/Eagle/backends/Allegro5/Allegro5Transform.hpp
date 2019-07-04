
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5Transform.hpp
 * @brief The interface for working with Allegro 5 transforms
 */

#ifndef Allegro5Transform_HPP
#define Allegro5Transform_HPP



#include "Eagle/Transforms.hpp"

#include "allegro5/allegro.h"


/**! @fn Allegro5TransformBase
 *   @brief A concrete implementation of the @ref TransformBase class
 *   
 *   For public methods, see @ref Transform
 */

class Allegro5TransformBase : public TransformBase {
   
   friend class Allegro5Transformer;
   
protected :
   ALLEGRO_TRANSFORM t;
   
public :
   
   Allegro5TransformBase(const ALLEGRO_TRANSFORM& src);
   
   Allegro5TransformBase();
   
   Allegro5TransformBase(Allegro5TransformBase* a5trans);
   
   TransformBase* Copy(Allegro5TransformBase* transform_base);


   /// TransformBase functions

   virtual TransformBase* Rotate(double radians , double x , double y , double z);
   virtual TransformBase* Scale(double xscale , double yscale , double zscale = 1.0);
   virtual TransformBase* Translate(double dx , double dy , double dz = 0.0);
   virtual TransformBase* Invert();

   virtual TransformBase* Copy(TransformBase* transform_base);

   virtual TransformBase* ComposeWith(TransformBase* rhs);/// Composes this transform with the right hand side. 
                                                          /// The transform on the right will be applied second
   virtual TransformBase* GetIdentityMatrix();
   virtual TransformBase* GetViewMatrix();/// Gets view matrix for current bitmap target
   virtual TransformBase* GetProjectionMatrix();/// Gets projection matrix for current bitmap target

   virtual TransformBase* Clone();

   virtual void ApplyTransformation(double& destx , double& desty , double& destz);
};



/**! @class Allegro5Transformer
 *   @brief The Allegro 5 implementation of the @ref Transformer class
 *   
 *   See @ref Transformer for public methods
 */

class Allegro5Transformer : public Transformer {
   
protected :


   virtual TransformBase* CreateIdentityTransform();
   virtual TransformBase* CreateViewTransform();
   virtual TransformBase* CreateProjectionTransform();

   virtual void SetViewTransform(TransformBase* transform_base);
   virtual void SetProjectionTransform(TransformBase* transform_base);

public :
   virtual void ResetTransformStack(EagleGraphicsContext* win);
   virtual void RebaseTransformStack(EagleGraphicsContext* win);

   Allegro5Transformer();
};



#endif // Allegro5Transform_HPP



