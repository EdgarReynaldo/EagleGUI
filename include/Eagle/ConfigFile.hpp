



#ifndef ConfigFile_HPP
#define ConfigFile_HPP

#include "Eagle/MemFile.hpp"


#include <vector>
#include <string>



class ConfigLine {
   bool comment;
   bool spacer;
   std::string line;
   std::string key;
   std::string value;
   
   void ParseLine();

public :
   ConfigLine();
   
   ConfigLine(std::string ln);

   void SetLine(std::string ln);

   void SetKeyAndValue(std::string k , std::string v);
   void SetKey(std::string k);
   void SetValue(std::string v);

   std::string Key() {return key;}
   std::string& Value() {return value;}
   std::string Line();
   bool IsComment() {return comment;}
   bool IsSpacer() {return spacer;}
};



class ConfigSection {

   std::vector<ConfigLine*> clines;

   std::vector<ConfigLine*>::iterator GetConfigIterator(std::string key);

   ConfigLine* FindConfig(std::string key);

public :
   
///   ConfigSection();
   ConfigSection() :
         clines()
   {}
   
   ConfigLine* GetConfigByKey(std::string key);

   void SetKeyValuePair(std::string key , std::string value);
   void RemoveLineByKey(std::string key);
   
   std::string& operator[](std::string key);
   
   void AddSpacer();
   void AddComment(std::string comment);
   void AddConfigLine(std::string line);
   void AddConfigLine(std::string key , std::string value);
   
   std::string GetConfigLine(int index);
   unsigned int NConfigLines() {return clines.size();}
};



class ConfigFile {

public :
   typedef std::map<std::string , ConfigSection> SECTIONMAP;
   typedef SECTIONMAP::iterator SMIT;
   
protected :
   std::string contents;
   SECTIONMAP sectionmap;

   void UpdateContents();
   
public :
///   ConfigSettings();
   ConfigFile() :
      contents(""),
      sectionmap()
   {}
   
   void Clear();
      
   bool LoadFromFile(const char* path);
   
   bool SaveToFile(const char* path);
   
   ConfigSection& operator[] (std::string section) {return sectionmap[section];}
};



#endif // ConfigFile_HPP

