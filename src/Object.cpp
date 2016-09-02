
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



#include "Eagle/Object.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Error.hpp"

#include <iostream>
#include <ostream>

using std::endl;
using std::ostream;


const EAGLE_ID EAGLE_ID_UNASSIGNED = -1;



static int next_id = 0;

static int NextId() {
   return next_id++;
}

int CurrentId() {
   return next_id;
}



int TotalObjectCount() {
   return EagleObjectRegistryManager::Registry()->TotalObjectCount();
}



int LiveObjectCount() {
   return EagleObjectRegistryManager::Registry()->LiveObjectCount();
}



void OutputLiveObjectsBrief() {
   EagleObjectRegistryManager::Registry()->OutputLiveObjectsBrief();
}



void OutputLiveObjectsFull() {
   EagleObjectRegistryManager::Registry()->OutputLiveObjectsFull();
}



std::vector<EagleObject*> GetObjectsByName(std::string name) {
   return EagleObjectRegistryManager::Registry()->GetObjectsByName(name);
}



EagleObject* GetFirstObjectByName(std::string name) {
   return EagleObjectRegistryManager::Registry()->GetFirstObjectByName(name);
}



int GetNameCount(std::string name) {
   return EagleObjectRegistryManager::Registry()->GetNameCount(name);
}



EagleObject* GetObjectById(int eid) {
   return EagleObjectRegistryManager::Registry()->Object(eid);
}



bool GetValidById(int eid) {
   return EagleObjectRegistryManager::Registry()->Valid(eid);
}



std::string GetNameById(int eid) {
   return EagleObjectRegistryManager::Registry()->Name(eid);
}



bool GetValidByAddress(EagleObject* obj) {
   return EagleObjectRegistryManager::Registry()->Valid(obj);
}



std::string GetNameByAddress(EagleObject* obj) {
   return EagleObjectRegistryManager::Registry()->Name(obj);
}




/// ----------------------------------- EagleObjectRegistry -------------------------------------------



EOBINFO* EagleObjectRegistry::pinfo = 0;
ADDRESSMAP* EagleObjectRegistry::paddressmap = 0;
NAMEMAP* EagleObjectRegistry::pnamemap = 0;

int EagleObjectRegistry::start_id = 0;
int EagleObjectRegistry::stop_id = 0;
int EagleObjectRegistry::destruct_count = 0;



void EagleObjectRegistry::CheckIdRange(EAGLE_ID eid) {
   (void)eid;
   EAGLE_DEBUG
   (
      bool valid = (eid >= start_id) && (eid < stop_id);
      if (!valid) {
         throw EagleError
         (StringPrintF("EagleObjectRegistry::CheckIdRange : EAGLE_ID %d out of range [%d,%d)" , eid , start_id , stop_id));
      }
   );
}



void EagleObjectRegistry::RemoveNameEntry(EAGLE_ID eid) {
   NAMEMAP& namemap = *NameMap();
   NAMERANGE namerange = namemap.equal_range(Name(eid));
   
   CheckIdRange(eid);
   
   for (NMIT nmit = namerange.first ; nmit != namerange.second ; ++nmit) {
      if (nmit->second == eid) {
         namemap.erase(nmit);
         break;
      }
   }
}



EOBINFO* EagleObjectRegistry::Objects() {
   if (!pinfo) {
      pinfo = new EOBINFO();
   }
   return pinfo;
}



ADDRESSMAP* EagleObjectRegistry::AddressMap() {
   if (!paddressmap) {
      paddressmap = new ADDRESSMAP();
   }
   return paddressmap;
}



NAMEMAP* EagleObjectRegistry::NameMap() {
   if (!pnamemap) {
      pnamemap = new NAMEMAP();
   }
   return pnamemap;
}



void EagleObjectRegistry::Register(EagleObject* object , std::string name , EAGLE_ID eid /* = EAGLE_ID_UNASSIGNED*/) {
   EAGLE_ASSERT(object);
   
   EOBINFO& eobinfo = *Objects();
   ADDRESSMAP& addrmap = *AddressMap();
   NAMEMAP& namemap = *NameMap();

   int id_index = -1;
   
   if (eid != EAGLE_ID_UNASSIGNED) {
      /// In this case all we are doing is assigning a new name to the object
      
      CheckIdRange(eid);

      /// Remove the correct entry from the name map before inserting the new name element
      RemoveNameEntry(eid);
      
      NMVALUETYPE nmval(name , eid);

      namemap.insert(nmval);
            
///      namemap[name] = eid;
      
      id_index = eid - start_id;

      EAGLE_ASSERT(eobinfo[id_index].object == object);/// Make sure the object address matches for this eid

      eobinfo[id_index].wname = name;

      return;
   }
   
   EAGLE_ID new_id = NextId();
   
   object->SetId(new_id);
   
   EagleLog() << StringPrintF("Creating eagle object '%s' at %p with eid %d\n" , name.c_str() , object , new_id).c_str() << std::endl;

   stop_id = new_id + 1;
   
   id_index = new_id - start_id;
   
   EAGLE_ASSERT((int)eobinfo.size() == id_index);
   
   EagleObjectInfo eoi(object , name);
   
   eobinfo.push_back(eoi);
   
   addrmap[object] = new_id;
   
   NMVALUETYPE nmval(name , new_id);

   namemap.insert(nmval);
         
///   namemap[name] = new_id;
}



void EagleObjectRegistry::Unregister(EAGLE_ID eid) {
   EAGLE_ASSERT(eid != EAGLE_ID_UNASSIGNED);
   
   EOBINFO& info = *Objects();
   ADDRESSMAP& addrmap = *AddressMap();
   
   CheckIdRange(eid);
   
   RemoveNameEntry(eid);
   
   int id_index = eid - start_id;
   
   EagleObjectInfo& eoi = info[id_index];
   
   EagleLog() << StringPrintF("Destroying eagle object '%s' at %p with eid %d\n" , eoi.wname.c_str() , eoi.object , eid).c_str() << std::endl;

   ADDRESSMAP::iterator mit = addrmap.find(eoi.object);
   
   EAGLE_ASSERT(mit != addrmap.end());
   
   addrmap.erase(mit);
   
   EAGLE_ASSERT(!(eoi.destroyed));
   
   eoi.destroyed = true;
   
   ++destruct_count;
   
}



EagleObjectRegistry::EagleObjectRegistry() 
{}



EagleObjectRegistry::~EagleObjectRegistry() {

   delete pinfo;
   pinfo = 0;

   delete paddressmap;
   paddressmap = 0;
   
   delete pnamemap;
   pnamemap = 0;
   
   start_id = stop_id = -1;

}



bool EagleObjectRegistry::Destroyed(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*Objects())[eid-start_id].destroyed;
}



bool EagleObjectRegistry::Valid(EAGLE_ID eid) {
   CheckIdRange(eid);
   return !((*Objects())[eid-start_id].destroyed);
}
   


std::string EagleObjectRegistry::Name(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*Objects())[eid-start_id].wname;
}



EagleObject* EagleObjectRegistry::Object(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*Objects())[eid-start_id].object;
}



bool EagleObjectRegistry::Destroyed(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.destroyed;
}



bool EagleObjectRegistry::Valid(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return !(info.destroyed);
}



std::string EagleObjectRegistry::Name(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.wname;
}



const EagleObjectInfo& EagleObjectRegistry::Info(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*Objects())[eid];
}



const EagleObjectInfo& EagleObjectRegistry::FindInfoByAddress(EagleObject* object) {
   EOBINFO& eobinfo = *Objects();
   ADDRESSMAP& addrmap = *AddressMap();
   ADDRESSMAP::iterator mit = addrmap.find(object);
   if (mit == addrmap.end()) {
      throw EagleError(StringPrintF("EagleObjectRegistry::FindInfoByAddress : Could not find info for object %p" , object));
   }
   return eobinfo[mit->second];/// Access the object info using the EAGLE_ID key value stored in the address map
}



int EagleObjectRegistry::TotalObjectCount() {
   return (int)Objects()->size();
}



int EagleObjectRegistry::LiveObjectCount() {
   return (int)Objects()->size() - destruct_count;
}



std::vector<EagleObject*> EagleObjectRegistry::GetObjectsByName(std::string name) {

   NAMEMAP& namemap = *NameMap();
   
   NAMERANGE namerange = namemap.equal_range(name);
   
   std::vector<EagleObject*> objs;

   for (NMIT nmit = namerange.first ; nmit != namerange.second ; ++nmit) {
      objs.push_back(GetObjectById(nmit->second));
   }
   
   return objs;
}



EagleObject* EagleObjectRegistry::GetFirstObjectByName(std::string name) {

   NAMEMAP& namemap = *NameMap();
   
   NAMERANGE namerange = namemap.equal_range(name);
   
   NMIT nmit = namerange.first;
   
   if (nmit == namemap.end()) {
      return 0;
   }
   
   return Object(nmit->second);

}



int EagleObjectRegistry::GetNameCount(std::string name) {
   
   NAMEMAP& namemap = *NameMap();
   
   return namemap.count(name);
   
}



void EagleObjectRegistry::OutputLiveObjectsBrief() {
   
   EOBINFO& eobinfo = *Objects();
   
   for (unsigned int id_index = 0 ; id_index != eobinfo.size() ; ++id_index) {
      const EagleObjectInfo& info = eobinfo[id_index];

      if (info.destroyed) {continue;}
      
      EagleLog() << StringPrintF("EagleObject at %p named '%s'" , info.object , info.wname.c_str()) << std::endl;
   }
}



void EagleObjectRegistry::OutputLiveObjectsFull() {
   
   EOBINFO& eobinfo = *Objects();
   
   for (unsigned int id_index = 0 ; id_index != eobinfo.size() ; ++id_index) {
      const EagleObjectInfo& info = eobinfo[id_index];

      if (info.destroyed) {continue;}
      
      OutputLog() << StringPrintF("EagleObject at %p named '%s'" , info.object , info.wname.c_str()) << std::endl;
      OutputLog() << *(info.object) << std::endl;
   }
}



/// ----------------------------------- EagleObjectRegistryManager -------------------------------------------



EagleObjectRegistry* EagleObjectRegistryManager::registry = Registry();



void EagleObjectRegistryManager::DeleteRegistry() {
   delete registry;
   registry = 0;
}



EagleObjectRegistryManager::EagleObjectRegistryManager()
{}



EagleObjectRegistryManager::~EagleObjectRegistryManager() {
   DeleteRegistry();
}



EagleObjectRegistry* EagleObjectRegistryManager::Registry() {
   if (!registry) {
      registry = new EagleObjectRegistry();
   }
   return registry;
}




/// ----------------------------------- EagleObject -------------------------------------------



void EagleObject::Register(EagleObject* obj , std::string name , EAGLE_ID eid) {
   EagleObjectRegistryManager::Registry()->Register(obj , name , eid);
}



void EagleObject::Unregister() {
   EagleObjectRegistryManager::Registry()->Unregister(id);
   
}



EagleObject::EagleObject() :
      id(EAGLE_ID_UNASSIGNED)
{
   Register(this , StringPrintF("Nemo at %p" , this) , id);
}



EagleObject::EagleObject(std::string name) :
      id(EAGLE_ID_UNASSIGNED)
{
   Register(this , name , id);
}



EagleObject::EagleObject(const EagleObject& rhs) :
   id(EAGLE_ID_UNASSIGNED)
{
   EagleLog() << "WARNING : EagleObject copy constructor called. RHS Object name will be duplicated." << std::endl;
   Register(this , rhs.GetName() , id);
}



EagleObject& EagleObject::operator=(const EagleObject& rhs) {
   /// DO NOT assign the id here - let objects go out of scope normally, ids do not change once assigned
   (void)rhs;
   EAGLE_DEBUG
   (
   EagleLog() << StringPrintF("EagleObject assignment operator called with lhs = %p and rhs = %p" , this , &rhs) << std::endl;
   );
   return *this;
}



EagleObject::~EagleObject() {
   Unregister();
}



std::string EagleObject::GetName() const {
   return GetNameById(id);
}



void EagleObject::SetName(std::string newname) {
   Register(this , newname , id);
}



ostream& EagleObject::DescribeTo(ostream& os , Indenter indent) const {
   EagleObjectInfo info = EagleObjectRegistryManager::Registry()->Info(id);
	return os << indent << StringPrintF("EagleObject %s. Addr=%p. Id = %i" , info.wname.c_str() , this , id);
}



ostream& operator<<(ostream& os , const EagleObject& obj) {
   return obj.DescribeTo(os);
}



