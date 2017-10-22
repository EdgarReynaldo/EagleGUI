
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
 *    Copyright 2009-2017+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleObject_HPP
#define EagleObject_HPP



#include <iostream>
#include <string>

#include "Eagle/Indenter.hpp"
#include "Eagle/ObjectInfo.hpp"


typedef int EAGLE_ID;
extern const EAGLE_ID EAGLE_ID_UNASSIGNED;


class EagleLogGuard;


class EagleObject {

   friend class EagleObjectRegistry;/// For private SetId function

private :

   EAGLE_ID id;/// Unique id to track EagleObjects
   bool log_registration;/// Need this to disallow some logging so the EagleLogGuard can construct a mutex without calling itself
   EagleObjectInfo our_info;
   
   void SetId(EAGLE_ID eid) {id = eid;}
   
   void Register(EagleObject* obj , std::string objclass , std::string name , EAGLE_ID eid);
   void Unregister();

///   virtual void SetClassName()=0;
                     
public :
   
///   EagleObject();
   EagleObject(std::string objclass , std::string objname , bool log = true);
   
   EagleObject(const EagleObject& rhs);/// Generates warning about name duplication
   
   EagleObject& operator=(const EagleObject& rhs);/// Safe, ids are preserved
   
   virtual ~EagleObject();

   const char* ShortName() const;
   const char* FullName() const;
   const char* ClassName() const;

   EagleObjectInfo Info() const;
   
   void SetShortName(std::string newname);
   void SetClassName(std::string newname);

   EAGLE_ID GetEagleId() const {return id;}

   EagleLogGuard& DescribeTo(EagleLogGuard&& guard , Indenter indent = Indenter()) const ;
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};

std::ostream& operator<<(std::ostream& os , const EagleObject& obj);

#endif // EagleObject_HPP


