
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





#include "Eagle/Gui/WidgetAttributes.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"



#include <sstream>



/// Global functions



bool IsKnownAttribute(const ATTRIBUTE& a) {
   return AttributeValueMap::KnownAttributes()->HasAttribute(a);
}



void RegisterKnownAttribute(const ATTRIBUTE& a) {
   AttributeValueMap::KnownAttributes()->AddAttribute(a);
}



void RemoveKnownAttribute(const ATTRIBUTE& a) {
   AttributeValueMap::KnownAttributes()->RemoveAttribute(a);
}



/// AttributeSet



bool AttributeValueMap::AttributeSet::HasAttribute(const ATTRIBUTE& a) {
   return attset.find(a) != attset.end();
}



void AttributeValueMap::AttributeSet::AddAttribute(const ATTRIBUTE& a) {
   attset.insert(a);
}



void AttributeValueMap::AttributeSet::RemoveAttribute(const ATTRIBUTE& a) {
   if (HasAttribute(a)) {
      attset.erase(attset.find(a));
   }
}



/// AttributeValueMap



AttributeValueMap* AttributeValueMap::GlobalAttributeMap() {
   static AttributeValueMap global_attribute_map;
   return &global_attribute_map;
}



AttributeValueMap::AttributeSet* AttributeValueMap::KnownAttributes() {
   static AttributeSet aset;
   return &aset;
}



VALUE AttributeValueMap::GetDefinedAttributeValue(const ATTRIBUTE& a) const {
   if (!HasAttribute(a)) {
      throw EagleException(StringPrintF("GetDefinedAttributeValue attribute '%s' unknown!!!\n" , a.c_str()));
   }
   ATTVALMAP::const_iterator cit = attributes.find(a);
   return cit->second;
}



std::ostream& AttributeValueMap::DescribeTo(std::ostream& os , Indenter indent) const {

   os << indent << ToString();

   return os;
}



std::string AttributeValueMap::ToString() const {
   std::stringstream ss;
   ATTVALMAP::const_iterator it = attributes.begin();
   while (it != attributes.end()) {
      ss << it->first << ":" << it->second << ";";
   }
   return ss.str();
}



std::ostream& operator<<(std::ostream& os , const AttributeValueMap& avm) {
   return avm.DescribeTo(os);
}




