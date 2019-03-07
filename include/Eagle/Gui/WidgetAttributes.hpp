
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
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
      bool HasAttribute(const ATTRIBUTE& a);
      void AddAttribute(const ATTRIBUTE& a);
      void RemoveAttribute(const ATTRIBUTE& a);
   };

protected :
   
   ATTVALMAP attributes;
   
public :
   
   
   
   static AttributeValueMap* GlobalAttributeMap();
   static AttributeSet* KnownAttributes();
   
   inline bool HasAttribute(const ATTRIBUTE& a) const;
   VALUE GetDefinedAttributeValue(const ATTRIBUTE& a) const;

   inline VALUE& operator[](const ATTRIBUTE& a);
   inline VALUE GetAttributeValue(const ATTRIBUTE& a);
   inline void SetAttribute(const ATTRIBUTE& a , const VALUE& v);
   inline void RemoveAttribute(const ATTRIBUTE& a);

   const ATTVALMAP& GetAttributeValueMap() const {return attributes;}
   
   void Clear() {attributes.clear();}
   bool Empty() {return attributes.empty();}
   
   std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
   std::string ToString() const;
   
   friend std::ostream& operator<<(std::ostream& os , const AttributeValueMap& avm);
};



std::ostream& operator<<(std::ostream& os , const AttributeValueMap& avm);



inline bool AttributeValueMap::HasAttribute(const ATTRIBUTE& a) const {
   return attributes.find(a) != attributes.end();
}



inline VALUE& AttributeValueMap::operator[](const ATTRIBUTE& a) {
   return attributes[a];
}



inline VALUE AttributeValueMap::GetAttributeValue(const ATTRIBUTE& a) {
   return operator[](a);
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

