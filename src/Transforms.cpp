
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#include "Eagle/Transforms.hpp"

#include "Eagle/Exception.hpp"




Transform::Transform() :
   shared_transform((TransformBase*)0)
{}



Transform::Transform(TransformBase* ptransform) : 
      shared_transform(ptransform) 
{}



Transform::Transform(const Transform& transform) :
   shared_transform()
{
   EAGLE_ASSERT(!transform.Empty());
   shared_transform.reset(transform.shared_transform.get()->Clone());
}



Transform& Transform::operator=(const Transform& rhs) {
   if (rhs.Empty()) {
      shared_transform.reset((TransformBase*)0);
      return *this;
   }
   if (Empty()) {
      shared_transform.reset(rhs.shared_transform.get()->Clone());
   }
   else {
      shared_transform.get()->Copy(rhs.shared_transform.get());
   }
   return *this;
}



Transform& Transform::operator*=(const Transform& rhs) {
   EAGLE_ASSERT(!rhs.Empty());
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->ComposeWith(rhs.shared_transform.get());
   return *this;
}



bool Transform::Empty() const {
   return shared_transform.get() == 0;
}



Transform& Transform::Rotate(double radians , double x , double y , double z) {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->Rotate(radians , x , y , z);
   return *this;
}



Transform& Transform::Scale(double xscale , double yscale , double zscale) {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->Scale(xscale , yscale , zscale);
   return *this;
}



Transform& Transform::Translate(double dx , double dy , double dz) {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->Translate(dx , dy , dz);
   return *this;
}



Transform& Transform::Invert() {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->Invert();
   return *this;
}



Transform& Transform::Copy(TransformBase* transform_base) {
   if (Empty()) {
      shared_transform.reset(transform_base->Clone());
   }
   else {
      shared_transform.get()->Copy(transform_base);
   }
   return *this;
}



Transform& Transform::GetIdentityMatrix() {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->GetIdentityMatrix();
   return *this;
}



Transform& Transform::GetViewMatrix() {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->GetViewMatrix();
   return *this;
}



Transform& Transform::GetProjectionMatrix() {
   EAGLE_ASSERT(!Empty());
   shared_transform.get()->GetProjectionMatrix();
   return *this;
}



/// ------------------------      Transformer      --------------------------------



Transformer::Transformer() :
      view_stack(),
      proj_stack()
{}


Transform Transformer::GetIdentityTransform() {
   return Transform(CreateIdentityTransform());
}



Transform Transformer::GetViewTransform() {
   return Transform(CreateViewTransform());
}



Transform Transformer::GetProjectionTransform() {
   return Transform(CreateProjectionTransform());
}





void Transformer::SetViewTransform(const Transform& t) {
   SetViewTransform(GetTransformBase(t));
   view_stack.top() = t;
}



void Transformer::SetProjectionTransform(const Transform& t) {
   SetProjectionTransform(GetTransformBase(t));
   proj_stack.top() = t;
}



void Transformer::PushViewTransform(const Transform& t) {
   SetViewTransform(GetTransformBase(t));
   view_stack.push(t);
}



void Transformer::PushProjectionTransform(const Transform& t) {
   SetProjectionTransform(GetTransformBase(t));
   proj_stack.push(t);
}



void Transformer::PopViewTransform() {
   if (!view_stack.empty()) {
      view_stack.pop();
      SetViewTransform(GetTransformBase(view_stack.top()));
   }
}



void Transformer::PopProjectionTransform() {
   if (!proj_stack.empty()) {
      proj_stack.pop();
      SetProjectionTransform(GetTransformBase(proj_stack.top()));
   }
}




