


#include "Eagle.hpp"

#include "Eagle/backends/Allegro5/Allegro5Image.hpp"
#include "Eagle/backends/Allegro5/Allegro5Color.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"




ALLEGRO_BITMAP* GetAllegroBitmap(EagleImage* img) {
   Allegro5Image* a5img = dynamic_cast<Allegro5Image*>(img);
   EAGLE_ASSERT(a5img);
   if (!a5img) {return 0;}
   return a5img->AllegroBitmap();
}



Allegro5TargetBitmap::Allegro5TargetBitmap(ALLEGRO_BITMAP* target) :
		oldtarget(0),
		newtarget(0)
{
	oldtarget = al_get_target_bitmap();
	newtarget = target;
	if (oldtarget != newtarget) {
		al_set_target_bitmap(newtarget);
	}
}



Allegro5TargetBitmap::~Allegro5TargetBitmap() {
	if (newtarget != oldtarget) {
		al_set_target_bitmap(oldtarget);
	}
}



void Allegro5Image::SetClippingRectangle(Rectangle new_clip) {
	Allegro5TargetBitmap a5target_bitmap(bmp);
	al_set_clipping_rectangle(new_clip.X() , new_clip.Y() , new_clip.W() , new_clip.H());
}



void Allegro5Image::ResetClippingRectangle() {
	Allegro5TargetBitmap a5target_bitmap(bmp);
	al_reset_clipping_rectangle();
}



Allegro5Image::Allegro5Image() :
      EagleImage((EagleGraphicsContext*)0 , StringPrintF("Allegro5Image at %p" , this)),
      bmp(0)
{
   
}
Allegro5Image::Allegro5Image(std::string name) :
      EagleImage((EagleGraphicsContext*)0 , name),
      bmp(0)
{
   
}



Allegro5Image::Allegro5Image(ALLEGRO_BITMAP* bitmap , bool take_ownership) :
      EagleImage((EagleGraphicsContext*)0 , StringPrintF("Allegro5Image at %p" , this)),
      bmp(0)
{
   ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
   if (take_ownership) {
      image_source = OWNIT;
   }
   else {
      image_source = REFERENCE_ONLY;
   }
   if (bitmap) {
      al_set_target_bitmap(bitmap);/// So we can call al_get_current_display
      ALLEGRO_DISPLAY* display = al_get_current_display();
      parent_context = GetAssociatedContext(display);\
      al_set_target_bitmap(old_target);
   }
   bmp = bitmap;
}



Allegro5Image::Allegro5Image(int width , int height , IMAGE_TYPE type) :
      EagleImage((EagleGraphicsContext*)0 , StringPrintF("Allegro5Image at %p" , this)),
      bmp(0)
{
   Allocate(width , height , type);
}



Allegro5Image::Allegro5Image(std::string file , IMAGE_TYPE type) :
      EagleImage((EagleGraphicsContext*)0 , StringPrintF("Allegro5Image at %p" , this)),
      bmp(0)
{
   Load(file , type);
}



Allegro5Image::Allegro5Image(EagleImage* parent_bitmap , int x , int y , int width , int height) :
      EagleImage((EagleGraphicsContext*)0 , StringPrintF("Allegro5Image at %p" , this)),
      bmp(0)
{
   CreateSubBitmap(parent_bitmap , x , y , width , height);
}



// creation

EagleImage* Allegro5Image::Clone(EagleGraphicsContext* parent_window) {
   return Allegro5Image::Clone(parent_window , this);
}



EagleImage* Allegro5Image::Clone(EagleGraphicsContext* parent_window , EagleImage* a5img) {
   /// TODO : Deep copy of base classes???
   bool ret = false;
   EagleImage* newimg = parent_window->EmptyImage();
   parent_window->DrawToBackBuffer();/// This sets the parent_window as the owning context for the call to Allocate
   if ((ret = newimg->Allocate(a5img->W() , a5img->H() , a5img->ImageType()))) {
      parent_window->PushDrawingTarget(newimg);
      /// TODO : Set overwrite blender
      parent_window->Draw(a5img , 0.0f , 0.0f , DRAW_NORMAL);
      /// TODO : Unset overwrite blender
      parent_window->PopDrawingTarget();
      return newimg;
   }
   // else, fail
   parent_window->FreeImage(newimg);
   return 0;
}



bool Allegro5Image::Allocate(int width , int height , IMAGE_TYPE type) {
	Free();
	int area = width*height;
	if (!area) {return true;}
	
   if (type == SYSTEM_IMAGE) {type = VIDEO_IMAGE;}
   
   if (type == VIDEO_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   }
   else if (type == MEMORY_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   }
   bmp = al_create_bitmap(width , height);
   if (!bmp) {
      EagleError() << "Failed to create " << width << " x " << height << " bitmap." << std::endl;
   }
   else {
      parent_context = GetAssociatedContext(al_get_current_display());
      w = al_get_bitmap_width(bmp);
      h = al_get_bitmap_height(bmp);
   }
   return bmp;
}



bool Allegro5Image::Load(std::string file , IMAGE_TYPE type) {
   Free();
   if (type == SYSTEM_IMAGE) {type = VIDEO_IMAGE;}
   
   if (type == VIDEO_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
   }
   else if (type == MEMORY_IMAGE) {
      al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
   }
   bmp = al_load_bitmap(file.c_str());
   if (!bmp) {
      EagleError() << "Failed to load " << file << " from disk." << std::endl;
      EAGLE_ASSERT(0);
   }
   else {
      source = file;
      parent_context = GetAssociatedContext(al_get_current_display());
///      SetName(file);
      w = al_get_bitmap_width(bmp);
      h = al_get_bitmap_height(bmp);
   }
   return bmp;
}



bool Allegro5Image::CreateSubBitmap(EagleImage* parent_bitmap , int x , int y , int width , int height) {
   EAGLE_ASSERT(parent_bitmap);
   Free();
   Allegro5Image* img = dynamic_cast<Allegro5Image*>(parent_bitmap);
   EAGLE_ASSERT(img);
   ALLEGRO_BITMAP* allegro_bitmap = img->AllegroBitmap();
   EAGLE_ASSERT(allegro_bitmap);
   
   bmp = al_create_sub_bitmap(allegro_bitmap , x , y , width , height);
   if (bmp) {
      parent_context = parent_bitmap->ParentContext();
      parent_bitmap->AddChild(this);
      SetParent(parent_bitmap);
      w = al_get_bitmap_width(bmp);
      h = al_get_bitmap_height(bmp);
   }
   else {
      EagleError() << "Failed to create sub bitmap from " << parent_bitmap << std::endl;
   }
   return bmp;
}



void Allegro5Image::AdoptBitmap(ALLEGRO_BITMAP* bitmap) {
   Free();
   EAGLE_ASSERT(bitmap);
   ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
   al_set_target_bitmap(bitmap);
   parent_context = GetAssociatedContext(al_get_current_display());
   al_set_target_bitmap(old_target);
   bmp = bitmap;
   w = al_get_bitmap_width(bmp);
   h = al_get_bitmap_height(bmp);
   if (al_get_bitmap_flags(bmp) & ALLEGRO_MEMORY_BITMAP) {
      image_type = MEMORY_IMAGE;
   }
   if (al_get_bitmap_flags(bmp) & ALLEGRO_VIDEO_BITMAP) {
      image_type = VIDEO_IMAGE;
   }
   image_source = OWNIT;
}



void Allegro5Image::ReferenceBitmap(ALLEGRO_BITMAP* bitmap) {
   Free();
   EAGLE_ASSERT(bitmap);
   ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
   al_set_target_bitmap(bitmap);
   parent_context = GetAssociatedContext(al_get_current_display());
   al_set_target_bitmap(old_target);
   bmp = bitmap;
   w = al_get_bitmap_width(bmp);
   h = al_get_bitmap_height(bmp);
   if (al_get_bitmap_flags(bmp) & ALLEGRO_MEMORY_BITMAP) {
      image_type = MEMORY_IMAGE;
   }
   if (al_get_bitmap_flags(bmp) & ALLEGRO_VIDEO_BITMAP) {
      image_type = VIDEO_IMAGE;
   }
   image_source = REFERENCE_ONLY;
}



void Allegro5Image::Free() {
   if (bmp && (image_source != REFERENCE_ONLY)) {
      for (unsigned int i = 0 ; i < children.size() ; ++i) {
         EagleImage* img = children[i];
         img->Free();
      }
      children.RemoveAll();
      EagleInfo() << "Destroying allegro bitmap at " << bmp << std::endl;
      al_destroy_bitmap(bmp);
      bmp = 0;
      w = 0;
      h = 0;
      image_type = MEMORY_IMAGE;
      parent = 0;
      parent_context = 0;
   }
};



// reading operations
EagleColor Allegro5Image::GetPixel(int x , int y) {
   EAGLE_ASSERT(bmp);
   ALLEGRO_COLOR ac = al_get_pixel(bmp , x , y);
   return GetEagleColor(ac);
}

