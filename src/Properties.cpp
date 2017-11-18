

#include "Eagle/Properties.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"







void AttributeValueMap::RefreshAVString() {
   avstr = "";
   for (AVMIT it = avmap.begin() ; it != avmap.end() ; ++it) {
      avstr.append(it->first);
      avstr.append(":");
      avstr.append(it->second);
      avstr.append(";");
   }
}



void AttributeValueMap::RefreshAVMap() {
   avmap = ParseAttributeSet(avstr);
}



AttributeValueMap::AttributeValueMap() :
      avstr(""),
      avmap()
{}



AttributeValueMap::AttributeValueMap(std::string attributes) :
      avmap()
{
   SetAttributes(attributes);
}



AttributeValueMap& AttributeValueMap::operator=(const ATTRIBUTE_VALUE_MAP& valmap) {
   avmap = valmap;
   RefreshAVString();
   return *this;
}



void AttributeValueMap::SetAttribute(std::string attribute , std::string value) {
   avmap[attribute] = value;
   RefreshAVString();
}


   
std::string AttributeValueMap::GetAttributeValue(std::string attribute) {
   AVMIT it = avmap.find(attribute);
   if (it == avmap.end()) {
      return "";
   }
   return it->second;
}



void AttributeValueMap::SetAttributes(std::string attribute_value_set) {
   avstr = attribute_value_set;
   RefreshAVMap();
}



void AttributeValueMap::RemoveAttribute(std::string attribute) {
   AVMIT it = avmap.find(attribute);
   if (it != avmap.end()) {
      avmap.erase(it);
   }
   RefreshAVString();
}



void AttributeValueMap::Clear() {
   avmap.clear();
   avstr = "";
}



std::string AttributeValueMap::operator[](std::string attribute) const {
   std::map<std::string , std::string>::const_iterator it = avmap.find(attribute);
   if (it == avmap.end()) {
      return "";
   }
   return it->second;
}



std::string AttributeValueMap::ToString() {
   return avstr;
}




