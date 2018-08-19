


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

   friend class Allegro5GraphicsContext;
   
   void SetParentContext(EagleGraphicsContext* owner_context);
   
public :
   
   Allegro5Image(std::string objname = "Nemo");
   /**
   explicit Allegro5Image(ALLEGRO_BITMAP* bitmap , bool take_ownership);
   Allegro5Image(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE);
   Allegro5Image(std::string file , IMAGE_TYPE type = VIDEO_IMAGE);
   Allegro5Image(EagleImage* parent , int x , int y , int width , int height);
   */
   ~Allegro5Image();
   
   /// Image creation
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window , std::string iname = "Nemo");/// Clones us, may return 0 if not valid
   virtual EagleImage* CreateSubBitmap(int x , int y , int width , int height , std::string iname = "Nemo");/// Creates a sub bitmap of ours, may return 0 if not valid
   
   /// Image modification
   virtual bool Allocate(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE);
   virtual bool Load(std::string file , IMAGE_TYPE = VIDEO_IMAGE);

   /// Saving
   virtual bool Save(std::string filepath , std::string extension);
   
   void AdoptBitmap(ALLEGRO_BITMAP* bmp);
   void ReferenceBitmap(ALLEGRO_BITMAP* bmp);

   
   virtual bool Valid() const {return bmp;}
   
   virtual void Free();

   // reading operations
   virtual EagleColor GetPixel(int x , int y);

   
   ALLEGRO_BITMAP* AllegroBitmap() {return bmp;}
   
};


Allegro5Image* CreateClone(EagleGraphicsContext* win , Allegro5Image* a5img);


void ConvertMaskColorToAlphaZero(Allegro5Image* img , EagleColor mask);




#endif // Allegro5Image_HPP


