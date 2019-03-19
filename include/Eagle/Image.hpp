
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

class EagleImage;

typedef SHAREDOBJECT<EagleImage> SHAREDIMAGE;



class EagleImage : public EagleObject {
protected :
   EagleGraphicsContext* pcontext;
   int w;
   int h;
   IMAGE_TYPE image_type;
   IMAGE_SOURCE image_source;
   std::string source;
   
   EagleImage* parent;
   
   typedef std::unordered_set<EagleImage*> IMGSET;
   typedef IMGSET::iterator IMGIT;
   
   IMGSET children;

   
	std::vector<Rectangle> clip_rects;

   virtual void SetClippingRectangle(Rectangle new_clip)=0;
   virtual void ResetClippingRectangle()=0;
   



   void SetParent(EagleImage* parent_image);
   void AddChild(EagleImage* child);
   void RemoveChild(EagleImage* child);

   
public :

   EagleImage(std::string objclass = "EagleImage" , std::string objname = "Nemo");
   virtual ~EagleImage();
   
   /// Deallocation
   void FreeChildren();/// Must be called in your derived class destructor
   void FreeChild(EagleImage* child);

   /// Image creation
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window , std::string iname = "Nemo")=0;
   virtual EagleImage* CreateSubBitmap(int x , int y , int width , int height , std::string iname = "Nemo")=0;

   virtual bool Allocate(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE)=0;
   virtual bool Load(std::string file , IMAGE_TYPE = VIDEO_IMAGE)=0;
   
   virtual bool Save(std::string filepath , std::string extension)=0;
   
   virtual bool Valid() const =0;
   virtual void Free()=0;

   /// reading operations
   virtual EagleColor GetPixel(int x , int y)=0;

   /// Getters
   int W() {return w;}
   int H() {return h;}
   int Area() {return w*h;}
   
   IMAGE_TYPE ImageType() {return image_type;}
   
   EagleGraphicsContext* ParentContext() {return pcontext;}
   
   EagleImage* Parent() {return parent;}
   
   /// Clipping API
   void PushClippingRectangle(Rectangle new_clip);
   void PopClippingRectangle();

};


EagleImage* Clone(EagleGraphicsContext* parent_window , EagleImage* img , std::string iname = "Nemo");
EagleImage* CreateSubImage(EagleImage* img , int sx , int sy , int sw , int sh , std::string iname = "Nemo");



class Clipper {
	EagleImage* img;

public :
	Clipper(EagleImage* image , Rectangle clip);
	~Clipper();
	
};






#endif // EagleImage_HPP




