



#ifndef Allegro5Transform_HPP
#define Allegro5Transform_HPP


#include "Eagle/Transforms.hpp"


#include "allegro5/allegro.h"



class Allegro5TransformBase : public TransformBase {
   
   friend class Allegro5Transformer;
   
protected :
   ALLEGRO_TRANSFORM t;
   
public :
   
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

};





class Allegro5Transformer : public Transformer {
   
protected :
   
   
   virtual TransformBase* CreateIdentityTransform();
   virtual TransformBase* CreateViewTransform();
   virtual TransformBase* CreateProjectionTransform();

   virtual void SetViewTransform(TransformBase* transform_base);
   virtual void SetProjectionTransform(TransformBase* transform_base);

public :
   
   Allegro5Transformer();

   /// See Transformer base class for member functions
   
};






#endif // Allegro5Transform_HPP
