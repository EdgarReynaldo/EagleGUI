
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 #include "Eagle/Exception.hpp"
 #include "Eagle/Object.hpp"
 #include "Eagle/ObjectRegistry.hpp"
 #include "Eagle/Logging.hpp"
 #include "Eagle/StringWork.hpp"
 
 
 
/// ----------------------------------- EagleObject -------------------------------------------



const EAGLE_ID EAGLE_ID_INVALID = ~0;



void EagleObject::Register(EagleObject* obj , std::string objclass , std::string name , EAGLE_ID eid) {
   EagleObjectRegistry::Instance()->Register(obj , objclass , name , eid);
}



void EagleObject::Unregister() {
   EagleObjectRegistry::Instance()->Unregister(id);
   
}



EagleObject::EagleObject(std::string objclass , std::string objname , bool log) :
      id(EAGLE_ID_INVALID),
      log_registration(log),
      our_info()
{
   Register(this , objclass , objname , id);
}



EagleObject::EagleObject(const EagleObject& rhs) :
   id(EAGLE_ID_INVALID),
   log_registration(rhs.log_registration),
   our_info()
{
   EagleWarn() << "WARNING : EagleObject copy constructor called. RHS Object name will be duplicated." << std::endl;
   Register(this , std::string(rhs.ClassName()) , std::string(rhs.ShortName()) , id);
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
   return our_info.ShortName();
}



const char* EagleObject::FullName() const {
   return our_info.FullName();
}



const char* EagleObject::ClassName() const {
   return our_info.ClassName();
}



EagleObjectInfo EagleObject::Info() const {
   return our_info;
}



void EagleObject::SetShortName(std::string newname) {
   Register(this , ClassName() , newname , id);
}



void EagleObject::SetClassName(std::string objclass) {
   Register(this , objclass , ShortName() , id);
}



EagleLogGuard& EagleObject::DescribeTo(EagleLogGuard&& guard , Indenter indent) const {
   std::ostream& os = (std::ostream&)(guard.log);
   DescribeTo(os , indent);
   return guard;
}



std::ostream& EagleObject::DescribeTo(std::ostream& os , Indenter indent) const {
	return os << indent << FullName() << std::endl;
}


std::ostream& operator<<(std::ostream& os , const EagleObject& obj) {
   return obj.DescribeTo(os);
}





