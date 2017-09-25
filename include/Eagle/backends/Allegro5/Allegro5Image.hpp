


#ifndef Allegro5Image_HPP
#define Allegro5Image_HPP



#include "Eagle/Image.hpp"

#include "allegro5/allegro.h"



ALLEGRO_BITMAP* GetAllegroBitmap(EagleImage* img);


class Allegro5TargetBitmap {
	ALLEGRO_BITMAP* oldtarget;
	ALLEGRO_BITMAP* newtarget;
	
public :
	Allegro5TargetBitmap(ALLEGRO_BITMAP* target);
	~Allegro5TargetBitmap();
};


class Allegro5Image : public EagleImage {
private :
   ALLEGRO_BITMAP* bmp;

protected :
   void SetClippingRectangle(Rectangle new_clip);
   void ResetClippingRectangle();

public :
   
   Allegro5Image(EagleGraphicsContext* owner , std::string objname);

   /// TODO : ADD PARENT GC
   explicit Allegro5Image(ALLEGRO_BITMAP* bitmap , bool take_ownership);
   Allegro5Image(int width , int height , IMAGE_TYPE type);
   Allegro5Image(std::string file , IMAGE_TYPE type);
   Allegro5Image(EagleImage* parent , int x , int y , int width , int height);

   ~Allegro5Image();
   
   // creation
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window);
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window , EagleImage* a5img);
   virtual bool Allocate(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE);
   virtual bool Load(std::string file , IMAGE_TYPE = VIDEO_IMAGE);
   virtual bool CreateSubBitmap(EagleImage* parent_bitmap , int x , int y , int width , int height);

   virtual bool Save(std::string filepath , std::string extension);
   
   
   void AdoptBitmap(ALLEGRO_BITMAP* bmp);
   void ReferenceBitmap(ALLEGRO_BITMAP* bmp);

   
   virtual bool Valid() {return bmp;}
   
   virtual void Free();

   // reading operations
   virtual EagleColor GetPixel(int x , int y);

   
   ALLEGRO_BITMAP* AllegroBitmap() {return bmp;}
   
};


Allegro5Image* CreateClone(EagleGraphicsContext* win , Allegro5Image* a5img);



#endif // Allegro5Image_HPP


