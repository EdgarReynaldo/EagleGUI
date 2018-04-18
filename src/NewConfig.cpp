



#include "Eagle/NewConfig.hpp"

#include "Eagle/FileSystem.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"



#include <sstream>
#include <fstream>






void ConfigSettings::UpdateContents() {
   contents = "";
   std::stringstream ss;
   SMIT it = sectionmap.begin();
   while (it != sectionmap.end()) {
      
      /// Write section header here
      ss << "[" << it->first << "]" << std::endl << std::endl;
         
      KEYMAP::iterator kit = (it->second).begin();
      while (kit != (it->second).end()) {
         
         /// Write key value pair here
         ss << kit->first << " = " << kit->second << std::endl;
         
         ++kit;
      }
      ++it;
      ss << std::endl;
   }
   ss << std::endl;
   contents = ss.str();
   ss.clear();
}



void ConfigSettings::Clear() {
   sectionmap.clear();
   contents = "";
}



bool ConfigSettings::LoadFromFile(const char* path) {
   Clear();
   
   FSInfo finfo = GetFileInfo(std::string(path));
   
   EAGLE_ASSERT(finfo.Mode().IsFile());
   MemFile mem(finfo);
   if (!mem.ReadFileIntoMemory()) {
      return false;
   }
   
   contents.insert(contents.begin() , mem.Begin() , mem.End());
   
   std::vector<std::string> lines = SplitByNewLines(contents);
   
   KEYMAP* section = &sectionmap["GLOBAL"];
   for (int i = 0 ; i < (int)lines.size() ; ++i) {
      std::string l = lines[i];
      if (!l.length()) {
         continue;
      }
      if (l[0] == '#') {
         continue;
      }
      unsigned int j = l.find_first_of('=');
      if (j == std::string::npos) {
         /// Not an assignment
         unsigned int i1 = l.find_first_of('[');
         unsigned int i2 = l.find_first_of(i1 , ']');
         if (i1 != std::string::npos && i2 != std::string::npos) {
             section = &sectionmap[l.substr(i1,i2)];
         }
         else {
            EagleWarn() << StringPrintF("Malformed section heading on line %d in config file %s\n" , i + 1 , path) << std::endl;
         }
      }
      else {
         /// Should have a key value pair
         std::string key = l.substr(0 , j);
         std::string value = l.substr(j + 1);
         std::string::reverse_iterator rit = key.rbegin();
         while (j > 0 && isspace(key[j-1])) {--j;}
         key = key.substr(0 , j);/// Trim trailing white space off of key
         j = 0;
         while (j < value.size() && isspace(value[j])) {++j;}
         value = value.substr(j);
      
         (*section)[key] = value;
      }
   }
}



bool ConfigSettings::SaveToFile(const char* path) {
   FSInfo info = GetFileInfo(path);
   if (info.Exists() && !info.Mode().CanWrite()) {
      throw EagleException(StringPrintF("ConfigSettings::SaveToFile - file %s is read only!\n" , path));
   }
   
   UpdateContents();
   
   std::ofstream fout(info.Path() , std::ios_base::out | std::ios_base::binary);
   if (!fout.good()) {
      return false;
   }
   fout << contents << std::endl;
   fout.close();
   return true;
}



std::string ConfigSettings::GetConfigString(std::string section , std::string key) {
   SMIT it = sectionmap.find(section);
   if (it == sectionmap.end()) {
      throw EagleException(StringPrintF("ConfigSettings::GetConfigString - section '%s' not found!\n" , section.c_str()));
   }
   KEYMAP::iterator kit = (it->second).find(key);
   if (kit == (it->second).end()) {
      throw EagleException(StringPrintF("ConfigSettings::GetConfigString - key '%s' not found in section %s!\n" ,
                                         key.c_str() , section.c_str()));
   }
   return kit->second;
}



int ConfigSettings::GetConfigInt(std::string section , std::string key) {
   std::string istr = GetConfigString(section , key);
   int n = 0;
   if (1 != sscanf(istr.c_str() , "%d" , &n)) {
      throw EagleException(StringPrintF("ConfigSettings::GetConfigInt - failed to read int value from '%s'\n" , istr.c_str()));
   }
   return n;
}



float ConfigSettings::GetConfigFloat(std::string section , std::string key) {
   std::string fstr = GetConfigString(section , key);
   float f = 0.0f;
   if (1 != sscanf(fstr.c_str() , "%f" , &f)) {
      throw EagleException(StringPrintF("ConfigSettings::GetConfigInt - failed to read float value from '%s'\n" , fstr.c_str()));
   }
   return f;
}



void ConfigSettings::SetConfigString(std::string section , std::string key , std::string value) {
   sectionmap[section][key] = value;
}



void ConfigSettings::SetConfigInt(std::string section , std::string key , int val) {
   SetConfigString(section , key , StringPrintF("%d" , val));
}



void ConfigSettings::SetConfigFloat(std::string section , std::string key , float val) {
   SetConfigString(section , key , StringPrintF("%f" , val));
}







