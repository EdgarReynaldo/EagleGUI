
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Transforms_HPP
#define Transforms_HPP


#include <memory>
#include <stack>


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




class Transform {

friend class Transformer;

protected :   
   std::shared_ptr<TransformBase> shared_transform;
   
public :
   Transform();/// Creates EMPTY transform!!!
   Transform(TransformBase* ptransform);
   Transform(const Transform& transform);

   Transform& operator=(const Transform& rhs);

   Transform& operator*=(const Transform& rhs);
   
   bool Empty() const;
   
   Transform& Rotate(double radians , double x , double y , double z);
   Transform& Scale(double xscale , double yscale , double zscale = 1.0);
   Transform& Translate(double dx , double dy , double dz = 0.0);
   Transform& Invert();
   Transform& Copy(TransformBase* transform_base);
   
   Transform& GetIdentityMatrix();
   Transform& GetViewMatrix();
   Transform& GetProjectionMatrix();
   
   void ApplyTransformation(double& destx , double& desty , double& destz);
};


class EagleGraphicsContext;



class Transformer {

protected :
   
   std::stack<Transform> view_stack;
   std::stack<Transform> proj_stack;
   
   TransformBase* GetTransformBase(const Transform& t) {return t.shared_transform.get();}
   
   virtual TransformBase* CreateIdentityTransform()=0;
      
   virtual TransformBase* CreateViewTransform()=0;
   virtual TransformBase* CreateProjectionTransform()=0;

   virtual void SetViewTransform(TransformBase* transform_base)=0;
   virtual void SetProjectionTransform(TransformBase* transform_base)=0;
   
public :
   virtual void RebaseTransformStack(EagleGraphicsContext* win)=0;
   virtual void ResetTransformStack(EagleGraphicsContext* win)=0;
   

   Transformer();
   
   virtual ~Transformer() {}
   
   Transform GetIdentityTransform();
   Transform GetViewTransform();
   Transform GetProjectionTransform();
   
//   Transform GetImageTransform

   Transform CreateTransform() {return GetIdentityTransform();}

   void SetViewTransform(const Transform& t);
   void SetProjectionTransform(const Transform& t);
   
   void PushViewTransform(const Transform& t);/// Pushes on stack and sets as current view transform
   void PushProjectionTransform(const Transform& t);/// Pushes on stack and sets as current projection transform
   void PopViewTransform();/// Pops view transform off top of stack and sets new top to current view transform
   void PopProjectionTransform();/// Pops projection transform off top of stack and sets new top to current projection transform

};



#endif // Transforms_HPP





