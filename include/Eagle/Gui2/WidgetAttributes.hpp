
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



bool IsKnownAttribute(const ATTRIBUTE& a);
void RegisterKnownAttribute(const ATTRIBUTE& a);
void RemoveKnownAttribute(const ATTRIBUTE& a);



class ATTRIBUTEVALUEMAP {
   
public :
   typedef std::string ATTRIBUTE;
   typedef std::string VALUE;
   typedef std::map<ATTRIBUTE , VALUE> ATTVALMAP;
   
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
   
   inline VALUE& operator[](ATTRIBUTE a);
   inline bool HasAttribute(ATTRIBUTE a) const;
   inline VALUE GetAttributeValue(ATTRIBUTE a);
   inline void SetAttribute(ATTRIBUTE a);
   inline void RemoveAttribute(ATTRIBUTE a);
};



inline VALUE& ATTRIBUTEVALUEMAP::operator[](ATTRIBUTE a) {
   return attributes[a];
}



inline bool ATTRIBUTEVALUEMAP::HasAttribute(const ATTRIBUTE& a) const {
   return attributes.find(a) != attributes.end();
}



inline VALUE ATTRIBUTEVALUEMAP::GetAttributeValue(ATTRIBUTE a) {
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

