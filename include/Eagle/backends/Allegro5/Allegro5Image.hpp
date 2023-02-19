
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5Image.hpp
 * @brief The interface for Allegro 5 images
 */

#ifndef Allegro5Image_HPP
#define Allegro5Image_HPP



#include "Eagle/Image.hpp"

#include "allegro5/allegro.h"



ALLEGRO_BITMAP* GetAllegroBitmap(EagleImage* img);///< Get the ALLEGRO_BITMAP from an EagleImage


/**! @class Allegro5TargetBitmap
 *   @brief Simple helper class to change the bitmap drawing target
 *   
 *   Upon creation, the specified target will be set as the drawing target
 *   Upon destruction, the previous target will be restored
 */

class Allegro5TargetBitmap {
	ALLEGRO_BITMAP* oldtarget;
	ALLEGRO_BITMAP* newtarget;
	
public :
	Allegro5TargetBitmap(ALLEGRO_BITMAP* target);
	~Allegro5TargetBitmap();
};



/**! @class Allegro5Image
 *   @brief The Allegro 5 implementation of an @ref EagleImage
 */

class Allegro5Image : public EagleImage {
private :
   ALLEGRO_BITMAP* bmp;

protected :
   void SetClippingRectangle(Rectangle new_clip);
   void ResetClippingRectangle();

   friend class Allegro5GraphicsContext;
   
   void SetParentContext(EagleGraphicsContext* owner_context);
   
public :
   
   Allegro5Image(std::string objname = "Nemo");///< Public constructor
   ~Allegro5Image();///< Destructor
   
   /// Image creation

   /// Clones us, may return 0 if not valid
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window , std::string iname = "Nemo");

   /// Creates a sub bitmap of ours, may return 0 if not valid
   virtual EagleImage* CreateSubBitmap(int x , int y , int width , int height , std::string iname = "Nemo");
   
   /// Image modification

   ///< Allocate a new image of the specified width, height, and image type
   virtual bool Allocate(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE);
   
   ///< Load an image from disk using the specified image type
   virtual bool Load(std::string file , IMAGE_TYPE = VIDEO_IMAGE);

   ///< Save this image using the specified filename and extension
   ///< @retval True on success, false on failure
   virtual bool Save(std::string filename , std::string extension);
   
   ///< Adopt an ALLEGRO_BITMAP* and take ownership
   void AdoptBitmap(ALLEGRO_BITMAP* bmp);
   
   ///< Adopt this bitmap, but as reference only, we do not own it
   void ReferenceBitmap(ALLEGRO_BITMAP* bmp);

   ///< True if valid and ready
   virtual bool Valid() const {return bmp;}
   
   ///< Free the underlying image
   virtual void Free();

   /// Get the color of a single pixel
   virtual EagleColor GetPixel(int x , int y);

   /// Write a single pixel
   virtual void PutPixel(int x , int y , EagleColor c);

   ///< Get a reference to the ALLEGRO_BITMAP* used by this image
   ALLEGRO_BITMAP* AllegroBitmap() {return bmp;}
   
};



Allegro5Image* CreateClone(EagleGraphicsContext* win , Allegro5Image* a5img);///< Clone an image into the specified window



void ConvertMaskColorToAlphaZero(Allegro5Image* img , EagleColor mask);///< Change all pixels from mask color to zero alpha



#endif // Allegro5Image_HPP



