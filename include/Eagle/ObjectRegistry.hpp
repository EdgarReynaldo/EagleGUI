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



#ifndef ObjectRegistry_HPP
#define ObjectRegistry_HPP

#include <vector>
#include <string>
#include <map>
#include <utility>

#include "Eagle/Object.hpp"
#include "Eagle/ObjectInfo.hpp"



EAGLE_ID CurrentId();

/// There should be an object map, which tells us whether an object is registered and whether it is valid
/// An object address can be re-used by the OS
/// We should be able to retrieve an EagleObject* from an object name or eagle id
/// We should be able to retrieve a name from an eagle id, but what for?

/**
   All EagleObject objects are registered with the EagleObjectRegisty. If you have an EAGLE_ID, you can get the address, name,
   and validity of the object. These attributes persist outside the scope of the EagleObject's themselves.
   
   The registry allows you to retrieve a reference to the object, and you can use dynamic_cast or GetRealObject to retrieve
   a reference to the real class of the object. For instance, this allows you to retrieve objects such as fonts or images by their
   unique EAGLE_ID or more loosely, by their name (not guaranteed to be unique).
*/



int TotalObjectCount();

int LiveObjectCount();

void OutputLiveObjectsBrief();

void OutputLiveObjectsFull();



std::vector<EagleObject*> GetObjectsByName(std::string name);/// Not the best way to get an object - use EAGLE_ID if possible

EagleObject* GetFirstObjectByName(std::string name);

int GetNameCount(std::string name);


EagleObject* GetObjectById   (EAGLE_ID eid);
bool         GetValidById    (EAGLE_ID eid);
std::string  GetShortNameById(EAGLE_ID eid);
std::string  GetClassNameById(EAGLE_ID eid);
std::string  GetFullNameById (EAGLE_ID eid);

EagleObjectInfo InfoById(EAGLE_ID eid);

///int GetIdByAddress(EagleObject* obj);

bool GetValidByAddress(EagleObject* obj);

std::string GetShortNameByAddress(EagleObject* obj);

std::string GetFullNameByAddress(EagleObject* obj);



typedef std::vector<EagleObjectInfo> EOBINFO;

typedef std::map<EagleObject* , EAGLE_ID> ADDRESSMAP;

typedef std::multimap<std::string , EAGLE_ID> NAMEMAP;

typedef NAMEMAP::iterator NMIT;

typedef std::pair<NMIT , NMIT> NAMERANGE;

typedef std::pair<std::string , EAGLE_ID> NMVALUETYPE;


class CXX11Mutex;

class EagleObjectRegistry {
   
   friend class EagleObject;/// For EagleObjects to automatically register themselves with the registry (which are private functions).
   
   CXX11Mutex* registry_mutex;
   
   EOBINFO* pinfo;
   ADDRESSMAP* paddressmap;
   NAMEMAP* pnamemap;

   int start_id;
   int stop_id;
   
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
   

   static EagleObjectRegistry* Instance();
   
   bool Destroyed(EAGLE_ID eid);
   bool Valid(EAGLE_ID eid);
   std::string ShortName(EAGLE_ID eid);
   std::string ClassName(EAGLE_ID eid);
   std::string FullName(EAGLE_ID eid);
   EagleObject* Object(EAGLE_ID eid);
   
   bool Destroyed(EagleObject* obj);
   bool Valid(EagleObject* obj);
   std::string ShortName(EagleObject* obj);
   std::string FullName(EagleObject* obj);

   EagleObjectInfo Info(EAGLE_ID eid);
   EagleObjectInfo FindInfoByAddress(EagleObject* object);
   
   int TotalObjectCount();
   int LiveObjectCount();

   std::vector<EagleObject*> GetObjectsByName(std::string name);

   EagleObject* GetFirstObjectByName(std::string name);
   
   int GetNameCount(std::string name);

   EOBINFO GetRegistryCopy();
   
   void OutputAllObjectsBrief();
   void OutputLiveObjectsBrief();
   void OutputLiveObjectsFull();
};




#endif // ObjectRegistry_HPP

