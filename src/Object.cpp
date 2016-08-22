
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


///std::map<EagleObject* , std::string>         object_map;

std::multimap<std::string , EagleObject*>    object_name_map;

std::vector<std::string>                     id_name_map;

std::vector<std::pair<bool , EagleObject*> > id_valid_address_map;

///std::map<EagleObject* , int>                 object_id_map;/// Redundant



void RegisterObject(EagleObject* obj) {
   EAGLE_ASSERT(obj);
   if (object_map.find(obj) == object_map.end()) {
      object_map[obj] = obj->GetName();
      object_name_map.insert(std::pair<std::string , EagleObject*>(obj->GetName() , obj));
   }
   int id = obj->GetId();
   if (id == (int)id_name_map.size()) {
      id_name_map.resize(id_name_map.size() + 1 , "Uninitialized Eagle Object Id Name Map Element");
      id_valid_address_map.resize(id_valid_address_map.size() + 1 , std::pair<bool , EagleObject*>(false,0));
   }
   id_name_map[id] = obj->GetName();
   id_valid_address_map[id] = std::pair<bool , EagleObject*>(true , obj);
   object_id_map[obj] = obj->GetId();
}



void UnregisterObject(EagleObject* obj) {
   EAGLE_ASSERT(obj);
   std::map<EagleObject* , std::string>::iterator it = object_map.find(obj);
   if (it != object_map.end()) {
      object_map.erase(it);
      std::pair<std::multimap<std::string , EagleObject*>::iterator , std::multimap<std::string , EagleObject*>::iterator> range = object_name_map.equal_range(obj->GetName());
      object_name_map.erase(range.first , range.second);
   }
}



std::vector<EagleObject*> GetObjectsByName(std::string name) {
   std::vector<EagleObject*> objs;
   std::pair<std::multimap<std::string , EagleObject*>::iterator , std::multimap<std::string , EagleObject*>::iterator> range = object_name_map.equal_range(name);
   for (std::multimap<std::string , EagleObject*>::iterator it = range.first ; it != range.second ; ++it) {
      objs.push_back(it->second);
   }
   return objs;
}



void OutputObjects() {
   for (std::map<EagleObject* , std::string>::iterator it = object_map.begin() ; it != object_map.end() ; ++it) {
      OutputLog() << StringPrintF("EagleObject at %p named '%s'" , it->first , it->second.c_str()) << std::endl;
   }
}



int ObjectCount() {
   return object_map.size();
}



EagleObject* GetObjectById(int eid) {
   EAGLE_ASSERT(eid >= 0 && eid < (int)id_valid_address_map.size());
   if (eid < 0 || eid >= (int)id_valid_address_map.size()) {
      return 0;
   }
   return id_valid_address_map[eid].second;
}



bool GetValidById(int eid) {
   if (eid < 0) {return false;}
   
   EAGLE_ASSERT(eid >= 0 && eid < (int)id_valid_address_map.size());
   if (eid < 0 || eid >= (int)id_valid_address_map.size()) {
      return false;
   }
   return id_valid_address_map[eid].first;
}



std::string GetNameById(int eid) {
   EAGLE_ASSERT(eid >= 0 && eid < (int)id_name_map.size());
   if (eid < 0 || eid >= (int)id_name_map.size()) {
      return "Invalid ID";
   }
   return id_name_map[eid];
}



int GetIdByAddress(EagleObject* obj) {
   std::map<EagleObject* , int>::iterator it = object_id_map.find(obj);
   if (it != object_id_map.end()) {
      return it->second;
   }
   return -1;
}



bool GetValidByAddress(EagleObject* obj) {
   return GetValidById(GetIdByAddress(obj));
}



/// ----------------------------------- EagleObjectRegistry -------------------------------------------







/// ----------------------------------- EagleObjectRegistryManager -------------------------------------------






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

      wname(StringPrintF("Nemo at %p",this))
{
   EagleLog() << StringPrintF("Creating eagle object at %p named %s" , this , wname.c_str()) << std::endl;
   SetName(StringPrintF("Nemo at %p" , this));
///   EAGLE_DEBUG(RegisterObject(this););
}



EagleObject::EagleObject(std::string name) :
      id(EAGLE_ID_UNASSIGNED)
{
   Register(this , name , id);
}

{
   
   
   EagleLog() << StringPrintF("Creating object at %p named %s" , this , wname.c_str()) << std::endl;
   SetName(name);
///   EAGLE_DEBUG(RegisterObject(this););
}



EagleObject::~EagleObject() {
   Deregister();
}



std::string EagleObject::GetName() const {
   return GetNameById(id);
}



void EagleObject::SetName(std::string newname) {
   Register(this , newname , id);
}



ostream& EagleObject::DescribeTo(ostream& os , Indenter indent) const {
   EAGLE_ASSERT(!destroyed);
	return os << indent << StringPrintF("EagleObject %s. Addr=%p. Id = %i" , shortname.c_str() , this , id);
	///<< endl;
}



ostream& operator<<(ostream& os , const EagleObject& obj) {
   return obj.DescribeTo(os);
}



