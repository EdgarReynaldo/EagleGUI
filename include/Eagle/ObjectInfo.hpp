
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ObjectInfo.hpp
 * @brief For tracking info related to @ref EagleObject objects
 */

#ifndef ObjectInfo_HPP
#define ObjectInfo_HPP



#include <string>



class EagleObject;


/**! @class EagleObjectInfo
 *   @brief To track info related to an EagleObject outside of itself so as to outlive object destruction
 */

class EagleObjectInfo {

   EagleObject* object;///< Pointer to the object we're tracking
   bool destroyed;///< True if destroyed
   std::string classname;///< Class name
   std::string shortname;///< Short name
   std::string fullname;///< Full name
   


   void RebuildName();///< Helper function

public :

   EagleObjectInfo();///< Empty default constructor
   
   /**! @fn EagleObjectInfo <EagleObject* , std::string , std::string>
    *   @brief Constructor to create a named object info with the specified class, name, and address
    */
   EagleObjectInfo(EagleObject* obj , std::string objclass , std::string sname);

   ~EagleObjectInfo();///< Empty destructor

   /// Setters
   
   void SetObject(EagleObject* pobject);     ///< Set the object we're tracking
   void SetClassName(std::string class_name);///< Set the class name
   void SetShortName(std::string short_name);///< Set the short name
   void SetDestroyedFlag();                  ///< Set the destroyed flag to true (one way!)
   
   void SetInfo(EagleObject* pobject , std::string short_name);///< Set our object and name
   
   /// Getters
   
   EagleObject* GetObject()   const;///< Get a pointer to the object we're tracking
   bool         IsDestroyed() const;///< True if destroyed already
   const char*  ShortName()   const;///< Get a cstr of the short name
   const char*  ClassName()   const;///< Get a cstr of the class name
   const char*  FullName()    const;///< Get a cstr of the full name
};



#endif // ObjectInfo_HPP



