
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
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleImage_HPP
#define EagleImage_HPP

#include "Eagle/Area.hpp"
#include "Eagle/Color.hpp"
#include "Eagle/Container.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/SharedMemory.hpp"

#include <string>
#include <vector>
#include <memory>

enum IMAGE_TYPE {
   MEMORY_IMAGE = 0,
   SYSTEM_IMAGE = 1,
   VIDEO_IMAGE = 2
};

enum IMAGE_SOURCE {
   ALLOCATED = 0,
   LOADED = 1,
   SUBBITMAP = 2,
   REFERENCE_ONLY = 3,
   OWNIT = 4
};



/**
TODO : Add in member to track window ownership, and ability to traverse windows
EagleGraphicsContext* owner_window
*/



class EagleImage : public EagleObject {
protected :
   EagleGraphicsContext* parent_context;
   int w;
   int h;
   IMAGE_TYPE image_type;
   IMAGE_SOURCE image_source;
   std::string source;
   
   EagleImage* parent;
   PointerManager<EagleImage> children;

	std::vector<Rectangle> clip_rects;



   virtual void SetClippingRectangle(Rectangle new_clip)=0;
   virtual void ResetClippingRectangle()=0;
   

public :

   EagleImage(EagleGraphicsContext* owner_context , std::string objclass = "EagleImage" , std::string objname = "Nemo");
   virtual ~EagleImage();

   /** SUGGESTED IMPLEMENTATIONS IN YOUR DERIVED CLASS CONSTRUCTORS
   EagleImage(int width , int height , IMAGE_TYPE type);
   EagleImage(std::string file , IMAGE_TYPE type);
   EagleImage(EagleImage* parent , int x , int y , int width , int height);
   */

/** TODO : Track child/parent relationship? What for? Auto destruct? */
   void AddChild(EagleImage* child);
   void RemoveChild(EagleImage* child);
   void SetParent(EagleImage* parent_image);
   
   
   
   // creation
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window)=0;
   virtual bool Allocate(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE)=0;
   virtual bool Load(std::string file , IMAGE_TYPE = VIDEO_IMAGE)=0;
   virtual bool CreateSubBitmap(EagleImage* parent_bitmap , int x , int y , int width , int height)=0;
   
   virtual bool Save(std::string filepath , std::string extension)=0;
   
   virtual bool Valid()=0;
   virtual void Free()=0;

   // reading operations
   virtual EagleColor GetPixel(int x , int y)=0;

   // Getters
   int W() {return w;}
   int H() {return h;}
   int Area() {return w*h;}
   
   IMAGE_TYPE ImageType() {return image_type;}
   
   EagleGraphicsContext* ParentContext() {return parent_context;}
   
   // 
   void PushClippingRectangle(Rectangle new_clip);
   void PopClippingRectangle();

};


class Clipper {
	EagleImage* img;

public :
	Clipper(EagleImage* image , Rectangle clip);
	~Clipper();
	
};


typedef SHAREDOBJECT<EagleImage> SHAREDIMAGE;




#endif // EagleImage_HPP




