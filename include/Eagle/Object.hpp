
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





void OutputObjects();
int ObjectCount();

//void RegisterObject(EagleObject* obj);
//void UnregisterObject(EagleObject* obj);

std::vector<EagleObject*> GetObjectsByName(std::string name);/// Not the best way to get an object - use EAGLE_ID instead


EagleObject* GetObjectById(EAGLE_ID eid);
bool         GetValidById (EAGLE_ID eid);
std::string  GetNameById  (EAGLE_ID eid);

///int GetIdByAddress(EagleObject* obj);

bool GetValidByAddress(EagleObject* obj);


class EagleObjectInfo {
public :
   EagleObject* object;
   
   std::string wname;
   
   bool destroyed;

///   EagleObjectInfo(EagleObject* obj , std::string name);
   EagleObjectInfo(EagleObject* obj , std::string name) :
         object(obj),
         wname(name),
         destroyed(false)
   {}
};



class EagleObjectRegistry {
   
   friend class EagleObject;
   
   typedef vector<EagleObjectInfo> EOBINFO;
   
   static EOBINFO* pinfo = 0;

   static int start_id = 0;
   static int stop_id = 0;
   
   static int id_offset = 0;

   static void CheckIdRange(EAGLE_ID eid);
   static EOBINFO* Objects();/// Creator function. Returns object info vector (Singleton)
   static void Register(EagleObject* object);
   static void Unregister(EAGLE_ID eid);

void EagleObjectRegistry::CheckIdRange(EAGLE_ID eid) {
   EAGLE_ASSERT((eid >= start_id) && (eid < stop_id));
}
   
EOBINFO* EagleObjectRegistry::Objects() {
   if (!pinfo) {
      pinfo = new EOBINFO();
   }
   return pinfo;
}
void Register(EagleObject* object , std::string name , EAGLE_ID eid /* = EAGLE_ID_UNASSIGNED*/) {
   EAGLE_ASSERT(object);
   
   EOBINFO& eobinfo = *Objects();

   int id_index = -1;
   
   if (eid != EAGLE_ID_UNASSIGNED) {
      CheckIdRange(eid);
      id_index = eid - start_id;
      EAGLE_ASSERT(eobinfo[id_index].object == object);
      eobinfo[id_index].wname = name;
      return;
   }
   
   EAGLE_ID new_id = NextId();
   
   object->SetId(new_id);
   
   stop_id = new_id + 1;
   
   id_index = new_id - start_id;
   
   EAGLE_ASSERT(eobinfo.size() == id_index);
   
   EagleObjectInfo eoi(object , name);
   
   eobinfo.push_back(eoi);
   
   EagleLog() << StringPrintF("Creating eagle object '%s' at %p\n" , eoi.wname.c_str() , eoi.object).c_str() << std::endl;
}
void Unregister(EAGLE_ID eid) {
   EAGLE_ASSERT(eid != EAGLE_ID_UNASSIGNED);
   
   EOBINFO& info = *Objects();
   
   CheckIdRange(eid);
   
   int id_index = eid - start_id;
   
   EagleObjectInfo& eoi = info[id_index];
   
   eoi.destroyed = true;
   
   EagleLog() << StringPrintF("Destroying eagle object '%s' at %p\n" , eoi.wname.c_str() , eoi.object).c_str() << std::endl;
}
   
public :
   
   EagleObjectRegistry();
   ~EagleObjectRegistry();

   
   static bool Destroyed(EAGLE_ID eid);
   static bool Valid(EAGLE_ID eid);
   static std::string Name(EAGLE_ID eid);
   static EagleObject* Object(EAGLE_ID eid);

EagleObjectRegistry::EagleObjectRegistry() :
{}

EagleObjectRegistry::~EagleObjectRegistry() {
   delete pinfo;
   pinfo = 0;
   start_id = stop_id = id_offset = -1;
}
   

bool EagleObjectRegistry::Destroyed(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*Objects())[eid-id_offset].destroyed;
}
bool EagleObjectRegistry::Valid(EAGLE_ID eid) {
   CheckIdRange(eid);
   return !((*Objects())[eid-id_offset].destroyed);
}
   
std::string EagleObjectRegistry::Name(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*Objects())[eid-id_offset].wname;
}
EagleObject* EagleObjectRegistry::Object(EAGLE_ID eid) {
   CheckIdRange(eid);
   return ((*Objects())[eid-id_offset].object;
}

};



class EagleObjectRegistryManager {
   
   static EagleObjectRegistry* registry=0;
   
   
public :
   EagleObjectRegistryManager();
   ~EagleObjectRegistryManager();

   static EagleObjectRegistry* Registry();
   void DeleteRegistry();

EagleObjectRegistryManager::EagleObjectRegistryManager() :
      registry(Registry())
{}

EagleObjectRegistryManager::~EagleObjectRegistryManager() {
   DeleteRegistry();
}


static EagleObjectRegistryManager::EagleObjectRegistry* Registry() {
   if (!registry) {
      registry = new EagleObjectRegistry();
   }
   return registry;
}

void EagleObjectRegistryManager::DeleteRegistry() {
   delete registry;
   registry = 0;
}
};


class EagleObject {

   friend class EagleObjectRegistry;

private :

   EAGLE_ID id;
///   bool destroyed;/// This is an unreliable method to track destruction of an EagleObject
                     /// because once it goes out of scope its value is undefined. Track separately.
   
   void SetId(EAGLE_ID eid) {id = eid;}
   
   void Register(EagleObject* obj , std::string name , EAGLE_ID eid);
   void Unregister();

                     
public :
   
   EagleObject();
   EagleObject(std::string name);
   
   virtual ~EagleObject();

   std::string GetName() const;
   void SetName(std::string newname);

   EAGLE_ID GetEagleId() {return id;}

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
//   virtual ostream& Describe(ostream& os);
};

std::ostream& operator<<(std::ostream& os , const EagleObject& obj);



#endif // EagleObject_HPP


