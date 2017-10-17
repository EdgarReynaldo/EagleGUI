


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
const char*  GetShortNameById(EAGLE_ID eid);
const char*  GetClassNameById(EAGLE_ID eid);
const char*  GetFullNameById (EAGLE_ID eid);

///int GetIdByAddress(EagleObject* obj);

bool GetValidByAddress(EagleObject* obj);

const char* GetShortNameByAddress(EagleObject* obj);

const char* GetFullNameByAddress(EagleObject* obj);



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

   EOBINFO* Objects();/// Creator function. Returns object info vector (Singleton)
   ADDRESSMAP* AddressMap();/// Creator function. Returns address map (Singleton)
   NAMEMAP* NameMap();/// Creator function. Returns name map (Singleton)

   void Register(EagleObject* object , std::string objclass , std::string name , EAGLE_ID eid);
   void Unregister(EAGLE_ID eid);

   static EagleObjectRegistry* registry;
   
   EagleObjectRegistry();
   ~EagleObjectRegistry();
   
   void Create();
   void Destroy();
public :
   

   static EagleObjectRegistry* Instance();
   
   bool Destroyed(EAGLE_ID eid);
   bool Valid(EAGLE_ID eid);
   const char* ShortName(EAGLE_ID eid);
   const char* ClassName(EAGLE_ID eid);
   const char* FullName(EAGLE_ID eid);
   EagleObject* Object(EAGLE_ID eid);
   
   bool Destroyed(EagleObject* obj);
   bool Valid(EagleObject* obj);
   const char* ShortName(EagleObject* obj);
   const char* FullName(EagleObject* obj);

   const EagleObjectInfo& Info(EAGLE_ID eid);
   const EagleObjectInfo& FindInfoByAddress(EagleObject* object);
   
   int TotalObjectCount();
   int LiveObjectCount();

   std::vector<EagleObject*> GetObjectsByName(std::string name);

   EagleObject* GetFirstObjectByName(std::string name);
   
   int GetNameCount(std::string name);

   void OutputLiveObjectsBrief();
   void OutputLiveObjectsFull();
};




#endif // ObjectRegistry_HPP

