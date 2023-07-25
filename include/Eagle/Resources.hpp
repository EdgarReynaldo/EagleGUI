
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Resources.hpp
 * @brief The interface for Eagle resources
 */

#ifndef Resources_HPP
#define Resources_HPP



#include "Eagle/File.hpp"
#include "Eagle/MemFile.hpp"
#include "Eagle/SharedMemory.hpp"

#include <memory>
#include <atomic>

typedef unsigned int RESOURCEID;

extern const RESOURCEID BADRESOURCEID;

RESOURCEID NextRid();


/** @enum RESOURCE_TYPE
 *  @brief Simple deduction of resource type based on extension string for the file path minus the leading dot
 *     Supported image types are bmp png jpg and tga. Font extensions include ttf only. 
 *     Treatment of an image file as a font does not constitute it as such. Supported audio includes ogg and wav.
 *     Supported video containers are ogv through theora. Supported archives include zip files and 7z files.
 *     Second to last check sees whether they are a binary data file according to the extensions dat or bin.
 *     All other files are treated as text files in UTF8.
 */

enum RESOURCE_TYPE {
   RT_UNKNOWN   =  0,///< unsupported extension
   RT_IMAGE     =  1,///< bmp png jpg tga
   RT_FONT      =  2,///< ttf bmp
   RT_AUDIO     =  3,///< ogg wav
   RT_VIDEO     =  4,///< ogv
   RT_ARCHIVE   =  5,///< zip, 7z, tar
   RT_BINFILE   =  6,///< dat bin
   RT_TEXTFILE  =  7,///< txt
   NUM_RT_TYPES =  8
};


class ResourceLibrary;

class ResourceBase {

protected :
   ResourceLibrary* owner;
   RESOURCEID rid;
   RESOURCE_TYPE rtype;

   bool loaded;

   FilePath filepath;


   virtual bool LoadFromFilePath()=0;



//   MemFile memfile;

//   virtual bool LoadFromMemory(MemFile mf)=0;
   
   ResourceBase(const ResourceBase& r);/// copying is prohibited
   ResourceBase& operator=(const ResourceBase& r);/// assignment is prohibited

   
public :
///   ResourceBase();
   ResourceBase(RESOURCE_TYPE rt = RT_UNKNOWN);

   virtual ~ResourceBase();

   /// Getters
   RESOURCEID RID() {return rid;}
   RESOURCE_TYPE RType() {return rtype;}
   bool Loaded() {return loaded;}
   FilePath GetFilePath() {return filepath;}


   /// Virtual methods
   bool LoadFromFile(FilePath fp);


};



class EagleImage;
class EagleGraphicsContext;

class ImageResource : public ResourceBase {
protected :
   EagleGraphicsContext* window;
   EagleImage* image;
   
   bool LoadFromFilePath();

public :
   ImageResource(EagleGraphicsContext* win);
   ~ImageResource();

   void Free();

   EagleImage* GetImage();

};


class EagleFont;

class FontResource : public ResourceBase {
protected :
   
   EagleGraphicsContext* window;

   EagleFont* font;/// We don't own this, but we can ask to free it when we're done with it

   int pixelheight;/// pixel height of a font, in em for >0 and pixels for <0

      
   bool LoadFromFilePath();

public :
   FontResource(EagleGraphicsContext* win);
   ~FontResource();
   
   void Free();
   
   EagleFont* GetTheFont() {return font;}
};



class AudioResource : public ResourceBase {

   bool LoadFromFilePath();

public :
   AudioResource() :
      ResourceBase(RT_AUDIO)
   {}
};



class VideoResource : public ResourceBase {

   bool LoadFromFilePath();

public :
   VideoResource() :
         ResourceBase(RT_VIDEO)
   {}
};


class EagleSystem;
class FileSystem;

class ArchiveResource : public ResourceBase {
protected :
   EagleGraphicsContext* win;

   std::shared_ptr<Folder> contents;
   std::map<std::shared_ptr<File> , std::shared_ptr<ResourceBase> > resfilemap;
   
   
   bool LoadFromFilePath();
   
//   bool LoadFolder(std::shared_ptr<Folder> folder);
//   bool LoadFileResources(std::shared_ptr<Folder> folder);
//   bool LoadSubFolders(std::shared_ptr<Folder> folder);
//   bool LoadNestedArchives(std::shared_ptr<Folder> folder);
   
public :
   ArchiveResource(EagleGraphicsContext* window);
   ~ArchiveResource();
   
   std::shared_ptr<Folder> GetContents() {return contents;}
   
   void FreeContents();
   void FreeResources();
};

#include "Eagle/BinStream.hpp"



class BinaryResource : public ResourceBase {
protected : 
   BinStream binstream;
   
   bool LoadFromFilePath();
   
   
public :
   BinaryResource() :
         ResourceBase(RT_BINFILE),
         binstream()
   {}
   BinStream& GetBinStream() {return binstream;}
};

#include <string>

class TextResource : public ResourceBase {
protected :
   std::string filetext;
   
   bool LoadFromFilePath();
   
public :
   TextResource() :
         ResourceBase(RT_TEXTFILE)
   {}
   
   const std::string& FileText() {return filetext;}
};





#endif // Resources_HPP




