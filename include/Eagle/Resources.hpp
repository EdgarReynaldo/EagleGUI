



#ifndef Resources_HPP
#define Resources_HPP

#include "Eagle/File.hpp"


typedef unsigned int RESOURCEID;

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
   void* pres;

   ResourceBase(const ResourceBase& r);/// copying is prohibited
   ResourceBase& operator=(const ResourceBase& r);/// assignment is prohibited

public :
///   ResourceBase();
   ResourceBase(RESOURCE_TYPE rt = RT_UNKNOWN);
   virtual ~ResourceBase() {}

   RESOURCEID RID() {return rid;}
   RESOURCE_TYPE RType() {return rtype;}
   void* Resource() {return pres;}

   virtual bool LoadFromFile(FilePath fp)=0;
///   virtual bool LoadFromMemory(MemFile mf)=0;
};



class ImageResource : public ResourceBase {
public :
   ImageResource() :
         ResourceBase(RT_IMAGE)
   {}
};



class FontResource : public ResourceBase {
public :
   FontResource() :
         ResourceBase(RT_FONT)
   {}
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




