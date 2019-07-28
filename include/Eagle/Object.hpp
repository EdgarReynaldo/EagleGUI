
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Object.hpp
 * @brief The interface and classes for working with EagleObject objects
 * 
 * This is a base class for all objects you wish to track in the @ref ObjectRegistry
 */

#ifndef EagleObject_HPP
#define EagleObject_HPP



#include <iostream>
#include <string>



#include "Eagle/Indenter.hpp"
#include "Eagle/ObjectInfo.hpp"


/**! @typedef EAGLE_ID
 *   @brief Typedef for the eagle id type. Retrieve any EagleObject from the @ref ObjectRegistry by using its EAGLE_ID.
 *   
 *   All EAGLE_IDs are unique, starting from zero
 *
 *   The only invalid EAGLE_ID is @ref EAGLE_ID_INVALID, but all ids greater than or equal to @ref CurrentId are out of range
 */

typedef unsigned int EAGLE_ID;

extern const EAGLE_ID EAGLE_ID_INVALID;///< Used for all invalid eagle ids



class EagleLogGuard;


/**! @class EagleObject
 *   @brief The base class for all objects you wish to track in Eagle.
 */

class EagleObject {

   friend class EagleObjectRegistry;///< For private @ref EagleObjectRegistry::SetId function

private :

   EAGLE_ID id;///< Unique id to track EagleObjects

   bool log_registration;///< Need this to disallow some logging so the EagleLogGuard can construct a mutex without calling itself

   EagleObjectInfo our_info;
   


   void SetId(EAGLE_ID eid) {id = eid;}///< Sets this object's id, for Registry only
   

   void Register(EagleObject* obj , std::string objclass , std::string name , EAGLE_ID eid);///< Register this object upon creation
   
   void Unregister();///< Unregister upon destruction


public :
   
///   EagleObject();
   EagleObject(std::string objclass , std::string objname = "Nemo" , bool log = true);///< Constructor for all eagle objects
   
   EagleObject(const EagleObject& rhs);///< Generates warning about name duplication
   
   EagleObject& operator=(const EagleObject& rhs);///< Safe, ids are preserved
   
   virtual ~EagleObject();///< Virtual destructor

   const char* ShortName() const;///< Get a cstr of the short name
   const char* FullName() const;///< Get a cstr of the full name
   const char* ClassName() const;///< Get a cstr of the class name

   EagleObjectInfo Info() const;///< Get the info on this object
   
   void SetShortName(std::string newname);///< Set this object's short name
   void SetClassName(std::string newname);///< Set this object's class name

   EAGLE_ID GetEagleId() const {return id;}///< Get the EAGLE_ID for this object

   EagleLogGuard& DescribeTo(EagleLogGuard&& guard , Indenter indent = Indenter()) const ;///< Describe to the log
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe to a stream
};



std::ostream& operator<<(std::ostream& os , const EagleObject& obj);///< Output an eagle object to a stream




#endif // EagleObject_HPP



