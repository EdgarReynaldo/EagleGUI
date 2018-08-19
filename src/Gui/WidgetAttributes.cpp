
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





#include "Eagle/Gui/WidgetAttributes.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"



/// Global functions



bool IsKnownAttribute(const ATTRIBUTE& a) {
   return ATTRIBUTEVALUEMAP::KnownAttributes()->HasAttribute(a);
}



void RegisterKnownAttribute(const ATTRIBUTE& a) {
   ATTRIBUTEVALUEMAP::KnownAttributes()->AddAttribute(a);
}



void RemoveKnownAttribute(const ATTRIBUTE& a) {
   ATTRIBUTEVALUEMAP::KnownAttributes()->RemoveAttribute(a);
}



/// ATTRIBUTESET



bool ATTRIBUTEVALUEMAP::ATTRIBUTESET::HasAttribute(const ATTRIBUTE& a) {
   return attset.find(a) != attset.end();
}



void ATTRIBUTEVALUEMAP::ATTRIBUTESET::AddAttribute(const ATTRIBUTE& a) {
   attset.insert(a);
}



void ATTRIBUTEVALUEMAP::ATTRIBUTESET::RemoveAttribute(const ATTRIBUTE& a) {
   if (HasAttribute(a)) {
      attset.erase(attset.find(a));
   }
}



/// ATTRIBUTEVALUEMAP



ATTRIBUTEVALUEMAP* ATTRIBUTEVALUEMAP::GlobalAttributeMap() {
   static ATTRIBUTEVALUEMAP global_attribute_map;
   return &global_attribute_map;
}



ATTRIBUTEVALUEMAP::ATTRIBUTESET* ATTRIBUTEVALUEMAP::KnownAttributes() {
   static ATTRIBUTESET aset;
   return &aset;
}



VALUE ATTRIBUTEVALUEMAP::GetDefinedAttributeValue(const ATTRIBUTE& a) const {
   if (!HasAttribute(a)) {
      throw EagleException(StringPrintF("GetDefinedAttributeValue attribute '%s' unknown!!!\n" , a.c_str()));
   }
   ATTVALMAP::const_iterator cit = attributes.find(a);
   return cit->second;
}



std::ostream& ATTRIBUTEVALUEMAP::DescribeTo(std::ostream& os , Indenter indent) const {

   os << indent;

   ATTVALMAP::const_iterator it = attributes.begin();
   while (it != attributes.end()) {
      os << it->first << ":" << it->second << ";";
   }
   return os;
}



std::ostream& operator<<(std::ostream& os , const ATTRIBUTEVALUEMAP& avm) {
   return avm.DescribeTo(os);
}




