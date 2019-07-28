
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
 * @file ObjectRegistry.hpp
 * @brief For tracking all EagleObject derived objects
 *   
 *   All EagleObject objects are registered with the EagleObjectRegisty. If you have an EAGLE_ID, you can get the address, name,
 *   and validity of the object. These attributes persist outside the scope of the EagleObject's themselves.
 *
 *   The registry allows you to retrieve a reference to the object, and you can use dynamic_cast or GetRealObject to retrieve
 *   a reference to the real class of the object. For instance, this allows you to retrieve objects such as fonts or images by their
 *   unique EAGLE_ID or more loosely, by their name (not guaranteed to be unique).
 *
 *   There is an object map, which tells us whether an object is registered and whether it is valid
 *   An object address can be re-used by the OS
 *   You can retrieve an EagleObject* from an object name or eagle id
 */

#ifndef ObjectRegistry_HPP
#define ObjectRegistry_HPP



#include <vector>
#include <string>
#include <map>
#include <utility>



#include "Eagle/Object.hpp"
#include "Eagle/ObjectInfo.hpp"



EAGLE_ID CurrentId();///< For retrieving the current id (the max id)



int TotalObjectCount();///< Total number of objects tracked

int LiveObjectCount();///< Number of live objects

void OutputLiveObjectsBrief();///< Output a brief list of live objects

void OutputLiveObjectsFull();///< Output a full list of live objects



std::vector<EagleObject*> GetObjectsByName(std::string name);///< Not the best way to get an object - use EAGLE_ID if possible

EagleObject* GetFirstObjectByName(std::string name);///< Get a pointer to the first object with this name

int GetNameCount(std::string name);///< Get a count of the number of objects with this name


EagleObject* GetObjectById   (EAGLE_ID eid);///< Get a pointer to an EagleObject by @ref EAGLE_ID
bool         GetValidById    (EAGLE_ID eid);///< Get whether this object is valid by EAGLE_ID
std::string  GetShortNameById(EAGLE_ID eid);///< Get the short name of this object by EAGLE_ID
std::string  GetClassNameById(EAGLE_ID eid);///< Get the class name of this object by EAGLE_ID
std::string  GetFullNameById (EAGLE_ID eid);///< Get the full name of this object by EAGLE_ID

EagleObjectInfo InfoById(EAGLE_ID eid);///< Get the full info on this object by EAGLE_ID


bool GetValidByAddress(EagleObject* obj);///< Get valid by address, prefer @ref GetValidById

std::string GetShortNameByAddress(EagleObject* obj);///< Get short name, prefer @ref GetShortNameById

std::string GetFullNameByAddress(EagleObject* obj);///< Get full name, prefer @ref GetFullNameById



class CXX11Mutex;

class EagleObjectRegistry {
   
   friend class EagleObject;///< For EagleObjects to automatically register themselves with the registry (which are private functions).
   
   typedef std::vector<EagleObjectInfo> EOBINFO;
   typedef std::map<EagleObject* , EAGLE_ID> ADDRESSMAP;
   typedef std::multimap<std::string , EAGLE_ID> NAMEMAP;
   typedef NAMEMAP::iterator NMIT;
   typedef std::pair<NMIT , NMIT> NAMERANGE;
   typedef std::pair<std::string , EAGLE_ID> NMVALUETYPE;

   CXX11Mutex* registry_mutex;
   
   EOBINFO* pinfo;
   ADDRESSMAP* paddressmap;
   NAMEMAP* pnamemap;

   unsigned int start_id;
   unsigned int stop_id;
   
   int destruct_count;


   void CheckIdRange(EAGLE_ID eid);

   void RemoveNameEntry(EAGLE_ID eid);

   void Register(EagleObject* object , std::string objclass , std::string name , EAGLE_ID eid);
   void Unregister(EAGLE_ID eid);

   static EagleObjectRegistry* registry;
   
   void LockOurMutex();
   void UnLockOurMutex();
   
   EagleObjectRegistry();
   ~EagleObjectRegistry();
   
   void Create();
   void Destroy();
public :
   

   static EagleObjectRegistry* Instance();///< Get the EagleObjectRegistry object
   
   bool Destroyed(EAGLE_ID eid);///< True if object[eid] is destroyed
   bool Valid(EAGLE_ID eid);///< True if object[eid] has not been destroyed
   std::string ShortName(EAGLE_ID eid);///< Short name for object[eid]
   std::string ClassName(EAGLE_ID eid);///< Class name for object[eid]
   std::string FullName(EAGLE_ID eid);///< Full name for object[eid]
   EagleObject* Object(EAGLE_ID eid);///< @ref EagleObject* for object[eid]
   
   bool Destroyed(EagleObject* obj);///< True if obj has been destroyed according to the registry
   bool Valid(EagleObject* obj);///< True if obj is valid, or has been resurrected by system memory
   std::string ShortName(EagleObject* obj);///< Short name for obj, favor @ref ShortName <EAGLE_ID>
   std::string FullName(EagleObject* obj);///< Full name for obj, favor @ref FullName <EAGLE_ID>

   EagleObjectInfo Info(EAGLE_ID eid);///< Get the info for this eid
   EagleObjectInfo FindInfoByAddress(EagleObject* object);///< May no longer refer to old object! May have been recycled!
   
   int TotalObjectCount();///< Same as @ref CurrentId
   int LiveObjectCount();///< Number of live objects in the registry

   std::vector<EagleObject*> GetObjectsByName(std::string name);///< Get all objects with this name

   EagleObject* GetFirstObjectByName(std::string name);///< Get only the first object with this name
   
   int GetNameCount(std::string name);///< Get the number of times this name appears

   EOBINFO GetRegistryCopy();///< Get a copy of the registry of EagleObjectInfo
   
   void OutputAllObjectsBrief();///< Output all objects using a brief description

   void OutputLiveObjectsBrief();///< Output live objects using a brief description

   void OutputLiveObjectsFull();///< Output live objects using a full description
};




#endif // ObjectRegistry_HPP



