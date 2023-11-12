
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
   RT_UNKNOWN      =  0,///< unsupported extension
   RT_IMAGE        =  1,///< bmp png jpg tga
   RT_FONT         =  2,///< ttf bmp
   RT_AUDIO_SAMPLE =  3,///< ogg wav mp3
   RT_AUDIO_STREAM =  4,///< ogg wav mp3
   RT_VIDEO        =  5,///< ogv
   RT_ARCHIVE      =  6,///< zip, 7z, tar, gz
   RT_BINFILE      =  7,///< dat bin exe dll
   RT_TEXTFILE     =  8,///< txt
   NUM_RT_TYPES    =  9
};


class ResourceLibrary;

class ResourceBase {

protected :
   ResourceLibrary* owner;
   RESOURCEID rid;
   RESOURCE_TYPE rtype;

   bool loaded;

   FilePath filepath;

   virtual bool LoadFromArgs(std::vector<std::string> args)=0;

   ResourceBase& operator=(const ResourceBase& r);
   ResourceBase(const ResourceBase& r);
public :
///   ResourceBase();
   ResourceBase(RESOURCE_TYPE rt = RT_UNKNOWN);

   virtual ~ResourceBase();

   
   
   /// Getters
   ResourceLibrary* Owner() {return owner;}
   RESOURCEID RID() {return rid;}
   RESOURCE_TYPE RType() {return rtype;}
   bool Loaded() {return loaded;}
   FilePath GetFilePath() {return filepath;}

   /// Load a resource
   bool LoadFromFile(FilePath fp);///< Shortcut for loading resources with no argumentsW
   bool LoadFromFileWithArgs(std::vector<std::string> args);///< File is first arg, rest are optional parameters
};



class EagleGraphicsContext;

class ArchiveResource : public ResourceBase {
protected :
   EagleGraphicsContext* win;

   std::shared_ptr<ArchiveFile> contents;
   std::map<std::string , ResourceBase*> resfilemap;
   
   
   virtual bool LoadFromArgs(std::vector<std::string> args);
   
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
   
   bool LoadFromArgs(std::vector<std::string> args) override;
   
   
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
   
   bool LoadFromArgs(std::vector<std::string> args) override;
   
public :
   TextResource() :
         ResourceBase(RT_TEXTFILE)
   {}
   
   const std::string& FileText() {return filetext;}
};





#endif // Resources_HPP




