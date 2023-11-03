
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
 * @file Image.hpp
 * @brief The main interface for the @ref EagleImage class
 */

#ifndef EagleImage_HPP
#define EagleImage_HPP



#include "Eagle/Area.hpp"
#include "Eagle/Color.hpp"
#include "Eagle/Container.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/Resources.hpp"
#include "Eagle/SharedMemory.hpp"

#include <string>
#include <vector>
#include <memory>



/**! @enum IMAGE_TYPE
 *   @brief Determines whether this image is a memory image, system image, or video image
 */

enum IMAGE_TYPE {
   MEMORY_IMAGE = 0,///< Memory image
   SYSTEM_IMAGE = 1,///< System image
   VIDEO_IMAGE = 2  ///< Video image
};


/**! @enum IMAGE_SOURCE
 *   @brief Stores data about where an image came from
 *   
 *   Lets you know whether this image was allocated from memory (any kind), loaded from disk, created from a parent image,
 *   whether this is a reference to an image we should not free, or whether we own it
 */

enum IMAGE_SOURCE {
   ALLOCATED      = 0,///< Allocated from main, system, or video memory
   LOADED         = 1,///< This image was loaded from disk
   SUBBITMAP      = 2,///< This image is a sub image
   REFERENCE_ONLY = 3,///< This image is a reference and should not be destroyed
   OWNIT          = 4 ///< We own this image, no one else should touch it
};




class EagleImage;

/**! @typedef SHAREDIMAGE
 *   @brief Typedef to make a shared pointer to an EagleImage
 */

typedef SHAREDOBJECT<EagleImage> SHAREDIMAGE;


/**! @class EagleImage
 *   @brief Base class for all eagle image objects
 *    
 *   TODO : Add in member to track window ownership (done), and ability to traverse windows (not done)
 */

class EagleImage : public EagleObject , public ResourceBase {
protected :
   EagleGraphicsContext* pcontext;///< Parent graphics context - this is the window that created us, or owns us currently
   int w;///< Image width
   int h;///< Image height
   IMAGE_TYPE image_type;///< Image type
   IMAGE_SOURCE image_source;///< Image source
   std::string source;///< Source string
   
   EagleImage* parent;///< Parent image
   
   /**! @typedef IMGSET
    *   @brief Typedef to make working with a set of EagleImage pointers easier
    */
   typedef std::unordered_set<EagleImage*> IMGSET;
   /**! @typedef IMGIT
    *   @brief Typedef for working with @ref IMGSET iterators
    */
   typedef IMGSET::iterator IMGIT;
   
   IMGSET children;///< Our child image set

	std::vector<Rectangle> clip_rects;///< Clipping rectangle stack

   virtual void SetClippingRectangle(Rectangle new_clip)=0;///< Pure virtual function to set the clipping rectangle for this image
   virtual void ResetClippingRectangle()=0;///< Pure virtual function to reset the clipping rectangle for this image
   
   void SetParent(EagleImage* parent_image);///< Set the parent image
   void AddChild(EagleImage* child);///< Add a child to the @ref EagleImage::children image set
   void RemoveChild(EagleImage* child);///< Remove a child from the @ref EagleImage::children image set

   bool LoadFromArgs(std::vector<std::string> args) override;

public :

   EagleImage(std::string objclass = "EagleImage" , std::string objname = "Nemo");///< Default empty constructor
   virtual ~EagleImage();
   
   /// Deallocation

   void FreeChildren();///< Frees all children. Must be called in your derived class destructor
   void FreeChild(EagleImage* child);///< Frees a child image

   /// Image creation

   ///< Pure virtual function to clone this EagleImage object into the new parent window with the given name
   virtual EagleImage* Clone(EagleGraphicsContext* parent_window , std::string iname = "Nemo")=0;

   ///< Pure virtual function to create a sub bitmap from this EagleImage using the specified area and new name
   virtual EagleImage* CreateSubBitmap(int x , int y , int width , int height , std::string iname = "Nemo")=0;

   ///< Pure virtual function to allocate a new image
   virtual bool Allocate(int width , int height , IMAGE_TYPE type = VIDEO_IMAGE)=0;

   ///< Pure virtual function to load a new image
   virtual bool Load(std::string file , IMAGE_TYPE = VIDEO_IMAGE)=0;
   
   ///< Pure virtual function to save an image using the specified file name and extension (determines the file type)
   virtual bool Save(std::string filepath , std::string extension)=0;
   
   virtual bool Valid() const =0;///< Pure virtual function to determine if this EagleImage object is valid and ready to use

   virtual void Free()=0;///< Pure virtual function to free an image

   /// reading operations

   ///< Reads the specified pixel from this image and returns its color
   virtual EagleColor GetPixel(int x , int y)=0;
   
   /// writing operations
   
   ///< Writes the specified pixel into the image
   virtual void PutPixel(int x , int y , EagleColor c)=0;

   /// Getters
   
   int W()    {return w;  }///< Get this image's width
   int H()    {return h;  }///< Get this image's height
   int Area() {return w*h;}///< Get this image's area
   
   IMAGE_TYPE ImageType() {return image_type;}///< Get this image's type
   
   EagleGraphicsContext* ParentContext() {return pcontext;}///< Get a pointer to the parent window of this image
   
   EagleImage* Parent() {return parent;}///< Get a pointer to the parent image of this one
   
   /// Clipping API
   
   bool PushClippingRectangle(Rectangle new_clip);///< Push a new clipping rectangle onto the stack for this image
   void PopClippingRectangle();///< Pop the top clipping rectangle from the stack for this image, will reset to none if empty
};


/**! @fn Clone <EagleGraphicsContext* , EagleImage* , std::string>
 *   @brief Clones the specified image into a new image owned by the specified window using the new name
 */
EagleImage* Clone(EagleGraphicsContext* parent_window , EagleImage* img , std::string iname = "Nemo");

/**! @fn CreateSubImage <EagleImage* , int , int , int , int , std::string>
 *   @brief Creates a new sub image of the specified image on its parent window using the specified area and name
 */
EagleImage* CreateSubImage(EagleImage* img , int sx , int sy , int sw , int sh , std::string iname = "Nemo");



/**! @class Clipper
 *   @brief A very simple class that pushes a clipping rectangle onto an image's clipping stack and pops it off when it goes out of scope
 *
 *   Usage :
 *   
 *   ```
 *       {
 *           Clipper clip(my_image , my_clipping_rectangle);
 *           /// Draw to my_image using the specified clipping rectangle
 *       }/// clip goes out of scope and pops the top clipping rectangle from my_image's clipping stack
 *   ```
 */

class Clipper {
	EagleImage* img;
   bool clip_worked;
public :
	Clipper(EagleImage* image , Rectangle clip);///< Pushes clip onto image's clipping rectangle stack
	~Clipper();///< Pops the top clipping rectangle off of image's clipping rectangle stack
};



#endif // EagleImage_HPP



