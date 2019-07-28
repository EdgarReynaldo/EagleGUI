
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ResourceLib.hpp
 * @brief The main interface for working with resources and resource files in Eagle
 * 
 * TODO : Incomplete
 */

#ifndef ResourceLib_HPP
#define ResourceLib_HPP



#include <set>
#include <vector>
#include <string>
#include <memory>



#include "Eagle/Resources.hpp"
#include "Eagle/SharedMemory.hpp"



class ResourceLibrary {
public :
   typedef std::map<RESOURCEID , SHAREDOBJECT<ResourceBase> > RESMAP;
   typedef RESMAP::iterator RMIT;
   typedef std::map<RESOURCE_TYPE , std::set<std::string> > TYPEMAP;

protected :
   RESMAP resmap;
   TYPEMAP typemap;

public :
   
///   ResourceLibrary();
   ResourceLibrary() :
         resmap(),
         typemap()
   {}
   virtual ~ResourceLibrary();
   
   void FreeResources();
   
   std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt);
   
   RESOURCE_TYPE DeduceResourceType(std::string ext);

   SHAREDOBJECT<ResourceBase> GetResource(RESOURCEID rid);

///   virtual RESOURCEID LoadResourceFromFile(FilePath fp)=0;
///   virtual RESOURCEID LoadResourceFromMemory(const MemFile* memfile)=0;
};




#endif // ResourceLib_HPP



