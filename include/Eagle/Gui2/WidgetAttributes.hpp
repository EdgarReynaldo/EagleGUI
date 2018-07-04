
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
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 

#ifndef WidgetAttributes_HPP
#define WidgetAttributes_HPP


#include <string>
#include <map>
#include <unordered_set>

typedef std::string ATTRIBUTE;
typedef std::string VALUE;
typedef std::map<ATTRIBUTE , VALUE> ATTVALMAP;


bool IsKnownAttribute(const ATTRIBUTE& a);
void RegisterKnownAttribute(const ATTRIBUTE& a);
void RemoveKnownAttribute(const ATTRIBUTE& a);



class ATTRIBUTEVALUEMAP {
   
public :
   
   class ATTRIBUTESET {
      std::unordered_set<ATTRIBUTE> attset;
   public :
      bool HasAttribute(const ATTRIBUTE& a);
      void AddAttribute(const ATTRIBUTE& a);
      void RemoveAttribute(const ATTRIBUTE& a);
   };

protected :
   
   ATTVALMAP attributes;
   
public :
   
   static ATTRIBUTEVALUEMAP* GlobalAttributeMap();
   static ATTRIBUTESET* KnownAttributes();
   
   inline VALUE& operator[](const ATTRIBUTE& a);
   inline bool HasAttribute(const ATTRIBUTE& a) const;
   inline VALUE GetAttributeValue(const ATTRIBUTE& a);
   inline void SetAttribute(const ATTRIBUTE& a , const VALUE& v);
   inline void RemoveAttribute(const ATTRIBUTE& a);
};



inline VALUE& ATTRIBUTEVALUEMAP::operator[](const ATTRIBUTE& a) {
   return attributes[a];
}



inline bool ATTRIBUTEVALUEMAP::HasAttribute(const ATTRIBUTE& a) const {
   return attributes.find(a) != attributes.end();
}



inline VALUE ATTRIBUTEVALUEMAP::GetAttributeValue(const ATTRIBUTE& a) {
   return operator[](a);
}



inline void ATTRIBUTEVALUEMAP::SetAttribute(const ATTRIBUTE& a , const VALUE& v) {
   attributes[a] = v;
}



inline void ATTRIBUTEVALUEMAP::RemoveAttribute(const ATTRIBUTE& a) {
   if (HasAttribute(a)) {
      attributes.erase(attributes.find(a));
   }
}



#endif // WidgetAttributes_HPP

