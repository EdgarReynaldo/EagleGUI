
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





#include "Eagle/Gui2/WidgetAttributes.hpp"



/// Global functions



bool IsKnownAttribute(const ATTRIBUTE& a) {
   return KnownAttributes()->HasAttribute(a);
}



void RegisterKnownAttribute(const ATTRIBUTE& a) {
   KnownAttributes()->AddAttribute(a);
}



void RemoveKnownAttribute(const ATTRIBUTE& a) {
   KnownAttributes()->RemoveAttribute(a);
}



/// ATTRIBUTESET



bool ATTRIBUTESET::HasAttribute(const ATTRIBUTE& a) {
   return attset.find(a) != attset.end();
}



void ATTRIBUTESET::AddAttribute(const ATTRIBUTE& a) {
   attset.insert(a);
}



void ATTRIBUTESET::RemoveAttribute(const ATTRIBUTE& a) {
   if (AttributeInSet(a)) {
      attset.erase(attset.find(a));
   }
}



/// ATTRIBUTEVALUEMAP



ATTRIBUTEVALUEMAP* ATTRIBUTEVALUEMAP::GlobalAttributeMap() {
   static ATTRIBUTEVALUEMAP global_attribute_map;
   return &global_attribute_map;
}



ATTRIBUTESET* ATTRIBUTEVALUEMAP::KnownAttributes() {
   static ATTRIBUTESET aset;
   return &aset;
}




