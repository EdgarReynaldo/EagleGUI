



#ifndef NewConfig_HPP
#define NewConfig_HPP

#include "Eagle/MemFile.hpp"



typedef std::map<std::string , std::string> KEYMAP;
typedef std::map<std::string , KEYMAP > SECTIONMAP;
typedef SECTIONMAP::iterator SMIT;


class ConfigSettings {
protected :
   std::string contents;
   SECTIONMAP sectionmap;

   void UpdateContents();
   
public :
   
   
   void Clear();
      
   bool LoadFromFile(const char* path);
   
   bool SaveToFile(const char* path);
   
   std::string GetConfigString(std::string section , std::string key);
   int GetConfigInt(std::string section , std::string key);
   float GetConfigFloat(std::string section , std::string key);

   void SetConfigString(std::string section , std::string key , std::string value);
   void SetConfigInt(std::string section , std::string key , int val);
   void SetConfigFloat(std::string section , std::string key , float val);
   
   KEYMAP& operator[] (std::string section) {return sectionmap[section];}
};



#endif // NewConfig_HPP

