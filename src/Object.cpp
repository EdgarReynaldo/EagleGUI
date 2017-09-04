
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
#include "Eagle/Exception.hpp"

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
   return EagleObjectRegistry::Instance()->TotalObjectCount();
}



int LiveObjectCount() {
   return EagleObjectRegistry::Instance()->LiveObjectCount();
}



void OutputLiveObjectsBrief() {
   EagleObjectRegistry::Instance()->OutputLiveObjectsBrief();
}



void OutputLiveObjectsFull() {
   EagleObjectRegistry::Instance()->OutputLiveObjectsFull();
}



std::vector<EagleObject*> GetObjectsByName(std::string name) {
   return EagleObjectRegistry::Instance()->GetObjectsByName(name);
}



EagleObject* GetFirstObjectByName(std::string name) {
   return EagleObjectRegistry::Instance()->GetFirstObjectByName(name);
}



int GetNameCount(std::string name) {
   return EagleObjectRegistry::Instance()->GetNameCount(name);
}



EagleObject* GetObjectById(int eid) {
   return EagleObjectRegistry::Instance()->Object(eid);
}



bool GetValidById(int eid) {
   return EagleObjectRegistry::Instance()->Valid(eid);
}



const char* GetShortNameById(int eid) {
   return EagleObjectRegistry::Instance()->ShortName(eid);
}



const char* GetFullNameById(int eid) {
   return EagleObjectRegistry::Instance()->FullName(eid);
}



bool GetValidByAddress(EagleObject* obj) {
   return EagleObjectRegistry::Instance()->Valid(obj);
}



const char* GetShortNameByAddress(EagleObject* obj) {
   return EagleObjectRegistry::Instance()->ShortName(obj);
}



const char* GetFullNameByAddress(EagleObject* obj) {
   return EagleObjectRegistry::Instance()->FullName(obj);
}



/// ------------------------------     EagleObjectInfo     ---------------------------------------



void EagleObjectInfo::RebuildName() {
   fullname = StringPrintF("%20s '%30s' : (EID %s %03d at %08p)" , 
                           classname.c_str() , shortname.c_str() , destroyed?"<X>":"<=>" , object->GetEagleId() , object);
}



EagleObjectInfo::EagleObjectInfo(EagleObject* obj , std::string sname) :
      object(obj),
      classname("Unknown Class"),
      shortname(sname),
      destroyed(false),
      fullname("")
{
   RebuildName();
}



void EagleObjectInfo::SetObject(EagleObject* pobject) {
   EAGLE_ASSERT(pobject);
   object = pobject;
   RebuildName();
}



void EagleObjectInfo::SetShortName(std::string short_name) {
   shortname = short_name;
   RebuildName();
}



void EagleObjectInfo::SetClassName(std::string class_name) {
   classname = class_name;
   RebuildName();
}



void EagleObjectInfo::SetDestroyedFlag() {
   destroyed = true;
   RebuildName();
}



void EagleObjectInfo::SetInfo(EagleObject* pobject , std::string short_name) {
   EAGLE_ASSERT(pobject);
   object = pobject;
   shortname = short_name;
   RebuildName();
}



EagleObject* EagleObjectInfo::GetObject() const {
   return object;
}



bool EagleObjectInfo::IsDestroyed() const {
   return destroyed;
}



const char* EagleObjectInfo::ShortName() const {
   return shortname.c_str();
}



const char* EagleObjectInfo::ClassName() const {
   return classname.c_str();
}



const char* EagleObjectInfo::FullName() const {
   return fullname.c_str();
}



/// ----------------------------------- EagleObjectRegistry -------------------------------------------




EagleObjectRegistry* EagleObjectRegistry::registry = 0;



void EagleObjectRegistry::CheckIdRange(EAGLE_ID eid) {
   (void)eid;
   EAGLE_DEBUG
   (
      bool valid = (eid >= start_id) && (eid < stop_id);
      if (!valid) {
         throw EagleException
         (StringPrintF("EagleObjectRegistry::CheckIdRange : EAGLE_ID %d out of range [%d,%d)" , eid , start_id , stop_id));
      }
   );
}



void EagleObjectRegistry::RemoveNameEntry(EAGLE_ID eid) {
   NAMERANGE namerange = pnamemap->equal_range(ShortName(eid));
   
   CheckIdRange(eid);
   
   for (NMIT nmit = namerange.first ; nmit != namerange.second ; ++nmit) {
      if (nmit->second == eid) {
         pnamemap->erase(nmit);
         break;
      }
   }
}



void EagleObjectRegistry::Register(EagleObject* object , std::string name , EAGLE_ID eid /* = EAGLE_ID_UNASSIGNED*/) {
   EAGLE_ASSERT(object);
   
///   EOBINFO& eobinfo = *Objects();
///   ADDRESSMAP& addrmap = *AddressMap();
///   NAMEMAP& namemap = *NameMap();

   int id_index = -1;
   
   if (eid != EAGLE_ID_UNASSIGNED) {
      /// In this case all we are doing is assigning a new name to the object
      
      CheckIdRange(eid);

      /// Remove the correct entry from the name map before inserting the new name element
      RemoveNameEntry(eid);
      
      NMVALUETYPE nmval(name , eid);
      pnamemap->insert(nmval);
            
      id_index = eid - start_id;

      EAGLE_ASSERT((*pinfo)[id_index].GetObject() == object);/// Make sure the object address matches for this eid
      
      (*pinfo)[id_index].SetShortName(name);

      return;
   }
   
   EAGLE_ID new_id = NextId();
   
   object->SetId(new_id);
   
   EagleLog() << StringPrintF("Creating eagle object '%s' at %p with eid %d\n" , name.c_str() , object , new_id).c_str() << std::endl;

   stop_id = new_id + 1;
   
   id_index = new_id - start_id;
   
   EAGLE_ASSERT((int)pinfo->size() == id_index);
   
   EagleObjectInfo eoi(object , name);
   
   pinfo->push_back(eoi);
   
   (*paddressmap)[object] = new_id;
   
   NMVALUETYPE nmval(name , new_id);

   pnamemap->insert(nmval);

}



void EagleObjectRegistry::Unregister(EAGLE_ID eid) {
   EAGLE_ASSERT(eid != EAGLE_ID_UNASSIGNED);
   
   CheckIdRange(eid);
   
   RemoveNameEntry(eid);
   
   int id_index = eid - start_id;
   
   EagleObjectInfo& eoi = (*pinfo)[id_index];
   
   EagleLog() << StringPrintF("Destroying eagle object %s" , eoi.FullName()) << std::endl;

   if (eoi.IsDestroyed()) {
      EagleError() << StringPrintF("Double destruction of eagle object '%s' detected." , eoi.FullName()) << std::endl;
      EAGLE_ASSERT(!eoi.IsDestroyed());/// Throw
   }
   else {
      ADDRESSMAP::iterator mit = paddressmap->find(eoi.GetObject());
      EAGLE_ASSERT(mit != paddressmap->end());
      paddressmap->erase(mit);
      eoi.SetDestroyedFlag();
      ++destruct_count;
   }
}



EagleObjectRegistry::EagleObjectRegistry() :
   pinfo(0),
   paddressmap(0),
   pnamemap(0),
   start_id(-1),
   stop_id(-1),
   destruct_count(0)
{
   Create();
}



EagleObjectRegistry::~EagleObjectRegistry() {
   Destroy();
}



void EagleObjectRegistry::Create() {
   if (!pinfo) {
      pinfo = new EOBINFO();
   }
   if (!paddressmap) {
      paddressmap = new ADDRESSMAP();
   }
   if (!pnamemap) {
      pnamemap = new NAMEMAP();
   }
}



void EagleObjectRegistry::Destroy() {
   if (pinfo) {
      delete pinfo;
      pinfo = 0;
   }
   if (paddressmap) {
      delete paddressmap;
      paddressmap = 0;
   }
   if (pnamemap) {
      delete pnamemap;
      pnamemap = 0;
   }
   start_id = stop_id = -1;
}



EagleObjectRegistry* EagleObjectRegistry::Instance() {
   if (!registry) {
      registry = new EagleObjectRegistry();
   }
   return registry;
}



bool EagleObjectRegistry::Destroyed(EAGLE_ID eid) {
   CheckIdRange(eid);
   return ((*pinfo)[eid-start_id].IsDestroyed());
}



bool EagleObjectRegistry::Valid(EAGLE_ID eid) {
   CheckIdRange(eid);
   return !((*pinfo)[eid-start_id].IsDestroyed());
}
   


const char* EagleObjectRegistry::ShortName(EAGLE_ID eid) {
   CheckIdRange(eid);
   return ((*pinfo)[eid-start_id].ShortName());
}



const char* EagleObjectRegistry::FullName(EAGLE_ID eid) {
   CheckIdRange(eid);
   return ((*pinfo)[eid-start_id].FullName());
}



EagleObject* EagleObjectRegistry::Object(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*pinfo)[eid-start_id].GetObject();
}



bool EagleObjectRegistry::Destroyed(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.IsDestroyed();
}



bool EagleObjectRegistry::Valid(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return !(info.IsDestroyed());
}



const char* EagleObjectRegistry::ShortName(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.ShortName();
}



const char* EagleObjectRegistry::FullName(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.FullName();
}



const EagleObjectInfo& EagleObjectRegistry::Info(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*pinfo)[eid - start_id];
}



const EagleObjectInfo& EagleObjectRegistry::FindInfoByAddress(EagleObject* object) {
   ADDRESSMAP::iterator mit = paddressmap->find(object);
   if (mit == paddressmap->end()) {
      throw EagleException(StringPrintF("EagleObjectRegistry::FindInfoByAddress : Could not find info for object %p" , object));
   }
   return (*pinfo)[mit->second - start_id];/// Access the object info using the EAGLE_ID key value stored in the address map
}



int EagleObjectRegistry::TotalObjectCount() {
   return (int)(pinfo->size());
}



int EagleObjectRegistry::LiveObjectCount() {
   return (int)(pinfo->size() - destruct_count);
}



std::vector<EagleObject*> EagleObjectRegistry::GetObjectsByName(std::string name) {

   NAMERANGE namerange = pnamemap->equal_range(name);
   
   std::vector<EagleObject*> objs;

   for (NMIT nmit = namerange.first ; nmit != namerange.second ; ++nmit) {
      objs.push_back(GetObjectById(nmit->second));
   }
   
   return objs;
}



EagleObject* EagleObjectRegistry::GetFirstObjectByName(std::string name) {

   NAMERANGE namerange = pnamemap->equal_range(name);
   
   NMIT nmit = namerange.first;
   
   if (nmit == pnamemap->end()) {
      return 0;
   }
   
   return Object(nmit->second);

}



int EagleObjectRegistry::GetNameCount(std::string name) {
   return pnamemap->count(name);
}



void EagleObjectRegistry::OutputLiveObjectsBrief() {
   
   for (unsigned int id_index = 0 ; id_index != pinfo->size() ; ++id_index) {
      const EagleObjectInfo& info = (*pinfo)[id_index];

      if (info.IsDestroyed()) {continue;}
      
      EagleInfo() << info.FullName() << std::endl;
   }
}



void EagleObjectRegistry::OutputLiveObjectsFull() {
   
   for (unsigned int id_index = 0 ; id_index != pinfo->size() ; ++id_index) {
      const EagleObjectInfo& info = (*pinfo)[id_index];

      if (info.IsDestroyed()) {continue;}
      
      EagleObject* obj = info.GetObject();
      
      EagleInfo() << info.FullName() << " {" << std::endl;
      Indenter i(1,3);
      obj->DescribeTo(EagleInfo() , i);
      EagleInfo() << "}" << std::endl;
   }
}



/// ----------------------------------- EagleObjectRegistryManager -------------------------------------------




/// ----------------------------------- EagleObject -------------------------------------------



void EagleObject::Register(EagleObject* obj , std::string name , EAGLE_ID eid) {
   EagleObjectRegistry::Instance()->Register(obj , name , eid);
}



void EagleObject::Unregister() {
   EagleObjectRegistry::Instance()->Unregister(id);
   
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
   EagleWarn() << "WARNING : EagleObject copy constructor called. RHS Object name will be duplicated." << std::endl;
   Register(this , std::string(rhs.ShortName()) , id);
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



const char* EagleObject::ShortName() const {
   return GetShortNameById(id);
}



const char* EagleObject::FullName() const {
   return GetFullNameById(id);
}



void EagleObject::SetShortName(std::string newname) {
   Register(this , newname , id);
}



ostream& EagleObject::DescribeTo(ostream& os , Indenter indent) const {
   EagleObjectInfo info = EagleObjectRegistry::Instance()->Info(id);
	return os << indent << FullName() << std::endl;
}


ostream& operator<<(ostream& os , const EagleObject& obj) {
   return obj.DescribeTo(os);
}



