
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleObject_HPP
#define EagleObject_HPP

#include "Eagle/Logging.hpp"

#include <string>
#include <map>
#include <vector>
#include <utility>

#include <iostream>


class EagleObject;


#include <string>



typedef int EAGLE_ID;
extern const EAGLE_ID EAGLE_ID_UNASSIGNED;


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


EagleObject* GetObjectById(EAGLE_ID eid);
bool         GetValidById (EAGLE_ID eid);
std::string  GetNameById  (EAGLE_ID eid);

///int GetIdByAddress(EagleObject* obj);

bool GetValidByAddress(EagleObject* obj);
std::string GetNameByAddress(EagleObject* obj);



class EagleObjectInfo {

public :

   EagleObject* object;
   std::string wname;
   bool destroyed;

   EagleObjectInfo(EagleObject* obj , std::string name) :
         object(obj),
         wname(name),
         destroyed(false)
   {}
};



typedef std::vector<EagleObjectInfo> EOBINFO;

typedef std::map<EagleObject* , EAGLE_ID> ADDRESSMAP;

typedef std::multimap<std::string , EAGLE_ID> NAMEMAP;

typedef NAMEMAP::iterator NMIT;

typedef std::pair<NMIT , NMIT> NAMERANGE;

typedef std::pair<std::string , EAGLE_ID> NMVALUETYPE;


class EagleObjectRegistry {
   
   friend class EagleObject;/// For EagleObjects to automatically register themselves with the registry (which are private functions).
   
   
   static EOBINFO* pinfo;
   static ADDRESSMAP* paddressmap;
   static NAMEMAP* pnamemap;

   static int start_id;
   static int stop_id;
   
   static int destruct_count;


   

      
   
   static void CheckIdRange(EAGLE_ID eid);

   static void RemoveNameEntry(EAGLE_ID eid);

   static EOBINFO* Objects();/// Creator function. Returns object info vector (Singleton)
   static ADDRESSMAP* AddressMap();/// Creator function. Returns address map (Singleton)
   static NAMEMAP* NameMap();/// Creator function. Returns name map (Singleton)

   static void Register(EagleObject* object , std::string name , EAGLE_ID eid);
   static void Unregister(EAGLE_ID eid);

   
public :
   
   EagleObjectRegistry();
   ~EagleObjectRegistry();

   static bool Destroyed(EAGLE_ID eid);
   static bool Valid(EAGLE_ID eid);
   static std::string Name(EAGLE_ID eid);
   static EagleObject* Object(EAGLE_ID eid);
   
   static bool Destroyed(EagleObject* obj);
   static bool Valid(EagleObject* obj);
   static std::string Name(EagleObject* obj);

   static const EagleObjectInfo& Info(EAGLE_ID eid);
   static const EagleObjectInfo& FindInfoByAddress(EagleObject* object);
   
   static int TotalObjectCount();
   static int LiveObjectCount();

   static std::vector<EagleObject*> GetObjectsByName(std::string name);

   static EagleObject* GetFirstObjectByName(std::string name);
   
   static int GetNameCount(std::string name);

   static void OutputLiveObjectsBrief();
   static void OutputLiveObjectsFull();
};




class EagleObjectRegistryManager {
   
   static EagleObjectRegistry* registry;
   
   static void DeleteRegistry();

public :
   EagleObjectRegistryManager();
   ~EagleObjectRegistryManager();

   static EagleObjectRegistry* Registry();
};


class EagleObject {

   friend class EagleObjectRegistry;/// For private SetId function

private :

   EAGLE_ID id;/// Unique id to track EagleObjects
   
   void SetId(EAGLE_ID eid) {id = eid;}
   
   void Register(EagleObject* obj , std::string name , EAGLE_ID eid);
   void Unregister();

                     
public :
   
   EagleObject();
   EagleObject(std::string name);
   
   EagleObject(const EagleObject& rhs);/// Generates warning about name duplication
   
   EagleObject& operator=(const EagleObject& rhs);/// Safe, ids are preserved
   
   virtual ~EagleObject();

   std::string GetName() const;
   void SetName(std::string newname);

   EAGLE_ID GetEagleId() {return id;}

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};

std::ostream& operator<<(std::ostream& os , const EagleObject& obj);




#endif // EagleObject_HPP


