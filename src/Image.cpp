
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Image.hpp"
#include "Eagle/Exception.hpp"

#include "Eagle/StringWork.hpp"


EagleImage::EagleImage(EagleGraphicsContext* owner_context , std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      parent_context(owner_context),
      w(0),
      h(0),
      image_type(MEMORY_IMAGE),
      image_source(ALLOCATED),
      source(),
      parent(0),
      children(false),
      clip_rects()
{
   
}



void EagleImage::AddChild(EagleImage* child) {
   EAGLE_ASSERT(child);
   child->SetParent(this);
   children.Add(child);
}



void EagleImage::RemoveChild(EagleImage* child) {
   EAGLE_ASSERT(child);
   children.Remove(child);
   child->SetParent(0);
}



void EagleImage::SetParent(EagleImage* parent_image) {
///   EAGLE_ASSERT(parent_image);
   parent = parent_image;
}



void EagleImage::PushClippingRectangle(Rectangle new_clip) {
	clip_rects.push_back(new_clip);
	SetClippingRectangle(new_clip);
}



void EagleImage::PopClippingRectangle() {
	if (clip_rects.size()) {
		clip_rects.pop_back();
		if (clip_rects.size() == 0) {
			ResetClippingRectangle();
		}
		else {
			SetClippingRectangle(clip_rects.back());
		}
	}
}



Clipper::Clipper(EagleImage* image , Rectangle clip)
{
	EAGLE_ASSERT(image);
	img = image;
	img->PushClippingRectangle(clip);
}



Clipper::~Clipper() {
	if (img) {
		img->PopClippingRectangle();
	}
}



/**
EagleImage::EagleImage(int width , int height , IMAGE_TYPE type) :
      w(0),
      h(0),
      image_type(MEMORY_IMAGE),
      image_source(ALLOCATED),
      source(),
      parent(0),
      children()
{
   Allocate(width , height , type);
}



EagleImage::EagleImage(std::string file , IMAGE_TYPE type) :
      w(0),
      h(0),
      image_type(MEMORY_IMAGE),
      image_source(ALLOCATED),
      source(),
      parent(0),
      children()
{
   Load(file , type);
}



EagleImage::EagleImage(EagleImage* parent , int x , int y , int width , int height) :
      w(0),
      h(0),
      image_type(MEMORY_IMAGE),
      image_source(ALLOCATED),
      source(),
      parent(0),
      children()
{
   CreateSubBitmap(parent , x , y , width , height);
}
*/

