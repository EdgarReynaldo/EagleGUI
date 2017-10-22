


#ifndef ObjectInfo_HPP
#define ObjectInfo_HPP


#include <string>
class EagleObject;



class EagleObjectInfo {

   EagleObject* object;
   bool destroyed;
   std::string classname;
   std::string shortname;
   std::string fullname;
   
   void RebuildName();

   
public :

   EagleObjectInfo();
   EagleObjectInfo(EagleObject* obj , std::string objclass , std::string sname);
   ~EagleObjectInfo();

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





#endif // ObjectInfo_HPP

