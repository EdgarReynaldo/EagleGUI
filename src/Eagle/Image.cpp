
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



#include "Eagle/Image.hpp"
#include "Eagle/Exception.hpp"

#include "Eagle/StringWork.hpp"



EagleImage* Clone(EagleGraphicsContext* parent_window , EagleImage* img , std::string iname) {
   return img->Clone(parent_window , iname);
}



EagleImage* CreateSubImage(EagleImage* img , int sx , int sy , int sw , int sh , std::string iname) {
   return img->CreateSubBitmap(sx,sy,sw,sh , iname);
}



/// -----------------------      EagleImage      -------------------------------



void EagleImage::SetParent(EagleImage* parent_image) {
   parent = parent_image;
}



void EagleImage::AddChild(EagleImage* child) {
   EAGLE_ASSERT(child);
   child->SetParent(this);
   children.insert(child);
}



void EagleImage::RemoveChild(EagleImage* child) {
   EAGLE_ASSERT(child);
   IMGIT it = children.find(child);
   EAGLE_ASSERT(it != children.end());
   children.erase(it);
   /// child removes itself upon destruction - don't call child methods
   ///child->SetParent(0);/// BAD, UNNECESSARY
}



EagleImage::EagleImage(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      pcontext(0),
      w(0),
      h(0),
      image_type(MEMORY_IMAGE),
      image_source(ALLOCATED),
      source(),
      parent(0),
      children(),
      clip_rects()
{
   
}



EagleImage::~EagleImage() {
   /// Can't free children here, as our parents derived class destructor has already run
   /// Must do it in derived class destructors....
   (void)0;
   if (parent) {
      parent->RemoveChild(this);
   }
}



void EagleImage::FreeChildren() {
   IMGSET imgs = children;/// Must make a copy as children is altered by destruction of each child which calls RemoveChild
   for (IMGIT it = imgs.begin() ; it != imgs.end() ; ++it) {
      FreeChild(*it);
   }
   EAGLE_ASSERT(children.size() == 0);
}



void EagleImage::FreeChild(EagleImage* child) {
   IMGIT it = children.find(child);
   if (it != children.end()) {
      delete child;/// ~EagleImage calls RemoveChild
   }
   EAGLE_ASSERT(children.find(child) == children.end());
}



/**! TODO : FIXME Pushing an invalid rectangle onto the clipping stack and then popping results in UB */
bool EagleImage::PushClippingRectangle(Rectangle new_clip) {
   if (new_clip == BADRECTANGLE) {return false;}
   if (clip_rects.size()) {
      Rectangle outer = clip_rects.back();
      if (!(outer.Contains(new_clip))) {
         Rectangle overlap = Overlap(outer , new_clip);
         if (overlap == BADRECTANGLE) {
            return false;
         }
         new_clip = overlap;
      }
   }
	clip_rects.push_back(new_clip);
	SetClippingRectangle(new_clip);
	return true;
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




/// ----------------     Clipper      ------------------------



Clipper::Clipper(EagleImage* image , Rectangle clip)
{
	EAGLE_ASSERT(image);
	img = image;
	clip_worked = img->PushClippingRectangle(clip);
}



Clipper::~Clipper() {
	if (img && clip_worked) {
		img->PopClippingRectangle();
	}
}




