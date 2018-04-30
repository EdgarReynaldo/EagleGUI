



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

   std::string Key() const {return key;}
   std::string& Value() {return value;}
   const std::string& Value() const {return value;}
   std::string Line();
   
   bool IsComment() {return comment;}
   bool IsSpacer() {return spacer;}
   bool IsKeyValuePair() {return !comment && !spacer;}
};



class ConfigSection {

   std::vector<ConfigLine*> clines;

   std::vector<ConfigLine*>::iterator GetConfigIterator(std::string key);
   std::vector<ConfigLine*>::const_iterator GetConfigIteratorConst(std::string key) const;

   ConfigLine* FindConfig(std::string key);
   const ConfigLine* FindConfigConst(std::string key) const ;

public :
   
///   ConfigSection();
   ConfigSection() :
         clines()
   {}
   
   ConfigLine* GetConfigByKey(std::string key);

   void SetKeyValuePair(std::string key , std::string value);
   void RemoveLineByKey(std::string key);
   
   std::string& operator[](std::string key);
   const std::string& operator[] (std::string key) const ;
   
   void AddSpacer();
   void AddComment(std::string comment);
   void AddConfigLine(std::string line);
   void AddConfigLine(std::string key , std::string value);
   
   std::string GetConfigLine(int index) const ;
   unsigned int NConfigLines() const;
   
   std::vector<std::string> GetKeys() const;
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
   
   void Absorb(const ConfigFile& c);
   
   ConfigSection& operator[] (std::string section);
   const ConfigSection& operator[] (std::string section) const;

};



#endif // ConfigFile_HPP

