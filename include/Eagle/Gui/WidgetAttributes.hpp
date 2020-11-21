
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
 * @file WidgetAttributes.hpp
 * @brief Interface for widget attribute storage and retrieval
 * 
 * WidgetAttributes.hpp contains a simple class to work with widget attributes and attribute values
 * 
 */

#ifndef WidgetAttributes_HPP
#define WidgetAttributes_HPP



#include <string>
#include <map>
#include <unordered_set>

#include "Eagle/Indenter.hpp"



typedef std::string ATTRIBUTE;
typedef std::string VALUE;
typedef std::map<ATTRIBUTE , VALUE> ATTVALMAP;
typedef ATTVALMAP::iterator AVMIT;


bool IsKnownAttribute(const ATTRIBUTE& a);
void RegisterKnownAttribute(const ATTRIBUTE& a);
void RemoveKnownAttribute(const ATTRIBUTE& a);



class AttributeValueMap {
   
public :
   
   class AttributeSet {
      std::unordered_set<ATTRIBUTE> attset;
   public :
      bool HasAttribute   (const ATTRIBUTE& a);///< Returns true if a is in the set
      void AddAttribute   (const ATTRIBUTE& a);///< Adds the attribute to the set
      void RemoveAttribute(const ATTRIBUTE& a);///< Removes the attribute from the set
   };

protected :
   
   ATTVALMAP attributes;///< Where the attribute value pairs are stored
   
public :
   
   static AttributeValueMap* GlobalAttributeMap();///< Returns the global attribute map
   static AttributeSet* KnownAttributes();///< Returns a set of the known attributes
   
   inline bool HasAttribute(const ATTRIBUTE& a) const;///< Returns true if a is in the map
   VALUE GetDefinedAttributeValue(const ATTRIBUTE& a) const;///< Returns the value associated with a, throws if not present

   inline VALUE& operator[](const ATTRIBUTE& a);///< Get a reference to the value associated with a
   inline VALUE GetAttributeValue(const ATTRIBUTE& a) const ;///< Get a copy of the value associated with a, empty if not present
   inline void SetAttribute(const ATTRIBUTE& a , const VALUE& v);///< Set the value v associated with a
   inline void RemoveAttribute(const ATTRIBUTE& a);///< Removes the specified attribute

   const ATTVALMAP& GetAttributeValueMap() const {return attributes;}///< Returns a const reference to the attribute value map
   
   void Clear() {attributes.clear();}///< Clears the attribute map
   bool Empty() {return attributes.empty();}///< Returns true if the attribute map is empty
   
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes the attribute map to an ostream
   
   std::string ToString() const;///< Returns the map parsed into ATTRIBUTE:VALUE; pairs
   
   friend std::ostream& operator<<(std::ostream& os , const AttributeValueMap& avm);///< Stream output
};



std::ostream& operator<<(std::ostream& os , const AttributeValueMap& avm);///< Output the attribute map to the stream



inline bool AttributeValueMap::HasAttribute(const ATTRIBUTE& a) const {
   return attributes.find(a) != attributes.end();
}



inline VALUE& AttributeValueMap::operator[](const ATTRIBUTE& a) {
   return attributes[a];
}



inline VALUE AttributeValueMap::GetAttributeValue(const ATTRIBUTE& a) const {
   ATTVALMAP::const_iterator it = attributes.find(a);
   if (it != attributes.end()) {
      return it->second;
   }
   return VALUE();
}



inline void AttributeValueMap::SetAttribute(const ATTRIBUTE& a , const VALUE& v) {
   attributes[a] = v;
}



inline void AttributeValueMap::RemoveAttribute(const ATTRIBUTE& a) {
   if (HasAttribute(a)) {
      attributes.erase(attributes.find(a));
   }
}



#endif // WidgetAttributes_HPP



