
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
 * @file Transforms.hpp
 * @brief The interface for working with 2D and 3D transformations in Eagle
 */

#ifndef Transforms_HPP
#define Transforms_HPP



#include <memory>
#include <stack>



/**! @class TransformBase
 *   @brief A purely virtual abstract base class for all transform objects
 */

class TransformBase {
   
public :
   TransformBase() {}
   virtual ~TransformBase() {}
   
   virtual TransformBase* Rotate(double radians , double x , double y , double z)=0;
   virtual TransformBase* Scale(double xscale , double yscale , double zscale = 1.0)=0;
   virtual TransformBase* Translate(double dx , double dy , double dz = 0.0)=0;
   virtual TransformBase* Invert()=0;

   virtual TransformBase* Copy(TransformBase* transform_base)=0;

   virtual TransformBase* ComposeWith(TransformBase* rhs)=0;
   
   virtual TransformBase* GetIdentityMatrix()=0;
   virtual TransformBase* GetViewMatrix()=0;
   virtual TransformBase* GetProjectionMatrix()=0;
   
   virtual TransformBase* Clone()=0;
   
   virtual void ApplyTransformation(double& destx , double& desty , double& destz)=0;
};



/**! @class Transform
 *   @brief The main class for working with transforms
 */

class Transform {

friend class Transformer;

protected :   
   std::shared_ptr<TransformBase> shared_transform;
   
public :
   Transform();/// Creates EMPTY transform!!!
   Transform(TransformBase* ptransform);///< Take ownership of ptransform
   Transform(const Transform& transform);///< Copy constructor

   Transform& operator=(const Transform& rhs);///< Assignment operator

   Transform& operator*=(const Transform& rhs);///< Compose transforms
   
   bool Empty() const;///< True if empty
   
   Transform& Rotate(double radians , double x , double y , double z);///< Rotate around (x,y,z) by the specified angle in radians
   Transform& Scale(double xscale , double yscale , double zscale = 1.0);///< Scale by xscale,yscale,zscale
   Transform& Translate(double dx , double dy , double dz = 0.0);///< Translate by dx,dy,dz
   Transform& Invert();///< Get the inverted form of this matrix
   Transform& Copy(TransformBase* transform_base);///< Copy another transformbase
   
   Transform& GetIdentityMatrix();///< Take on the value of the identity matrix
   Transform& GetViewMatrix();///< Take on the value of the view matrix
   Transform& GetProjectionMatrix();///< Take on the value of the projection matrix
   
   void ApplyTransformation(double& destx , double& desty , double& destz);///< Apply the transformation to destx,desty,destz
};



class EagleGraphicsContext;


/**! @class Transformer
 *   @brief The base class for all Transformers (Transform creators)
 */

class Transformer {

protected :
   
   std::stack<Transform> view_stack;///< The stack of view transforms
   std::stack<Transform> proj_stack;///< The stack of projection transforms
   
   TransformBase* GetTransformBase(const Transform& t) {return t.shared_transform.get();}///< Get the underlying @ref TransformBase from t
   
   virtual TransformBase* CreateIdentityTransform()=0;///< Pure virtual function to create an identity transform
      
   virtual TransformBase* CreateViewTransform()=0;///< Pure virtual function to create a view transform
   virtual TransformBase* CreateProjectionTransform()=0;///< Pure virtual function to create a projection transform

   virtual void SetViewTransform(TransformBase* transform_base)=0;///< Pure virtual function to set the view transform
   virtual void SetProjectionTransform(TransformBase* transform_base)=0;///< Pure virtual function to set the projection transform
   
public :
   virtual void RebaseTransformStack(EagleGraphicsContext* win)=0;///< ???
   virtual void ResetTransformStack(EagleGraphicsContext* win)=0;///< ???
   

   Transformer();///< Default constructor
   
   virtual ~Transformer() {}///< Virtual destructor
   
   Transform GetIdentityTransform();///< Get the identity transform
   Transform GetViewTransform();///< Get the view transform
   Transform GetProjectionTransform();///< Get the projection transform
   
   Transform CreateTransform() {return GetIdentityTransform();}///< Create an identity transform

   void SetViewTransform(const Transform& t);///< Set the view transform
   void SetProjectionTransform(const Transform& t);///< Set the projection transform
   
   void PushViewTransform(const Transform& t);///< Pushes on stack and sets as current view transform
   void PushProjectionTransform(const Transform& t);///< Pushes on stack and sets as current projection transform

   void PopViewTransform();///< Pops view transform off top of stack and sets new top to current view transform
   void PopProjectionTransform();///< Pops projection transform off top of stack and sets new top to current projection transform
};




#endif // Transforms_HPP



