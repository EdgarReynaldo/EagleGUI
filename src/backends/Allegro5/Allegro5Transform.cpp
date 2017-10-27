



#include "Eagle/backends/Allegro5/Allegro5Transform.hpp"

#include "Eagle/Exception.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/GraphicsContext.hpp"


Allegro5TransformBase::Allegro5TransformBase(const ALLEGRO_TRANSFORM& src) :
   TransformBase(),
   t()
{
   t = src;
}



Allegro5TransformBase::Allegro5TransformBase() :
      t()
{
   al_identity_transform(&t);
}



Allegro5TransformBase::Allegro5TransformBase(Allegro5TransformBase* a5trans) :
      t()
{
   Copy(a5trans);
}



TransformBase* Allegro5TransformBase::Copy(Allegro5TransformBase* transform_base) {
   if (!transform_base) {
      throw EagleException("Allegro5TransformBase::Copy(Allegro5TransformBase*) : transform_base is NULL!\n");
   }
   al_copy_transform(&t , &(transform_base->t));
   return this;
}



TransformBase* Allegro5TransformBase::Rotate(double radians , double x , double y , double z) {
   al_rotate_transform_3d(&t , x , y , z , radians);
   return this;
}



TransformBase* Allegro5TransformBase::Scale(double xscale , double yscale , double zscale) {
   al_scale_transform_3d(&t , xscale , yscale , zscale);
   return this;
}



TransformBase* Allegro5TransformBase::Translate(double dx , double dy , double dz) {
   al_translate_transform_3d(&t , dx , dy , dz);
   return this;
}



TransformBase* Allegro5TransformBase::Invert() {
   al_invert_transform(&t);
   return this;
}



TransformBase* Allegro5TransformBase::Copy(TransformBase* transform_base) {
   return Copy(dynamic_cast<Allegro5TransformBase*>(transform_base));
}



TransformBase* Allegro5TransformBase::ComposeWith(TransformBase* rhs) {
   Allegro5TransformBase* a5trans = dynamic_cast<Allegro5TransformBase*>(rhs);
   al_compose_transform(&t , &(a5trans->t));
   return this;
}



TransformBase* Allegro5TransformBase::GetIdentityMatrix() {
   al_identity_transform(&t);
   return this;
}



TransformBase* Allegro5TransformBase::GetViewMatrix() {
   al_copy_transform(&t , al_get_current_transform());
   return this;
}



TransformBase* Allegro5TransformBase::GetProjectionMatrix() {
   al_copy_transform(&t , al_get_current_projection_transform());
   return this;
}



TransformBase* Allegro5TransformBase::Clone() {
   return new Allegro5TransformBase(this);
}



/// ---------------------      Allegro5Transformer      -----------------------------------



Allegro5Transformer::Allegro5Transformer() :
      Transformer()
{
   
}



TransformBase* Allegro5Transformer::CreateIdentityTransform() {
   return new Allegro5TransformBase();
}



TransformBase* Allegro5Transformer::CreateViewTransform() {
   return (new Allegro5TransformBase())->GetViewMatrix();
}



TransformBase* Allegro5Transformer::CreateProjectionTransform() {
   return (new Allegro5TransformBase())->GetProjectionMatrix();
}



void Allegro5Transformer::SetViewTransform(TransformBase* transform_base) {
   Allegro5TransformBase* a5trans = dynamic_cast<Allegro5TransformBase*>(transform_base);
   EAGLE_ASSERT(a5trans);
   ALLEGRO_TRANSFORM* t = &(a5trans->t);
   al_use_transform(t);
}



void Allegro5Transformer::SetProjectionTransform(TransformBase* transform_base) {
   Allegro5TransformBase* a5trans = dynamic_cast<Allegro5TransformBase*>(transform_base);
   EAGLE_ASSERT(a5trans);
   ALLEGRO_TRANSFORM* t = &(a5trans->t);
   al_use_projection_transform(t);
}



void Allegro5Transformer::ResetTransformStack(EagleGraphicsContext* win) {
   EAGLE_ASSERT(win);
   EagleImage* tgt = win->GetDrawingTarget();
   EAGLE_ASSERT(tgt);

   while (!view_stack.empty()) {view_stack.pop();}
   while (!proj_stack.empty()) {proj_stack.pop();}

   Transform view_identity = GetIdentityTransform();
   ALLEGRO_TRANSFORM pt;
   al_identity_transform(&pt);
   al_orthographic_transform(&pt , 0 , 0 , -1 , tgt->W() , tgt->H() , 1);
   Transform proj_default = Transform(new Allegro5TransformBase(pt));
   
   PushViewTransform(view_identity);
   PushProjectionTransform(proj_default);
}



void Allegro5Transformer::RebaseTransformStack(EagleGraphicsContext* win) {
   (void)win;
/*
   EAGLE_ASSERT(win);
   EagleImage* tgt = win->GetDrawingTarget();
   EAGLE_ASSERT(tgt);
   Allegro5Image* img = dynamic_cast<Allegro5Image*>(tgt);
   EAGLE_ASSERT(img);
*/
   while (!view_stack.empty()) {view_stack.pop();}
   while (!proj_stack.empty()) {proj_stack.pop();}

   const ALLEGRO_TRANSFORM* vt = 0 , *pt = 0;
   vt = al_get_current_transform();
   pt = al_get_current_projection_transform();
   EAGLE_ASSERT(vt && pt);

   Transform view_identity = Transform(new Allegro5TransformBase(*vt));
   Transform proj_default = Transform(new Allegro5TransformBase(*pt));

   PushViewTransform(view_identity);
   PushProjectionTransform(proj_default);
}




