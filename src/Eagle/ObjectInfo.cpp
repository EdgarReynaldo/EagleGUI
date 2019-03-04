
#include "Eagle/Exception.hpp"
#include "Eagle/ObjectInfo.hpp"
#include "Eagle/Object.hpp"

#include "Eagle/StringWork.hpp"



/// ------------------------------     EagleObjectInfo     ---------------------------------------



void EagleObjectInfo::RebuildName() {
///   fullname = StringPrintF("%20s '%30s' : (EID %s %03d at 0x%p)" , 
   fullname = StringPrintF("%s %s : (%s %4d at 0x%p)" ,
                           classname.c_str() , shortname.c_str() , destroyed?"XXX":"EID" , object->GetEagleId() , object);
}



EagleObjectInfo::EagleObjectInfo() :
      object(0),
      destroyed(false),
      classname(""),
      shortname(""),
      fullname("")
{}



EagleObjectInfo::EagleObjectInfo(EagleObject* obj , std::string objclass , std::string sname) :
      object(obj),
      destroyed(false),
      classname(objclass),
      shortname(sname),
      fullname("")
{
   RebuildName();
}



EagleObjectInfo::~EagleObjectInfo() {
   object = 0;
   destroyed = true;
   classname = "";
   shortname = "";
   fullname = "";
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


