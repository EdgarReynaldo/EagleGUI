
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

#include "Eagle/Indenter.hpp"

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


EagleObject* GetObjectById   (EAGLE_ID eid);
bool         GetValidById    (EAGLE_ID eid);
const char*  GetShortNameById(EAGLE_ID eid);
const char*  GetClassNameById(EAGLE_ID eid);
const char*  GetFullNameById (EAGLE_ID eid);

///int GetIdByAddress(EagleObject* obj);

bool GetValidByAddress(EagleObject* obj);

const char* GetShortNameByAddress(EagleObject* obj);

const char* GetFullNameByAddress(EagleObject* obj);



class EagleObjectInfo {

   EagleObject* object;
   bool destroyed;
   std::string classname;
   std::string shortname;
   std::string fullname;
   
   void RebuildName();

public :

   EagleObjectInfo(EagleObject* obj , std::string objclass , std::string sname);

   void SetObject(EagleObject* pobject);
   void SetClassName(std::string class_name);
   void SetShortName(std::string short_name);
   void SetDestroyedFlag();
   
   void SetInfo(EagleObject* pobject , std::string short_name);/// , bool is_destroyed);

   EagleObject* GetObject() const;
   bool IsDestroyed() const;
   const char* ShortName() const;
   const char* ClassName() const;
   const char* FullName() const;
};



typedef std::vector<EagleObjectInfo> EOBINFO;

typedef std::map<EagleObject* , EAGLE_ID> ADDRESSMAP;

typedef std::multimap<std::string , EAGLE_ID> NAMEMAP;

typedef NAMEMAP::iterator NMIT;

typedef std::pair<NMIT , NMIT> NAMERANGE;

typedef std::pair<std::string , EAGLE_ID> NMVALUETYPE;


class EagleObjectRegistry {
   
   friend class EagleObject;/// For EagleObjects to automatically register themselves with the registry (which are private functions).
   
   
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



class EagleLogGuard;


class EagleObject {

   friend class EagleObjectRegistry;/// For private SetId function

private :

   EAGLE_ID id;/// Unique id to track EagleObjects
   
   void SetId(EAGLE_ID eid) {id = eid;}
   
   void Register(EagleObject* obj , std::string objclass , std::string name , EAGLE_ID eid);
   void Unregister();

///   virtual void SetClassName()=0;
                     
public :
   
///   EagleObject();
   EagleObject(std::string objclass , std::string objname);
   
   EagleObject(const EagleObject& rhs);/// Generates warning about name duplication
   
   EagleObject& operator=(const EagleObject& rhs);/// Safe, ids are preserved
   
   virtual ~EagleObject();

///   const char* GetNameCStr() const;
   const char* ShortName() const;
   const char* FullName() const;
   const char* ClassName() const;
   void SetName(std::string newname) {SetShortName(newname);}/// TODO : HACK : REMOVE
   void SetShortName(std::string newname);
   void SetClassName(std::string newname);

   EAGLE_ID GetEagleId() const {return id;}

   EagleLogGuard& DescribeTo(EagleLogGuard&& guard , Indenter indent = Indenter()) const ;
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
};

std::ostream& operator<<(std::ostream& os , const EagleObject& obj);

#endif // EagleObject_HPP


