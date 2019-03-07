



#error DEPRECATED

#ifndef Properties_HPP
#define Properties_HPP

#include <map>
#include <string>

typedef std::map<std::string , std::string> ATTRIBUTE_VALUE_MAP;
typedef ATTRIBUTE_VALUE_MAP::iterator AVMIT;



class AttributeValueMap {
   
   std::string avstr;

   ATTRIBUTE_VALUE_MAP avmap;
   
   void RefreshAVString();
   void RefreshAVMap();
   
public :
   AttributeValueMap();
   AttributeValueMap(std::string attributes);
   
   AttributeValueMap& operator=(const ATTRIBUTE_VALUE_MAP& valmap);
   
   void SetAttribute(std::string attribute , std::string value);

   std::string GetAttributeValue(std::string attribute);

   void SetAttributes(std::string attribute_value_set);/// semi-colon delimited list of colon separated attribute:value pairs

   void RemoveAttribute(std::string attribute);

   void Clear();

   const ATTRIBUTE_VALUE_MAP& GetMap() const {return avmap;}
   
   std::string operator[](std::string attribute) const;
   
   std::string ToString();
   
   bool Empty() {return avmap.empty();}
};


#endif // Properties_HPP

