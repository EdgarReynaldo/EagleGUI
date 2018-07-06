
/**
 *
 *     _______       ___       ____      __       _______       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_    \ \ \_____
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\    \ \_____ \
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_    \/ ____\ \
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\     /\______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/     \/______/
 *
 *
 *    EAGLE 5
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Resources_HPP
#define Resources_HPP

#include "Eagle/File.hpp"

#include <memory>



typedef unsigned int RESOURCEID;

extern const RESOURCEID BADRESOURCEID;

RESOURCEID NextRid();/// Not thread safe - TODO

enum RESOURCE_TYPE {
   RT_UNKNOWN   =  0,
   RT_IMAGE     =  1,/// bmp png jpg tga
   RT_FONT      =  2,/// ttf bmp
   RT_AUDIO     =  3,/// ogg wav
   RT_VIDEO     =  4,/// ogv
   RT_ARCHIVE   =  5,/// zip, 7z
   RT_BINFILE   =  6,/// dat bin
   RT_TEXTFILE  =  7,/// txt
   NUM_RT_TYPES =  8
};



class ResourceBase {
protected :
   RESOURCEID rid;
   RESOURCE_TYPE rtype;
   
   FilePath filepath;
   MemFile memfile;
   
   ResourceBase(const ResourceBase& r);/// copying is prohibited
   ResourceBase& operator=(const ResourceBase& r);/// assignment is prohibited

public :
///   ResourceBase();
   ResourceBase(RESOURCE_TYPE rt = RT_UNKNOWN);

   virtual ~ResourceBase() {}

   /// Getters
   RESOURCEID RID() {return rid;}
   RESOURCE_TYPE RType() {return rtype;}

   /// Virtual methods
   virtual bool LoadFromFile(FilePath fp)=0;
   virtual bool LoadFromMemory(MemFile mf)=0;
   virtual bool Loaded()=0;
};



class ImageResource : public ResourceBase {
protected :
   std::shared_ptr<EagleImage> image;
   
public :
   ImageResource() :
         ResourceBase(RT_IMAGE),
         image(0)
   {}
   std::shared_ptr<EagleImage> GetImage() {return image;}
};



class FontResource : public ResourceBase {
protected :
   std::shared_ptr<EagleFont> font;
   
public :
   FontResource() :
         ResourceBase(RT_FONT),
         font(0)
   {}
   std::shared_ptr<EagleFont> GetFont() {return font;}
};



class AudioResource : public ResourceBase {
public :
   AudioResource() :
      ResourceBase(RT_AUDIO)
   {}
};



class VideoResource : public ResourceBase {
public :
   VideoResource() :
         ResourceBase(RT_VIDEO)
   {}
};



class ArchiveResource : public ResourceBase {
public :
   ArchiveResource() :
         ResourceBase(RT_ARCHIVE)
   {}
};



class BinaryResource : public ResourceBase {
public :
   BinaryResource() :
         ResourceBase(RT_BINFILE)
   {}
};



class TextResource : public ResourceBase {
public :
   TextResource() :
         ResourceBase(RT_TEXTFILE)
   {}
};





#endif // Resources_HPP




