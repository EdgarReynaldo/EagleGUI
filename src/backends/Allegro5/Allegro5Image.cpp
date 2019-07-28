


#include "Eagle.hpp"

#include "Eagle/backends/Allegro5/Allegro5Image.hpp"
#include "Eagle/backends/Allegro5/Allegro5Color.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5WindowManager.hpp"




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



/// -------------------------      Allegro5Image     ----------------------------------------



void Allegro5Image::SetClippingRectangle(Rectangle new_clip) {
	Allegro5TargetBitmap a5target_bitmap(bmp);
	al_set_clipping_rectangle(new_clip.X() , new_clip.Y() , new_clip.W() , new_clip.H());
}



void Allegro5Image::ResetClippingRectangle() {
	Allegro5TargetBitmap a5target_bitmap(bmp);
	al_reset_clipping_rectangle();
}



void Allegro5Image::SetParentContext(EagleGraphicsContext* owner_context) {
   pcontext = owner_context;
}



Allegro5Image::Allegro5Image(std::string objname) :
      EagleImage("Allegro5Image" , objname),
      bmp(0)
{
   
}


/**
Allegro5Image::Allegro5Image(ALLEGRO_BITMAP* bitmap , bool take_ownership) :
      EagleImage("Allegro5Image" , StringPrintF("%s ALLEGRO_BITMAP* %p" , take_ownership?"Owner of":"Reference to" , bitmap)),
      bmp(0)
{
   EAGLE_ASSERT(bitmap);
   if (take_ownership) {
      AdoptBitmap(bitmap);
   }
   else {
      ReferenceBitmap(bitmap);
   }
}



Allegro5Image::Allegro5Image(int width , int height , IMAGE_TYPE type) :
      EagleImage("Allegro5Image" , "Allocated image"),
      bmp(0)
{
   Allocate(width , height , type);
}



Allegro5Image::Allegro5Image(std::string file , IMAGE_TYPE type) :
      EagleImage("Allegro5Image" , file),
      bmp(0)
{
   Load(file , type);
}



Allegro5Image::Allegro5Image(EagleImage* parent_bitmap , int x , int y , int width , int height) :
      EagleImage("Allegro5Image" , "SubImage"),
      bmp(0)
{
   CreateSubBitmap(parent_bitmap , x , y , width , height);
}

*/

Allegro5Image::~Allegro5Image() {
   Free();
}



/// creation

EagleImage* Allegro5Image::Clone(EagleGraphicsContext* parent_window , std::string iname) {
   EagleImage* newimg = parent_window->EmptyImage(iname);
   parent_window->DrawToBackBuffer();/// This sets the parent_window as the owning context for the call to Allocate
   if (newimg->Allocate(W() , H() , ImageType())) {
      parent_window->PushDrawingTarget(newimg);
      parent_window->SetCopyBlender();
      parent_window->Draw(this , 0.0f , 0.0f , DRAW_NORMAL);
      parent_window->RestoreLastBlendingState();
      parent_window->PopDrawingTarget();
      return newimg;
   }
   /// else, fail
   parent_window->FreeImage(newimg);
   
   return (EagleImage*)0;
}



EagleImage* Allegro5Image::CreateSubBitmap(int x , int y , int width , int height , std::string iname) {
   EAGLE_ASSERT(Valid());
   if (!Valid()) {return (EagleImage*)0;}
   ALLEGRO_BITMAP* sub = al_create_sub_bitmap(bmp , x , y , width , height);
   if (!sub) {
      return (EagleImage*)0;
   }
   Allegro5Image* subimg = new Allegro5Image(iname);
   subimg->AdoptBitmap(sub);
   subimg->pcontext = pcontext;
   subimg->SetParent(this);
   return subimg;
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
      pcontext = GetAllegro5WindowManager()->GetAssociatedContext(al_get_current_display());
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
      pcontext = GetAllegro5WindowManager()->GetAssociatedContext(al_get_current_display());
///      SetName(file);
      w = al_get_bitmap_width(bmp);
      h = al_get_bitmap_height(bmp);
      EagleInfo() << StringPrintF("Loaded image %s from file at size %d x %d." , file.c_str() , w , h) << std::endl;
   }
   return bmp;
}



bool Allegro5Image::Save(std::string filename , std::string extension) {
   filename.append(".");
   filename.append(extension);
   return al_save_bitmap(filename.c_str() , bmp);
}



void Allegro5Image::AdoptBitmap(ALLEGRO_BITMAP* bitmap) {
   EAGLE_ASSERT(bitmap);
   Free();

   image_source = OWNIT;

///   parent_context = GetAllegro5WindowManager()->GetAssociatedContext(al_get_current_display());

   bmp = bitmap;
   w = al_get_bitmap_width(bmp);
   h = al_get_bitmap_height(bmp);
   if (al_get_bitmap_flags(bmp) & ALLEGRO_MEMORY_BITMAP) {
      image_type = MEMORY_IMAGE;
   }
   if (al_get_bitmap_flags(bmp) & ALLEGRO_VIDEO_BITMAP) {
      image_type = VIDEO_IMAGE;
   }
}



void Allegro5Image::ReferenceBitmap(ALLEGRO_BITMAP* bitmap) {
   Free();
   EAGLE_ASSERT(bitmap);

   image_source = REFERENCE_ONLY;

///   parent_context = GetAllegro5WindowManager()->GetAssociatedContext(al_get_current_display());

   bmp = bitmap;
   w = al_get_bitmap_width(bmp);
   h = al_get_bitmap_height(bmp);
   if (al_get_bitmap_flags(bmp) & ALLEGRO_MEMORY_BITMAP) {
      image_type = MEMORY_IMAGE;
   }
   if (al_get_bitmap_flags(bmp) & ALLEGRO_VIDEO_BITMAP) {
      image_type = VIDEO_IMAGE;
   }
}



void Allegro5Image::Free() {
   FreeChildren();
   if (bmp && (image_source == OWNIT)) {
      EagleInfo() << "Destroying allegro bitmap at " << bmp << std::endl;
      al_destroy_bitmap(bmp);
   }
   bmp = 0;
   w = 0;
   h = 0;
   image_type = MEMORY_IMAGE;
   parent = 0;
   pcontext = 0;
};



// reading operations
EagleColor Allegro5Image::GetPixel(int x , int y) {
   EAGLE_ASSERT(bmp);
   ALLEGRO_COLOR ac = al_get_pixel(bmp , x , y);
   return GetEagleColor(ac);
}



void ConvertMaskColorToAlphaZero(Allegro5Image* img , EagleColor mask) {
   EAGLE_ASSERT(img);
   ALLEGRO_BITMAP* bmp = img->AllegroBitmap();
   EAGLE_ASSERT(bmp);
   
   al_convert_mask_to_alpha(bmp , al_map_rgba(mask.R() , mask.G() , mask.B() , mask.A()));
}























