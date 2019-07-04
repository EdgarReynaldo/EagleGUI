
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Exception.hpp"
#include "Eagle/ObjectRegistry.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/CXX11Mutexes.hpp"
#include "Eagle/Threads.hpp"




static unsigned int next_id = 0;



static unsigned int NextId() {
   return next_id++;
}



unsigned int CurrentId() {
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



EagleObject* GetObjectById(EAGLE_ID eid) {
   return EagleObjectRegistry::Instance()->Object(eid);
}



bool GetValidById(EAGLE_ID eid) {
   return EagleObjectRegistry::Instance()->Valid(eid);
}



std::string GetShortNameById(EAGLE_ID eid) {
   return EagleObjectRegistry::Instance()->ShortName(eid);
}



std::string GetClassNameById(EAGLE_ID eid) {
   return EagleObjectRegistry::Instance()->ClassName(eid);
}



std::string GetFullNameById(EAGLE_ID eid) {
   return EagleObjectRegistry::Instance()->FullName(eid);
}



EagleObjectInfo InfoById(EAGLE_ID eid) {
   return EagleObjectRegistry::Instance()->Info(eid);
}



bool GetValidByAddress(EagleObject* obj) {
   return EagleObjectRegistry::Instance()->Valid(obj);
}



std::string GetShortNameByAddress(EagleObject* obj) {
   return EagleObjectRegistry::Instance()->ShortName(obj);
}



std::string GetFullNameByAddress(EagleObject* obj) {
   return EagleObjectRegistry::Instance()->FullName(obj);
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



void EagleObjectRegistry::Register(EagleObject* object , std::string objclass , std::string name , EAGLE_ID eid /* = EAGLE_ID_UNASSIGNED*/) {
   EAGLE_ASSERT(object);
   
   int id_index = -1;
   
   if (eid != EAGLE_ID_INVALID) {
      /// In this case all we are doing is assigning a new name to the object
      
      LockOurMutex();

      CheckIdRange(eid);
      
      /// Remove the correct entry from the name map before inserting the new name element
      RemoveNameEntry(eid);
      
      NMVALUETYPE nmval(name , eid);
      pnamemap->insert(nmval);
            
      id_index = eid - start_id;

      EAGLE_ASSERT((*pinfo)[id_index].GetObject() == object);/// Make sure the object address matches for this eid
      
      (*pinfo)[id_index].SetShortName(name);
      (*pinfo)[id_index].SetClassName(objclass);
      
      object->our_info = (*pinfo)[id_index];

      UnLockOurMutex();
      
      return;
   }
   
   EAGLE_ID new_id = NextId();
   
   object->SetId(new_id);
   
   LockOurMutex();
   
   if (object->log_registration) {
      EaglePrefix("EAGLE_CREATE   : ") << StringPrintF("Creating %s object '%s' at %p with eid %d\n" , objclass.c_str() , name.c_str() , object , new_id) << std::endl;
   }
   
   stop_id = new_id + 1;
   
   id_index = new_id - start_id;
   
   EAGLE_ASSERT((int)pinfo->size() == id_index);
   
   EagleObjectInfo eoi(object , objclass , name);
   
   pinfo->push_back(eoi);
   
   object->our_info = eoi;
   
   (*paddressmap)[object] = new_id;
   
   NMVALUETYPE nmval(name , new_id);

   pnamemap->insert(nmval);

   UnLockOurMutex();
}



void EagleObjectRegistry::Unregister(EAGLE_ID eid) {
   EAGLE_ASSERT(eid != EAGLE_ID_INVALID);
   
   LockOurMutex();
   
   CheckIdRange(eid);
   
   RemoveNameEntry(eid);
   
   int id_index = eid - start_id;
   
   EagleObjectInfo& eoi = (*pinfo)[id_index];
   
   if (!eoi.IsDestroyed() && eoi.GetObject()->log_registration) {
      EaglePrefix("EAGLE_DESTROY  : ") << StringPrintF("Destroying eagle object %s" , eoi.FullName()) << std::endl;
   }
   
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
   
   UnLockOurMutex();
   
}



void EagleObjectRegistry::LockOurMutex() {
   if (registry_mutex) {
      ThreadLockMutex(NOT_A_THREAD , registry_mutex);
   }
}



void EagleObjectRegistry::UnLockOurMutex() {
   if (registry_mutex) {
      ThreadUnLockMutex(NOT_A_THREAD , registry_mutex);
   }
}



EagleObjectRegistry::EagleObjectRegistry() :
   registry_mutex(0),
   pinfo(0),
   paddressmap(0),
   pnamemap(0),
   start_id(0),
   stop_id(0),
   destruct_count(0)
{}



EagleObjectRegistry::~EagleObjectRegistry() {
   Destroy();
}



void EagleObjectRegistry::Create() {
   Destroy();
   
   if (!pinfo) {
      pinfo = new EOBINFO();
   }
   if (!paddressmap) {
      paddressmap = new ADDRESSMAP();
   }
   if (!pnamemap) {
      pnamemap = new NAMEMAP();
   }
   if (!registry_mutex) {
      registry_mutex = new CXX11Mutex("EagleObjectRegistry::registry_mutex" , false);
      registry_mutex->Create(false , false);
      EAGLE_ASSERT(registry_mutex->Valid());
   }
}



void EagleObjectRegistry::Destroy() {
   if (registry_mutex) {
      delete registry_mutex;
      registry_mutex = 0;
   }
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
   start_id = stop_id;
}



EagleObjectRegistry* EagleObjectRegistry::Instance() {
   if (!registry) {
      registry = new EagleObjectRegistry();
      registry->Create();
   }
   return registry;
}



bool EagleObjectRegistry::Destroyed(EAGLE_ID eid) {
   return Info(eid).IsDestroyed();
}



bool EagleObjectRegistry::Valid(EAGLE_ID eid) {
   return !Info(eid).IsDestroyed();
}
   


std::string EagleObjectRegistry::ShortName(EAGLE_ID eid) {
   return Info(eid).ShortName();
}



std::string EagleObjectRegistry::FullName(EAGLE_ID eid) {
   return Info(eid).FullName();
}



std::string EagleObjectRegistry::ClassName(EAGLE_ID eid) {
   return Info(eid).ClassName();
}



EagleObject* EagleObjectRegistry::Object(EAGLE_ID eid) {
   return Info(eid).GetObject();
}



bool EagleObjectRegistry::Destroyed(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.IsDestroyed();
}



bool EagleObjectRegistry::Valid(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return !(info.IsDestroyed());
}



std::string EagleObjectRegistry::ShortName(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.ShortName();
}



std::string EagleObjectRegistry::FullName(EagleObject* obj) {
   const EagleObjectInfo& info = FindInfoByAddress(obj);
   return info.FullName();
}



EagleObjectInfo EagleObjectRegistry::Info(EAGLE_ID eid) {
   CheckIdRange(eid);
   return (*pinfo)[eid - start_id];
}



EagleObjectInfo EagleObjectRegistry::FindInfoByAddress(EagleObject* object) {
   EAGLE_ID id = EAGLE_ID_INVALID;
   
   LockOurMutex();
   
   ADDRESSMAP::iterator mit = paddressmap->find(object);
   if (mit == paddressmap->end()) {
      throw EagleException(StringPrintF("EagleObjectRegistry::FindInfoByAddress : Could not find info for object %p" , object));
   }
   
   id = mit->second;
   
   UnLockOurMutex();
   
   return Info(id);
}



int EagleObjectRegistry::TotalObjectCount() {
   return (int)(pinfo->size());
}



int EagleObjectRegistry::LiveObjectCount() {
   return (int)(pinfo->size() - destruct_count);
}



std::vector<EagleObject*> EagleObjectRegistry::GetObjectsByName(std::string name) {

   std::vector<EagleObject*> objs;

   LockOurMutex();

   NAMERANGE namerange = pnamemap->equal_range(name);
   
   for (NMIT nmit = namerange.first ; nmit != namerange.second ; ++nmit) {
      objs.push_back(GetObjectById(nmit->second));
   }
   
   UnLockOurMutex();
   
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



EagleObjectRegistry::EOBINFO EagleObjectRegistry::GetRegistryCopy() {
   EOBINFO regcopy;
   LockOurMutex();
   if (pinfo) {
      regcopy = *pinfo;
   }
   UnLockOurMutex();
   return regcopy;
}



void EagleObjectRegistry::OutputAllObjectsBrief() {
   
   EOBINFO info = GetRegistryCopy();
   
   for (unsigned int id_index = 0 ; id_index < info.size() ; ++id_index) {
      const EagleObjectInfo& i = info[id_index];
      EagleInfo() << i.FullName() << std::endl;
   }
}



void EagleObjectRegistry::OutputLiveObjectsBrief() {
   
   EOBINFO info = GetRegistryCopy();
   
   for (unsigned int id_index = 0 ; id_index < info.size() ; ++id_index) {
      const EagleObjectInfo& i = info[id_index];
      
      if (i.IsDestroyed()) {continue;}
      
      EagleInfo() << i.FullName() << std::endl;
   }
}



void EagleObjectRegistry::OutputLiveObjectsFull() {
   
   EOBINFO info = GetRegistryCopy();
   
   for (unsigned int id_index = 0 ; id_index < info.size() ; ++id_index) {
      const EagleObjectInfo& i = info[id_index];

      if (i.IsDestroyed()) {continue;}
      
      EagleObject* obj = i.GetObject();
      
      EagleInfo() << i.FullName() << " {" << std::endl;
      Indenter indent(1,3);
      obj->DescribeTo(EagleInfo() , indent);
      --indent;
      EagleInfo() << "}" << std::endl;
   }
}



